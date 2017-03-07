# BlockBench Deployment Scripts

## Setup
Run the `install.sh` script inside `ethereum` and `hyperledger` directory to install `geth` and
hyperledger-fabric respectively. Both needs `sudo` privilege.

## Source structure
+ Smart contract sources are in `contracts` directory
+ Instructions and scripts to run benchmarks for Ethereum, Hyperledger and Pairty are in [ethereum](ethereum),
[hyperledger](hyperledger) and [parity](pairty) directories respectively

## Others
+ Notes on implementing and deploying:
    + [Smart contracts in Ethereum](ethereum/contracts.md)
    + [Chaincode in Hyperledger](hyperledger/contracts.md)

+ Benchmarking results and post-processing scripts can be found in [results](results) directory
