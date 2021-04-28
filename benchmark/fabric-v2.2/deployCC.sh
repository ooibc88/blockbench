 echo
 echo "join peer to channel"
 echo
 CORE_PEER_ADDRESS=localhost:8051 peer channel join -b channel-artifacts/mychannel.block
 CORE_PEER_ADDRESS=localhost:8051 peer channel list
sleep 10
 echo "install chaincode"
 CORE_PEER_ADDRESS=localhost:8051 peer lifecycle chaincode install kvstore.tar.gz

#peer chaincode invoke -o localhost:7050 --ordererTLSHostnameOverride orderer.example.com --tls true --cafile $ORDERER_CA -C mychannel -n kvstore --peerAddresses localhost:8051 --tlsRootCertFiles ${PWD}/organizations/peerOrganizations/org1.example.com/peers/peer1.org1.example.com/tls/ca.crt --peerAddresses localhost:9051 --tlsRootCertFiles ${PWD}/organizations/peerOrganizations/org2.example.com/peers/peer0.org2.example.com/tls/ca.crt -c '{"Args":["Write","name","Bob"]}'
#peer chaincode query -C mychannel -n kvstore -c '{"Args":["Read","name"]}'
