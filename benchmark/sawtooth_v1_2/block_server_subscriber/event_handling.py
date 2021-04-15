import logging

import requests
import yaml
from block_server_subscriber.databaseImp import DatabaseImp
import time

LOGGER = logging.getLogger(__name__)


class EventHandler(object):

    def __init__(self, rest_api_url):
        self._url = rest_api_url
        self.retry = 2

    def get_events_handler(self):
        return lambda events: self._handle_events(events)

    def _handle_events(self, events):
        block_num, block_id = self._parse_new_block(events)
        if block_num is not None:
            try:
                transactionIDS = self._get_txnts(block_id)
                DatabaseImp.insert("blkTxns", {"block_num": block_num, "transactions": transactionIDS})
                # get transactions id of the new block
                DatabaseImp.insert("height", {"height": block_num})

            except Exception as ex:
                LOGGER.warning(ex)

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

    def _send_get_txts(self,block_id):
        try:
            result = self._send_request(
                'blocks/{}'.format(block_id))
            return result
        except Exception as ex:
            return 0




    def _get_txnts(self, block_id):
        try:
            transactionIDS = []
            result = 0
            i = 0
            while result == 0:
                i = i+1
                LOGGER.warning("## getting transactions ## " + str(i))
                result = self._send_get_txts(block_id)

            i = 0
            res = yaml.safe_load(result)
            for batch in res['data']['batches']:
                for transactionId in batch['header']['transaction_ids']:
                    transactionIDS.append(transactionId)
            return transactionIDS

        except Exception as err:
            raise Exception(err) from err

    def _send_request(self, suffix, data=None, content_type=None):
        if self._url.startswith("http://"):
            url = "{}/{}".format(self._url, suffix)
        else:
            url = "http://{}/{}".format(self._url, suffix)
        headers = {}

        if content_type is not None:
            headers['Content-Type'] = content_type

        try:
            if data is not None:
                result = requests.post(url, headers=headers, data=data)
            else:
                result = requests.get(url, headers=headers)

            # if result.status_code == 404:
            #     raise Exception("No such key: {}".format(name))

            if not result.ok:
                raise Exception("Error {}: {}".format(

                    result.status_code, result.reason))


        except requests.ConnectionError as err:

            raise Exception(

                'Failed to connect to REST API: {}'.format(err)) from err


        except BaseException as err:

            raise Exception(err) from err

        return result.text
