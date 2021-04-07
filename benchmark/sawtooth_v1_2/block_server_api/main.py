import argparse
import asyncio
import logging
import sys
import threading

from aiohttp import web

from block_server_api.route_handler import RouteHandler
from zmq.asyncio import ZMQEventLoop
from block_server_api.databaseImp import DatabaseImp

LOGGER = logging.getLogger(__name__)


def parse_args(args):
    parser = argparse.ArgumentParser(add_help=False)

    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument(
        '-v', '--verbose',
        action='count',
        default=0,
        help='Increase output sent to stderr')

    parser.add_argument(
        '-b', '--bind',
        help='identify host and port for api to run on',
        default='block-server-rest-api:9001')


    return parser.parse_args(args)


def init_logger(level):
    logger = logging.getLogger()
    logger.addHandler(logging.StreamHandler())
    if level == 1:
        logger.setLevel(logging.INFO)
    elif level > 1:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.WARN)



def start_rest_api(host, port, opts, loop):
    # start REST API
    app = web.Application(loop=loop)
    handler = RouteHandler()

    app.router.add_get('/height', handler.get_height)
    app.router.add_get('/block', handler.get_block_transactions)

    LOGGER.warning('Starting REST API on %s:%s', host, port)
    web.run_app(
        app,
        host=host,
        port=port,
        access_log=LOGGER)


def main():
    LOGGER.warning("## in api ##")
    opts = parse_args(sys.argv[1:])
    init_logger(opts.verbose)
    try:
        host, port = opts.bind.split(":")
        port = int(port)
    except ValueError:
        print("Unable to parse binding {}: Must be in the format"
              " host:port".format(opts.bind))
        sys.exit(1)
    loop = ZMQEventLoop()
    asyncio.set_event_loop(loop)
    try:
        DatabaseImp.initialize()
        start_rest_api(host, port, opts, loop)


    except KeyboardInterrupt:
        pass
    finally:
        print("Closing Loop")
        loop.close()


main()
