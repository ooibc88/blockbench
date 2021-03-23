import logging

LOGGER = logging.getLogger(__name__)
height = 0


def get_events_handler():
    """Returns a events handler with a reference to a specific Database object.
    The handler takes a list of events and updates the Database appropriately.
    """
    return lambda events: _handle_events(events)


def _handle_events(events):
    block_num, block_id = _parse_new_block(events)
    if block_num is not None:
        global height
        height = block_num
        LOGGER.warning("block num")
        LOGGER.warning(block_num)




def _get_height():
    global height
    return height


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
