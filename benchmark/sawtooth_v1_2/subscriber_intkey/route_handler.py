from json import JSONDecodeError
import logging
from aiohttp.web_response import json_response
from subscriber_intkey.blockchain_data import BlockchainData

from subscriber_intkey.errors import ApiBadRequest
LOGGER = logging.getLogger(__name__)

class RouteHandler(object):
    def __init__(self):
        self.blockchain_data = BlockchainData.getInstance()

    def get_height(self, request):
        height = self.blockchain_data.get_height()
        return json_response({"status": "0", "height": str(height)})

    async def get_block_transactions(self, request):

        if request.rel_url.query['num']is None:
            raise ApiBadRequest(
                "missing num query parameters")
        try:
            blknum = int(request.rel_url.query['num'])
        except Exception:
            raise ApiBadRequest(
                "block number must be int")
        transactions = self.blockchain_data.get_transactions(blknum)
        if transactions is None:
            raise ApiBadRequest(
                "block num '{}' does not exist ".format(blknum))
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
