from aiohttp.web_response import json_response
from subscriber_intkey.event_handling import EventHandler


class RouteHandler(object):
    def __init__(self):
        self._event_handler = EventHandler.getInstance()

    def get_height(self, request):
        height = self._event_handler.get_height()
        return json_response({"height": height})
