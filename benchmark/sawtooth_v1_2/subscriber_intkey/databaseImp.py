import pymongo
import logging

LOGGER = logging.getLogger(__name__)


class DatabaseImp(object):
    URI = "mongodb://root:password@bb:27017/"
    DATABASE = None

    @staticmethod
    def initialize():
        try:
            client = pymongo.MongoClient(DatabaseImp.URI)
            DatabaseImp.DATABASE = client['blocks']
        except Exception as ex:
            LOGGER.warning(ex)

    @staticmethod
    def insert(collection, data):
        try:
            LOGGER.warning("before insert ")
            DatabaseImp.DATABASE[collection].insert(data)
            LOGGER.warning("after insert")
        except Exception as ex:
            LOGGER.warning(ex)

    @staticmethod
    def find(collection, query):
        return DatabaseImp.DATABASE[collection].find(query)

    @staticmethod
    def find_one(collection, query):
        return DatabaseImp.DATABASE[collection].find_one(query)
