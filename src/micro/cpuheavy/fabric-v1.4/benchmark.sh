if [ "$#" -ne 1 ]; then
    echo "Illegal number of parameters"
    echo "Should be ./benchmark.sh <# of elements to sort>"
    exit 1
fi

cwd=$(pwd)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd $DIR # Enter into the script directory

. env.sh

export CORE_PEER_ADDRESS=${PEER_ADDRS[0]} 
time peer chaincode query -C $CHANNEL_NAME -n ${CC_NAME} -c "{\"Args\":[\"sort\",\"$1\"]}" >&log.txt

cd $cwd