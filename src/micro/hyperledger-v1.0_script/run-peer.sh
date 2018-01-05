export CORE_VM_ENDPOINT=unix:///var/run/docker.sock
export CORE_LOGGING_PEER=debug
export CORE_PEER_ID=peer0.org1.example.com
export CORE_PEER_LOCALMSPID=Org1MSP
export CORE_PEER_MSPCONFIGPATH=${GOPATH}/src/github.com/hyperledger/fabric/build/bin/network/crypto-config/peerOrganizations/org1.example.com/peers/peer0.org1.example.com/msp
export CORE_PEER_ADDRESS=localhost:7051
export CORE_PEER_FILESYSTEMPATH=~/data/hyperledger/production
#export CORE_VM_DOCKER_HOSTCONFIG_NETWORKMODE=network_default
#export CORE_LEDGER_STATE_STATEDATABASE=UStore
export CORE_LEDGER_STATE_STATEDATABASE=goleveldb
#export CORE_CHAINCODE_MODE=dev
export CORE_CHAINCODE_LOGGING_LEVEL=DEBUG
export CORE_CHAINCODE_EXECUTETIMEOUT=300000s

${GOPATH}/src/github.com/hyperledger/fabric/build/bin/peer node start
