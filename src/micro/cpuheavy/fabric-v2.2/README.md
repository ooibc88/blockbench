# Prepare
  * Spin up the network, create the channel, deploy the contract and launch the helper web services, as in [README](../../../../benchmark/fabric-v2.2/README.md) with the following two bash **variables** `CC_NAME=sorter MODE=closed_loop`.


# Run
```
curl "http://<txn_service_addr>/query?function=Compute&args=<array_size>"
```

The transaction service will return the execution delay in millisecond. 
