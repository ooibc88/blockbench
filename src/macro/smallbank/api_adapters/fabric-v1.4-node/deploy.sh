
cwd=$(pwd)

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR # Enter into the script directory
. env.sh

echo "CCNAME: " ${CC_NAME}
rm -rf ${GOPATH}/src/${CC_NAME}
cp -r ${CC_SRC_PATH} ${GOPATH}/src/${CC_NAME}

for peer_addr in "${PEER_ADDRS[@]}"
do
    export CORE_PEER_ADDRESS=${peer_addr} 
    peer chaincode install -n ${CC_NAME} -v ${CC_VERSION} -l ${LANGUAGE} -p ${CC_NAME} >&log.txt
    if [ $? -eq 0 ]
    then
        echo "Successfully install chaincode ${CC_NAME} on peer ${peer_addr}"
    else
        echo "Fail to install chaincode ${CC_NAME}"
        exit 1
    fi
done

export CORE_PEER_ADDRESS=${PEER_ADDRS[0]} 


# peer chaincode instantiate -o ${ORDER_ADDR} -C ${CHANNEL_NAME} -c '{"Args":["init"]}' -n ${CC_NAME} -l ${LANGUAGE} -v ${CC_VERSION} -P "AND ('Org1MSP.peer')" >&log.txt
peer chaincode instantiate -o ${ORDER_ADDR} -C ${CHANNEL_NAME} -c '{"Args":["init"]}' -n ${CC_NAME} -l ${LANGUAGE} -v ${CC_VERSION} >&log.txt

if [ $? -eq 0 ]
then
  echo "Successfully instantiate chaincode ${CC_NAME} on anchor peer ${CORE_PEER_ADDRESS}"
else
  echo "Fail to instantiate chaincode ${CC_NAME}"
  exit 1
fi
cd $cwd
