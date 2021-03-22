import os
import getpass
from json import JSONDecodeError

from aiohttp.web_response import json_response

from rest.errors import ApiBadRequest

DEFAULT_URL = 'http://127.0.0.1:8008'


class RouteHandler(object):
    def __init__(self, loop, client):
        self._loop = loop
        self._client = client

    async def create_record(self, request):
        body = await decode_request(request)
        required_fields = ['name', 'value']
        validate_fields(required_fields, body)
        name = body.get('name')
        value = body.get('value')

        response = self._client.set(name, value)
        return json_response(response)


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
