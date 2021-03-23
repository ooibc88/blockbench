import asyncio
import logging

from sawtooth_sdk.protobuf.client_event_pb2 import ClientEventsSubscribeRequest, ClientEventsSubscribeResponse
from sawtooth_sdk.protobuf.events_pb2 import EventSubscription, EventList

LOGGER = logging.getLogger(__name__)
from sawtooth_sdk.messaging.stream import Stream
from sawtooth_sdk.protobuf.validator_pb2 import Message


class Subscriber(object):
    def __init__(self, validator_url):
        LOGGER.info('Connecting to validator: %s', validator_url)
        self._stream = Stream(validator_url)
        self._event_handlers = []

    def add_handler(self, handler):
        self._event_handlers.append(handler)

    async def listen_to_event(self):
        self._stream.wait_for_ready()
        # Step 1: Construct a Subscription
        block_sub = EventSubscription(event_type='sawtooth/block-commit')

        # Step 2: Submit the Event Subscription
        request = ClientEventsSubscribeRequest(
            subscriptions=[block_sub])

        response_future = self._stream.send(
            Message.CLIENT_EVENTS_SUBSCRIBE_REQUEST,
            request.SerializeToString())

        response = ClientEventsSubscribeResponse()
        response.ParseFromString(response_future.result().content)

        # Listen for events in an infinite loop
        LOGGER.warning("Listening to events.")
        while True:
            # to avoid blocking not the best solution
            await asyncio.sleep(1)
            msg = self._stream.receive()
            event_list = EventList()
            event_list.ParseFromString(msg.result().content)
            for handler in self._event_handlers:
                handler(event_list.events)




