# BlockBench

BlockBench is the first benchmarking framework for private blockchain systems.
It serves as a fair means of comparison for different platforms and enables deeper understanding
of different system design choices.

BlockBench provides both [Macro-benchmark](src/macro) for overall performance and [Micro-benchmark](src/micro)
for individual layers.

## Workloads Provided

### Macro-benchmark

* YCSB (KVStore)
* SmallBank (OLTP)

### Micro-benchmark

* DoNothing (consensus layer)
* IOHeavy (data model layer, read/write oriented)
* Analytics (data model layer, analytical query oriented)
* CPUHeavy (execution layer)

## Source file structure

+ Smart contract sources are in [benchmark/contracts](benchmark/contracts) directory
+ Instructions and scripts to run benchmarks for Ethereum, Hyperledger and Pairty are in [ethereum](benchmark/ethereum),
[hyperledger](benchmark/hyperledger) and [parity](benchmark/pairty) directories respectively.
+ Drivers for benchmark workloads are in [src](src) directory.

## Dependency

### C++ libraries
* [restclient-cpp](https://github.com/mrtazz/restclient-cpp)
* [libcurl](https://curl.haxx.se/libcurl/)

### Node.js libraries
* [Web3.js](https://github.com/ethereum/web3.js/)
* [zipfian](https://www.npmjs.com/package/zipfian)
* [bignumber.js](https://www.npmjs.com/package/bignumber.js)
