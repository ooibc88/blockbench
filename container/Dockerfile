FROM ubuntu:14.04

# windows workaround to ensure access to internet from within container
RUN echo "nameserver 8.8.8.8" >> /etc/resolv.conf

# basics
RUN \
  apt-get update && apt-get upgrade -q -y && \
  apt-get install -y --no-install-recommends git make gcc libc6-dev curl ca-certificates software-properties-common g++ patch nodejs npm autoconf libtool libtbb-dev automake

# download required cpp dependency
RUN \
  cd /root/ && \
  git clone https://github.com/mrtazz/restclient-cpp && \
  git clone https://github.com/curl/curl.git

# patch restclient-cpp for blockbench parity
COPY ./benchmark/parity/patch_restclient /root/patch_restclient
RUN \
  cd /root/restclient-cpp && patch -p4 < /root/patch_restclient

# install dependencies
RUN \
# first install curl lib since restclient-cpp depends on it
  cd /root/curl && ./buildconf && ./configure && make && make install && \
  cd /root/restclient-cpp && ./autogen.sh && ./configure && make install

ENTRYPOINT ["/bin/bash"]
