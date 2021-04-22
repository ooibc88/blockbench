FROM ubuntu:xenial
RUN apt-get update && apt-get install -y wget software-properties-common && rm -rf /var/lib/apt/lists/*
WORKDIR "/root"
RUN add-apt-repository -y ppa:ethereum/ethereum
ARG binary
RUN apt-get update && apt-get install -y ethereum
ENV key=""
CMD exec bootnode -nodekeyhex $key -verbosity 4
EXPOSE 30301/udp
EXPOSE 30303/udp