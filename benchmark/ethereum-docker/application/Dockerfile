FROM ubuntu:xenial
RUN apt-get update && apt-get install -y wget software-properties-common && rm -rf /var/lib/apt/lists/*
WORKDIR "/root"
RUN add-apt-repository -y ppa:ethereum/ethereum
RUN apt-get update && apt-get install -y ethereum
ARG pwd
ARG pk
ADD ./application.json ./application.json
RUN geth --datadir ~/application init ./application.json
RUN echo $pwd > `mktemp /tmp/.pwd.XXX`
RUN echo $pk > `mktemp /tmp/.pk.XXX`
RUN geth --datadir ~/application account import --password /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pwd` /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pk`
ENV addr=""
ENV id=""
ENV ip=""
CMD exec geth --bootnodes "enode://$id@$ip:30301" --datadir ~/application --networkid 6085214 --verbosity=4 --rpc --rpcaddr "0.0.0.0" --rpcport "59767" --rpcapi "eth,web3,net,admin,debug,db" --rpccorsdomain "*" --ws --ws.addr "0.0.0.0" --ws.port "63343" --ws.api "eth,web3,net,admin,debug,db,personal" --ws.origins "*" --syncmode=full  --mine  -miner.threads=1 --miner.etherbase $addr --unlock $addr --password /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pwd` --allow-insecure-unlock
EXPOSE 59767
EXPOSE 63343
EXPOSE 30303