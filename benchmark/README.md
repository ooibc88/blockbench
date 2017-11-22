# BlockBench Deployment Scripts

## Setup
Run the `install.sh` script inside `ethereum` , `hyperledger` and `quorum` directory to install `geth` and
hyperledger-fabric respectively. Both need `sudo` privilege.
More details in [ethereum](ethereum/README.md), [hyperledger](hyperledger/README.md) , [parity](pairty/README.md) , [quorum_raft](quorum_raft/README.md) and [quorum_vote](quorum_vote/README.md) directories

## Source structure
+ Smart contract sources are in `contracts` directory
+ Instructions and scripts to run benchmarks for Ethereum, Hyperledger , Parity and Quorum are in [ethereum](ethereum),
[hyperledger](hyperledger) , [parity](pairty) , [quorum_raft](quorum_raft) and [quorum_vote](quorum_vote) directories respectively

## Others
+ Notes on implementation and deployment:
    + [Smart contracts in Ethereum & Parity & quorum](ethereum/contracts.md)
    + [Chaincode in Hyperledger](hyperledger/contracts.md)
