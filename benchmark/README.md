# BlockBench Deployment Scripts

## Setup
Run the `install.sh` script inside `ethereum` and `hyperledger` directory to install `geth` and
hyperledger-fabric respectively. Both need `sudo` privilege.

## Source structure
+ Smart contract sources are in `contracts` directory
+ Instructions and scripts to run benchmarks for Ethereum, Hyperledger and Parity are in [ethereum](ethereum),
[hyperledger](hyperledger) and [parity](pairty) directories respectively

## Others
+ Notes on implementation and deployment:
    + [Smart contracts in Ethereum & Parity](ethereum/contracts.md)
    + [Chaincode in Hyperledger](hyperledger/contracts.md)
