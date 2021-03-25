import logging

LOGGER = logging.getLogger(__name__)


class BlockchainData(object):
    __instance = None

    @staticmethod
    def getInstance():
        """ Static access method. """

        if BlockchainData.__instance is None:
            BlockchainData()
        return BlockchainData.__instance

    def __init__(self):
        """ Virtually private constructor. """
        if BlockchainData.__instance is not None:
            raise Exception("This class is a singleton!")
        else:
            self._height = 0
            self._blkTxns = {}
            BlockchainData.__instance = self

    def get_height(self):
        return self._height + 1

    def set_height(self, height):
        self._height = height

    def add_blkTxns(self, blk_num, txns):
        self._blkTxns[blk_num] = txns

    def get_transactions(self, blk_num):
        return self._blkTxns.get(blk_num)
