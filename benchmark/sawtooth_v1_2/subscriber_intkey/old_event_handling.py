import logging

LOGGER = logging.getLogger(__name__)
height = 0


def get_events_handler(blockchain_data):
    return lambda events: _handle_events(blockchain_data, events)


def _handle_events(blockchain_data, events):
    block_num, block_id = _parse_new_block(events)
    if block_num is not None:
        blockchain_data.set_height(block_num)
        LOGGER.warning(blockchain_data.get_height())





def _get_height():
    LOGGER.warning("in handle event")
    LOGGER.warning(blockchain_data.get_height())


def _parse_new_block(events):
    try:
        block_attr = next(e.attributes for e in events
                          if e.event_type == 'sawtooth/block-commit')
    except StopIteration:
        return None, None

    block_num = int(next(a.value for a in block_attr if a.key == 'block_num'))
    block_id = next(a.value for a in block_attr if a.key == 'block_id')
    LOGGER.debug('Handling deltas for block: %s', block_num)
    return block_num, block_id
