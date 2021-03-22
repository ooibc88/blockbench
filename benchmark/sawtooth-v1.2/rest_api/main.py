import argparse
import asyncio
import getpass
import logging
import os
import sys
import traceback


import pkg_resources
from aiohttp import web

from colorlog import ColoredFormatter


from rest_api.intkey_client import IntkeyClient
from rest_api.exceptions import IntKeyCliException
from rest_api.exceptions import IntkeyClientException
from rest_api.route_handler import RouteHandler
from zmq.asyncio import ZMQEventLoop

from sawtooth_signing import create_context
from sawtooth_signing.secp256k1 import Secp256k1PublicKey




LOGGER = logging.getLogger(__file__)
DISTRIBUTION_NAME = 'sawtooth-intkey'

DEFAULT_URL = 'http://127.0.0.1:8008'
ARGS = ""


def create_console_handler(verbose_level):
    clog = logging.StreamHandler()
    formatter = ColoredFormatter(
        "%(log_color)s[%(asctime)s %(levelname)-8s%(module)s]%(reset)s "
        "%(white)s%(message)s",
        datefmt="%H:%M:%S",
        reset=True,
        log_colors={
            'DEBUG': 'cyan',
            'INFO': 'green',
            'WARNING': 'yellow',
            'ERROR': 'red',
            'CRITICAL': 'red',
        })

    clog.setFormatter(formatter)

    if verbose_level == 0:
        clog.setLevel(logging.WARN)
    elif verbose_level == 1:
        clog.setLevel(logging.INFO)
    else:
        clog.setLevel(logging.DEBUG)

    return clog

def setup_loggers(verbose_level):
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    logger.addHandler(create_console_handler(verbose_level))


def create_parent_parser(prog_name):
    parent_parser = argparse.ArgumentParser(prog=prog_name, add_help=False)
    parent_parser.add_argument(
        '-v', '--verbose',
        action='count',
        help='enable more verbose output')
    # add bind port to run server
    parent_parser.add_argument(
        '-b', '--bind',
        help='identify host and port for api to run on',
        default='simple-supply-rest-api:8000')

    parent_parser.add_argument(
        '--url',
        type=str,
        help='specify URL of REST API')

    parent_parser.add_argument(
        '--keyfile',
        type=str,
        help="identify file containing user's private key")

    parent_parser.add_argument(
        '--wait',
        nargs='?',
        const=sys.maxsize,
        type=int,
        help='set time, in seconds, to wait for transaction to commit')

    try:
        version = pkg_resources.get_distribution(DISTRIBUTION_NAME).version
    except pkg_resources.DistributionNotFound:
        version = 'UNKNOWN'

    parent_parser.add_argument(
        '-V', '--version',
        action='version',
        version=(DISTRIBUTION_NAME + ' (Hyperledger Sawtooth) version {}')
            .format(version),
        help='display version information')

    return parent_parser


def generate_private_key():
    context = create_context('secp256k1')
    private_key = context.new_random_private_key()
    private_key_hex = Secp256k1PublicKey.as_hex(private_key)
    f = open("test.priv", "w")
    f.write(private_key_hex)
    f.close()


def start_rest_api(host, port):
    loop = asyncio.get_event_loop()
    app = web.Application(loop=loop)
    client = _get_client(ARGS)
    handler = RouteHandler(loop, client)

    app.router.add_post('/set', handler.create_record)

    LOGGER.warning('Starting REST API on %s:%s', host, port)
    web.run_app(
        app,
        host=host,
        port=port,
        access_log=LOGGER)


def _get_client(args, read_key_file=True):
    return IntkeyClient(
        url=DEFAULT_URL if args.url is None else args.url,
        keyfile=_get_keyfile(args) if read_key_file else None)


def _get_keyfile(args):
    try:
        if args.keyfile is not None:
            return args.keyfile
    except AttributeError:
        return None

    real_user = getpass.getuser()
    home = os.path.expanduser("~")
    key_dir = os.path.join(home, ".sawtooth", "keys")

    return '{}/{}.priv'.format(key_dir, real_user)



def main(prog_name=os.path.basename(sys.argv[0]), args=None):
    if args is None:
        args = sys.argv[1:]
    parser = create_parent_parser(prog_name)
    args = parser.parse_args(args)
    global ARGS
    ARGS = args
    if args.verbose is None:
        verbose_level = 0
    else:
        verbose_level = args.verbose
    setup_loggers(verbose_level=verbose_level)

    loop = ZMQEventLoop()
    asyncio.set_event_loop(loop)

    try:
        host, port = args.bind.split(":")
        port = int(port)
    except ValueError:
        print("Unable to parse binding {}: Must be in the format"
              " host:port".format(args.bind))
        sys.exit(1)

    start_rest_api(host, port)


def main_wrapper():
    # pylint: disable=bare-except
    try:
        generate_private_key()
        main()
    except (IntKeyCliException, IntkeyClientException) as err:
        print("Error: {}".format(err), file=sys.stderr)
        sys.exit(1)
    except KeyboardInterrupt:
        pass
    except SystemExit as e:
        raise e
    except:
        traceback.print_exc(file=sys.stderr)
        sys.exit(1)


main_wrapper()
