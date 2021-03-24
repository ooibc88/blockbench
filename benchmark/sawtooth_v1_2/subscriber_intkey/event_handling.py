import logging
from subscriber_intkey.blockchain_data import BlockchainData

LOGGER = logging.getLogger(__name__)


class EventHandler(object):
    __instance = None

    @staticmethod
    def getInstance():
        """ Static access method. """
        if EventHandler.__instance == None:
            EventHandler()
        return EventHandler.__instance

    def __init__(self):
        """ Virtually private constructor. """
        if EventHandler.__instance != None:
            raise Exception("This class is a singleton!")
        else:
            self._blockchain_data = BlockchainData()
            EventHandler.__instance = self

    # def __init__(self):
    #     self._blockchain_data = BlockchainData()

    def get_events_handler(self):
        return lambda events: self._handle_events(events)

    def _handle_events(self, events):
        block_num, block_id = self._parse_new_block(events)
        if block_num is not None:
            self._blockchain_data.set_height(block_num)

    def get_height(self):
        return self._blockchain_data.get_height()

    def _parse_new_block(self, events):
        try:
            block_attr = next(e.attributes for e in events
                              if e.event_type == 'sawtooth/block-commit')
        except StopIteration:
            return None, None

        block_num = int(next(a.value for a in block_attr if a.key == 'block_num'))
        block_id = next(a.value for a in block_attr if a.key == 'block_id')
        LOGGER.debug('Handling deltas for block: %s', block_num)
        return block_num, block_id
