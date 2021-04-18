FROM ubuntu:xenial
RUN apt-get update && apt-get install -y wget software-properties-common && rm -rf /var/lib/apt/lists/*
WORKDIR "/root"
RUN add-apt-repository -y ppa:ethereum/ethereum
RUN apt-get update && apt-get install -y ethereum
ARG pwd
ARG pk
ADD ./application.json ./application.json
RUN geth --datadir ~/backup init ./application.json
RUN echo $pwd > `mktemp /tmp/.pwd.XXX`
RUN echo $pk > `mktemp /tmp/.pk.XXX`
RUN geth --datadir ~/backup account import --password /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pwd` /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pk`
ENV addr=""
ENV id=""
ENV ip=""
CMD exec geth --bootnodes "enode://$id@$ip:30301" --datadir ~/backup --networkid 6085214 --verbosity=3 --syncmode=full --mine --miner.etherbase $addr --unlock $addr --password /tmp/`ls -al /tmp/ | awk '{print $9}' | grep .pwd`

EXPOSE 30303