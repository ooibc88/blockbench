import logging

LOGGER = logging.getLogger(__name__)


class BlockchainData(object):
    def __init__(self):
        LOGGER.warning("creating blockchain data")
        self._height = 0
        self._blkTxns = {}

    def get_height(self):
        return self._height

    def set_height(self, height):
        self._height = height

    def add_blkTxns(self, blk_num, txns):
        self._blkTxns[blk_num] = txns
