CHANNEL_NAME=rpcchannel
ORDER_ADDR=localhost:7041
PEER_ADDRS=( localhost:7051 localhost:6051 localhost:8051 localhost:9051) # Place anchor peer at head

cwd=$(pwd)

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR # Enter into the script directory

if [ -z ${GOPATH+x} ]; then 
  echo "GOPATH is unset. Pls install golang..."; 
fi

export FABRIC_CFG_PATH=.
export CORE_PEER_LOCALMSPID=Org1MSP 
export CORE_PEER_MSPCONFIGPATH=./crypto_config/peerOrganizations/org1.example.com/users/Admin@org1.example.com/msp 

# Create channel
peer channel create -o $ORDER_ADDR -c $CHANNEL_NAME -f ./channel_artifacts/channel.tx  >&log.txt
if [ $? -eq 0 ]
then
  echo "Successfully create channel ${CHANNEL_NAME}"
else
  echo "Fail to create channel ${CHANNEL_NAME}"
  exit 1
fi

# Join Channel for each peer
for peer_addr in "${PEER_ADDRS[@]}"
do
  # echo "join channel to peer at ${peer_addr}"
  export CORE_PEER_ADDRESS=${peer_addr} 
  peer channel join -b ./${CHANNEL_NAME}.block  >&log.txt
  if [ $? -eq 0 ]
  then
    echo "Successfully join channel ${CHANNEL_NAME} on peer ${peer_addr}"
  else
    echo "Fail to join channel ${CHANNEL_NAME} on peer ${peer_addr}"
    exit 1
  fi
done
