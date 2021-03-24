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
        return json_response({"height": height})

    async def get_block_transactions(self, request):
        LOGGER.warning(request)
        body = await decode_request(request)
        required_fields = ['num']
        validate_fields(required_fields, body)
        blknum = body.get('num')
        transactions = self.blockchain_data.get_transactions(blknum)
        if transactions is None:
            raise ApiBadRequest(
                "block num '{}' does not exist ".format(blknum))
        return json_response({"transactions": transactions})


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
