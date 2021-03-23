from aiohttp.web_response import json_response
from subscriber_intkey.event_handling import _get_height


class RouteHandler(object):
    def get_height(self, request):
        height = _get_height()
        return json_response({"height": height})
