import os
import getpass
from json import JSONDecodeError
import time
import yaml
import logging
from aiohttp.web_response import json_response
from sawtooth_sdk.processor.context import Context

from rest_api.errors import ApiBadRequest

DEFAULT_URL = 'http://127.0.0.1:8008'
LOGGER = logging.getLogger(__file__)


class RouteHandler(object):
    def __init__(self, loop, client):
        self._loop = loop
        self._client = client

    async def invoke_function(self, request):
        body = await decode_request(request)
        required_fields = ['function', 'args']
        validate_fields(required_fields, body)
        function = body.get('function')
        args = body.get("args")
        if function == "write":
            res = await self.create_record(args)
            return json_response(res)
        else:
            raise ApiBadRequest(
                "'{}' is not supported".format(function))

    async def create_record(self, args):
        start = time.time()
        if len(args) != 2:
            raise ApiBadRequest("write function must invoke two parameters")
        name = args[0]
        value = args[1]
        response = self._client.set(name, value)
        end = time.time() - start
        LOGGER.warning("getting status")
        LOGGER.warning(yaml.safe_load(response))
        link = yaml.safe_load(response)['link']
        batchID = link.split("=")[1]
        LOGGER.warning(link)
        res = {"batchID": batchID, "latency_sec": end}
        return res

    async def delete_record(self, arg):
        pass


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
