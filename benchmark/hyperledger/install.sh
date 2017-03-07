#!/bin/bash
# installing hyperledger-fabric with docker 

cd `dirname ${BASH_SOURCE-$0}`
. env.sh

sudo cp docker /etc/default/
sudo service docker restart
sudo apt-get install -y libsnappy-dev zlib1g-dev libbz2-dev
cd $HL_DATA
wget https://storage.googleapis.com/golang/go1.7.3.linux-amd64.tar.gz
tar -zxvf go1.7.3.linux-amd64.tar.gz

git clone https://github.com/facebook/rocksdb.git
cd rocksdb
git checkout v4.1
PORTABLE=1 make shared_lib
sudo INSTALL_PATH=/usr/local make install-shared

cd $HL_DATA
mkdir -p src/github.com/hyperledger
cd src/github.com/hyperledger
git clone https://github.com/hyperledger/fabric
cd fabric
git checkout e728c5c22160620e189e85be6becb7cbf75d87dc
cp $HL_HOME/hl_core.yaml peer/core.yaml
make peer
