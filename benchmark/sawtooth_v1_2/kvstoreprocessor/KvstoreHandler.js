const { TextEncoder, TextDecoder } = require('text-encoding/lib/encoding')
const { TransactionHandler } = require('sawtooth-sdk/processor/handler')
const {
  InvalidTransaction,
  InternalError
} = require('sawtooth-sdk/processor/exceptions')

const crypto = require('crypto');
const cbor = require('cbor')
var encoder = new TextEncoder('utf8')
var decoder = new TextDecoder('utf8')



// function to hash data
const _hash = (x) =>
  crypto.createHash('sha512').update(x).digest('hex').toLowerCase()


const FAMILY_NAME = "kvstore";
const NAMESPACE = _hash(FAMILY_NAME).substring(0, 6);



const _decodeCbor = (buffer) =>
  new Promise((resolve, reject) =>
    cbor.decodeFirst(buffer, (err, obj) => (err ? reject(err) : resolve(obj)))
  )

const _toInternalError = (err) => {
  let message = (err.message) ? err.message : err
  throw new InternalError(message)
}


const _setEntry = (context, address, stateValue) => {
  let entries = {
    [address]: cbor.encode(stateValue)
  }
  return context.setState(entries)
}

const _applySet = (context, address, name, value) => (possibleAddressValues) => {
  let stateValueRep = possibleAddressValues[address]

  let stateValue
  if (stateValueRep && stateValueRep.length > 0) {
    stateValue = cbor.decodeFirstSync(stateValueRep)

  }

  // 'set' passes checks so store it in the state
  if (!stateValue) {
    stateValue = {}
  }

  stateValue[name] = value

  return _setEntry(context, address, stateValue)
}




//transaction handler class
class KvstoreHandler extends TransactionHandler {
    constructor() {
        super(FAMILY_NAME, ['1.0'], [NAMESPACE]);
    }

    //apply function - execution starts here when a transaction reaches the validator for this transaction proceesssor
      apply (transactionProcessRequest, context) {
    return _decodeCbor(transactionProcessRequest.payload)
      .catch(_toInternalError)
      .then((update) => {
        //
        // Validate the update
        let name = update.Name
        if (!name) {
          throw new InvalidTransaction('Name is required')
        }

        let verb = update.Verb
        if (!verb) {
          throw new InvalidTransaction('Verb is required')
        }

        let value = update.Value
        if (value === null || value === undefined) {
          throw new InvalidTransaction('Value is required')
        }


        // Determine the action to apply based on the verb
        let actionFn
        if (verb === 'set') {
          actionFn = _applySet

        } else {
          throw new InvalidTransaction(`Verb must be set not ${verb}`)
        }

        let address = NAMESPACE + _hash(name).slice(-64)

        // Get the current state, for the key's address:
        let getPromise = context.getState([address])

        // Apply the action to the promise's result:
        let actionPromise = getPromise.then(
          actionFn(context, address, name, value)
        )

        // Validate that the action promise results in the correctly set address:
        return actionPromise.then(addresses => {
          if (addresses.length === 0) {
            throw new InternalError('State Error!')
          }
          console.log(`Verb: ${verb} Name: ${name} Value: ${value}`)
        })
      })
  }
}
module.exports = KvstoreHandler;