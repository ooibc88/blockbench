import argparse
import asyncio
import logging
import sys
import threading

from subscriber_intkey.subscriber import Subscriber
from aiohttp import web

from subscriber_intkey.route_handler import RouteHandler
from subscriber_intkey.event_handling import EventHandler
from zmq.asyncio import ZMQEventLoop

#from subscriber_intkey.event_handling import get_events_handler
from subscriber_intkey.blockchain_data import BlockchainData
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
        default='block-txt-rest-api:8001')

    parser.add_argument(
        '-C', '--connect',
        help='The url of the validator to subscribe to',
        default='tcp://localhost:4004')

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


async def do_subscribe(opts):
    LOGGER.info('Starting subscriber...')
    subscriber = Subscriber(opts.connect)
    eventHandler = EventHandler.getInstance()
    subscriber.add_handler(eventHandler.get_events_handler())
    await subscriber.listen_to_event()


def start_rest_api(host, port, opts, loop):
    # start REST API
    app = web.Application(loop=loop)
    handler = RouteHandler()

    app.router.add_get('/height', handler.get_height)

    LOGGER.warning('Starting REST API on %s:%s', host, port)
    web.run_app(
        app,
        host=host,
        port=port,
        access_log=LOGGER)
    LOGGER.warning("out")


def main():
    LOGGER.warning("###########")
    opts = parse_args(sys.argv[1:])
    init_logger(opts.verbose)
    try:
        host, port = opts.bind.split(":")
        port = int(port)
    except ValueError:
        print("Unable to parse binding {}: Must be in the format"
              " host:port".format(opts.bind))
        sys.exit(1)
    loop = asyncio.get_event_loop()
    try:
        asyncio.ensure_future(do_subscribe(opts))
        start_rest_api(host, port, opts, loop)
        loop.run_forever()


    except KeyboardInterrupt:
        pass
    finally:
        print("Closing Loop")
        loop.close()


main()
