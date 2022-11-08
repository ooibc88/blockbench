#!/bin/bash
# installing quorum
git clone https://github.com/jpmorganchase/quorum.git
cd quorum
make all

#Binaries are placed in $REPO_ROOT/build/bin. Put that folder in your PATH to make geth and bootnode easily invokable, or copy those binaries to a folder already in PATH, e.g. /usr/local/bin.
#An easy way to supplement PATH is to add PATH=$PATH:/path/to/repository/build/bin to your ~/.bashrc or ~/.bash_aliases file.
#[quorum-Getting-Set-Up]https://github.com/jpmorganchase/quorum/wiki/Getting-Set-Up