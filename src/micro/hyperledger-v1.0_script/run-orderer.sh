export ORDERER_GENERAL_LOGLEVEL=debug
export ORDERER_GENERAL_LISTENADDRESS=0.0.0.0
export ORDERER_GENERAL_GENESISMETHOD=file
export ORDERER_GENERAL_GENESISFILE=${GOPATH}/src/github.com/hyperledger/fabric/build/bin/network/genesis.block
export ORDERER_GENERAL_LOCALMSPID=OrdererMSP
export ORDERER_GENERAL_LOCALMSPDIR=${GOPATH}/src/github.com/hyperledger/fabric/build/bin/network/crypto-config/ordererOrganizations/example.com/orderers/orderer.example.com/msp
export ORDERER_FILELEDGER_LOCATION=~/data/hyperledger/production/orderer

rm -rf ~/data/hyperledger/production/
${GOPATH}/src/github.com/hyperledger/fabric/build/bin/orderer

