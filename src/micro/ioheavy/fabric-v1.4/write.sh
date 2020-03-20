if [ "$#" -ne 2 ]; then
    echo "Illegal number of parameters"
    echo "Should be ./write.sh <start-key> <num-of-keys>"
    exit 1
fi

cwd=$(pwd)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR # Enter into the script directory

. env.sh

export CORE_PEER_ADDRESS=${PEER_ADDRS[0]} 
time peer chaincode invoke -o ${ORDER_ADDR} -C $CHANNEL_NAME -n ${CC_NAME} -c "{\"Args\":[\"write\",\"$1\", \"$2\"]}" >&log.txt

cd $cwd