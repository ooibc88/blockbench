import pymongo
import logging

LOGGER = logging.getLogger(__name__)


class DatabaseImp(object):
    #URI = "mongodb://root:password@bb:27017/"
    DATABASE = None

    @staticmethod
    def initialize(uri):
        try:
            client = pymongo.MongoClient(uri)
            DatabaseImp.DATABASE = client['blocks']
            DatabaseImp.DATABASE["blkTxns"].insert({"block_num": 0, "transactions": []})
            DatabaseImp.DATABASE["height"].insert({"height": 0})
        except Exception as ex:
            LOGGER.warning(ex)

    @staticmethod
    def insert(collection, data):
        try:
            DatabaseImp.DATABASE[collection].insert(data)

        except Exception as ex:
            LOGGER.warning(ex)

    @staticmethod
    def find(collection, query):
        return DatabaseImp.DATABASE[collection].find(query)

    @staticmethod
    def find_one(collection, query):
        return DatabaseImp.DATABASE[collection].find_one(query)

    @staticmethod
    def find_last_record(collection):
        try:
            record = DatabaseImp.DATABASE[collection].find({}).sort("_id", -1).limit(1)
        except Exception as ex:
            LOGGER.warning(ex)
        return record.next()
