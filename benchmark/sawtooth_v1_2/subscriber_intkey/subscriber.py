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
        self._is_active = False
        self._height = 0

    def listen_to_event(self):
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
        LOGGER.info("Listening to events.")
        while True:
            msg = self._stream.receive()
            event_list = EventList()
            event_list.ParseFromString(msg.result().content)
            block_num, block_id = _parse_new_block(event_list.events)
            if block_num is not None:
                self._height = block_num





def _parse_new_block(events):
    try:
        block_attr = next(e.attributes for e in events
                          if e.event_type == 'sawtooth/block-commit')
    except StopIteration:
        return None, None

    block_num = int(next(a.value for a in block_attr if a.key == 'block_num'))
    block_id = next(a.value for a in block_attr if a.key == 'block_id')
    LOGGER.debug('Handling deltas for block: %s', block_id)
    return block_num, block_id
