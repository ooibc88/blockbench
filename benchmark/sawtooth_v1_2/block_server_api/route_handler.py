from json import JSONDecodeError
import logging
from aiohttp.web_response import json_response

from block_server_api.databaseImp import DatabaseImp
from block_server_api.errors import ApiBadRequest

LOGGER = logging.getLogger(__name__)


class RouteHandler(object):
    def __init__(self):
        pass

    def get_height(self, request):
        height = DatabaseImp.find_last_record("height")
        field = 0
        if height is not None:
            field = height.get("height")

        return json_response({"status": "0", "height": str(field)})

    async def get_block_transactions(self, request):

        if request.rel_url.query['num'] is None:
            raise ApiBadRequest(
                "missing num query parameters")
        try:
            blknum = int(request.rel_url.query['num'])
        except Exception:
            raise ApiBadRequest(
                "block number must be int")
        blockTxs = DatabaseImp.find_one("blkTxns", {"block_num": blknum})
        if blockTxs is None:
            raise ApiBadRequest(
                "block num '{}' does not exist ".format(blknum))
        transactions = blockTxs.get("transactions")
        return json_response({"status": "0", "txns": transactions})


async def decode_request(request):
    try:
        return await request.json()
    except JSONDecodeError:
        raise ApiBadRequest('Improper JSON format')


def validate_fields(required_fields, body):
    for field in required_fields:
        if body.get(field) is None:
            raise ApiBadRequest(
                "'{}' parameter is required".format(field))
