# Prepare
  * Spin up the network, create the channel, deploy the contract and launch the helper web services, as in [README](../../benchmark/fabric-v2.2/README.md) with the following two bash **variables** `CC_NAME=ioheavy MODE=closed_loop`.


# Run
## Batch Writing
```
curl --header "Content-Type: application/json" \
--request POST \
--data '{"function":"Write","args":["<start_key>", "<#_of_records>"]}' \
http://<txn_service_endpoint>/invoke
```

## Scanning
```
curl "http://<txn_service_endpoint>/query?function=Scan&args=<start_key>,<#_of_records>"
```

`<start_key>` and  `<#_of_records>` must be numeric. 
The transaction service will return the execution delay in millisecond. 
