
 #!/bin/bash

  helpFunction()
  {
     echo ""
     echo "Usage: $0 -p peerPort -c CHANNEL_NAME -d chaincode"
     exit 1
  }

  while getopts "t:c:d:p:o:" opt
  do
     case "$opt" in
        t ) PORT="$OPTARG" ;;
        c ) CHANNEL_NAME="$OPTARG" ;;
        d ) CHAINCODE="$OPTARG" ;;
        p ) peerN="$OPTARG" ;;
        o ) orgN="$OPTARG" ;;
        ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
     esac
  done

  if [ -z "$PORT" ] || [ -z "$CHANNEL_NAME" ] || [ -z "$CHAINCODE" ] || [ -z "$peerN" ] || [ -z "$orgN" ]
  then
    echo $PORT $CHANNEL_NAME $CHAINCODE
     echo "Some or all of the parameters are empty";
     helpFunction
  fi

PEER="peer${peerN}"
ORG="org${orgN}"

 export CORE_PEER_TLS_ENABLED=true
 export CORE_PEER_LOCALMSPID="Org${orgN}MSP"
 export CORE_PEER_TLS_ROOTCERT_FILE=${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/peer0.${ORG}.example.com/tls/ca.crt
 export CORE_PEER_MSPCONFIGPATH=${PWD}/organizations/peerOrganizations/${ORG}.example.com/users/Admin@${ORG}.example.com/msp
 export CORE_PEER_ADDRESS=localhost:7051
 export ORDERER_CA=${PWD}/organizations/ordererOrganizations/example.com/orderers/orderer.example.com/msp/tlscacerts/tlsca.example.com-cert.pem


 echo
 echo "join peer to channel ${CHANNEL_NAME}"
 echo
 CORE_PEER_ADDRESS=localhost:${PORT} peer channel join -b channel-artifacts/${CHANNEL_NAME}.block
 CORE_PEER_ADDRESS=localhost:${PORT} peer channel list
sleep 10
 echo "install chaincode"
 CORE_PEER_ADDRESS=localhost:${PORT} peer lifecycle chaincode install ${CHAINCODE}.tar.gz

#peer chaincode invoke -o localhost:7050 --ordererTLSHostnameOverride orderer.example.com --tls true --cafile $ORDERER_CA -C ${CHANNEL_NAME} -n ${CHAINCODE} --peerAddresses localhost:${PORT} --tlsRootCertFiles ${PWD}/organizations/peerOrganizations/org1.example.com/peers/peer1.org1.example.com/tls/ca.crt --peerAddresses localhost:9051 --tlsRootCertFiles ${PWD}/organizations/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt -c '{"Args":["Write","name","Bob"]}'
#peer chaincode query -C ${CHANNEL_NAME -n ${CHAINCODE -c '{"Args":["Read","name"]}'
