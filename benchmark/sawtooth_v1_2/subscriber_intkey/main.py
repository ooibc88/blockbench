import argparse
import logging
import sys

from subscriber_intkey.subscriber import Subscriber

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


def do_subscribe(opts):
    LOGGER.info('Starting subscriber...')
    subscriber = Subscriber(opts.connect)
    subscriber.listen_to_event()


def main():
    opts = parse_args(sys.argv[1:])
    init_logger(opts.verbose)
    do_subscribe(opts)


main()
