export CORE_PEER_LOCALMSPID=Org1MSP 
export CORE_PEER_MSPCONFIGPATH=../../fabric-v1.4/four-nodes-docker/crypto_config/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp
export FABRIC_CFG_PATH=../../../../benchmark/fabric-v1.4/four-nodes-docker

CHANNEL_NAME=rpcchannel
ORDER_ADDR=localhost:7041
PEER_ADDRS=( localhost:7051 localhost:6051 localhost:8051 localhost:9051) # Place anchor peer at head

LANGUAGE=golang
CC_VERSION=v1

