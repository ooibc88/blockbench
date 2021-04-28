#!/bin/bash

helpFunction()
{
   echo ""
   echo "Usage: $0 -o organisationNumber -p peerNumber -t caPort"
   exit 1 # Exit script after printing help
}

while getopts "o:p:t:" opt
do
   case "$opt" in
      p ) peerN="$OPTARG" ;;
      o ) orgN="$OPTARG" ;;
      t ) CA_PORT="$OPTARG" ;;
      ? ) helpFunction ;; # Print helpFunction in case parameter is non-existent
   esac
done

if [ -z "$peerN" ] || [ -z "$orgN" ] || [ -z "$CA_PORT" ]
then
   echo "Some or all of the parameters are empty";
   helpFunction
fi

PEER="peer${peerN}"
ORG="org${orgN}"

 echo
 echo "adding peer$peerN in organisation $orgN"
 echo

 export CORE_PEER_TLS_ENABLED=true
 export CORE_PEER_LOCALMSPID="Org${orgN}MSP"
 export CORE_PEER_TLS_ROOTCERT_FILE=${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/peer0.${ORG}.example.com/tls/ca.crt
 export CORE_PEER_MSPCONFIGPATH=${PWD}/organizations/peerOrganizations/${ORG}.example.com/users/Admin@${ORG}.example.com/msp
 export ORDERER_CA=${PWD}/organizations/ordererOrganizations/example.com/orderers/orderer.example.com/msp/tlscacerts/tlsca.example.com-cert.pem

 export FABRIC_CA_CLIENT_HOME=${PWD}/organizations/peerOrganizations/${ORG}.example.com/
 sleep 1
 echo
 echo "Register ${PEER}"
 echo

 fabric-ca-client register --caname ca-${ORG} --id.name ${PEER} --id.secret ${PEER}pw --id.type peer --tls.certfiles ${PWD}/organizations/fabric-ca/${ORG}/tls-cert.pem
 sleep 1
 mkdir -p organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com
 echo
 echo "Generate the ${PEER} msp"
 echo
 fabric-ca-client enroll -u https://${PEER}:${PEER}pw@localhost:${CA_PORT} --caname ca-${ORG} -M ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/msp --csr.hosts ${PEER}.${ORG}.example.com --tls.certfiles ${PWD}/organizations/fabric-ca/${ORG}/tls-cert.pem
 sleep 1
 cp ${PWD}/organizations/peerOrganizations/${ORG}.example.com/msp/config.yaml ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/msp/config.yaml

 echo
 echo "Generate the ${PEER}-tls certificates"
 echo
 sleep 1
 fabric-ca-client enroll -u https://${PEER}:${PEER}pw@localhost:${CA_PORT} --caname ca-${ORG} -M ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls --enrollment.profile tls --csr.hosts ${PEER}.${ORG}.example.com --csr.hosts localhost --tls.certfiles ${PWD}/organizations/fabric-ca/${ORG}/tls-cert.pem
 sleep 1
 cp ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/tlscacerts/* ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/ca.crt
 cp ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/signcerts/* ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/server.crt
 cp ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/keystore/* ${PWD}/organizations/peerOrganizations/${ORG}.example.com/peers/${PEER}.${ORG}.example.com/tls/server.key
 sleep 3
 echo
 echo "creating container ${PEER}"
 echo
 IMAGE_TAG=2.2 docker-compose -f docker/docker-compose-${PEER}${ORG}.yaml up -d
