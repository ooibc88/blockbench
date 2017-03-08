# Hyperledger chaincode (smart contracts)
This article explains how we write, deploy, and invoke smart contracts in Hyperledger (also known as
chaincodes). Hyperledger supports several high-level languages, the most popular being Go and Java. In
contrast, Ethereum smart contracts must be compiled to EVM.   

## Writing chaincode
+ Hyperledger comes with a set of examples written in both Java and Go. They are in
`$HL_SOURCE/fabric/examples/chaincode/java` and `$HL_SOURCE/fabric/examples/chaincode/go`

+ To write a new chaincode in Java (similarly in Go), make a copy of an example directory in
`$HL_SOURCE/fabric/examples/java`.
    + Keep the same directory structure (it is important for Java compilation)
    + For java, change `build.gradle` file to reflect the correct class name. 

+ For the benchmark, the chaincode implementations are in `contracts/hyperledger/go` and
`contracts/hyperledger/java`.

## Deploying the contract
Like Ethereum, the chaincode is deployed by sending a special (deploy) transaction to the blockchain. But
first, one must ensure that it compiles successfully.  

### Java compilation
+ Java chaincode is built via `gradle`. Make sure the following is met:
    + `gradle >= 2.12`: in Ubuntu, this can be done via `apt-get` as follows:

        ```
        sudo add-apt-repository ppa:cwchien/gradle
        sudo apt-get update
        sudo apt-get install gradle-ppa
        ```
    + Java8 is needed, as the code uses lambdas
    + `shim-client-1-0.jar` is built successfully. For this:

        ```
        cd $HL_SOURCE/fabric/code/chaincode/shim/java
        gradle -b build.gradle clean
        gradle -b build.gradle build
        ```
    If these steps are unsuccessful, the chaincode is not gonna work. 

+ Build with `gradle`:
    + Make sure the directory structure is correct, i.e. it must have a `build.gradle` file. *Best is to copy
    the directory to `$HL_SOURCE/fabric/examples/chaincode/java/`
    + Go to the chaincode directory and build it:

        `gradle -b build.gradle build`

### Go compilation
+ Make sure `$GOPATH` and `$GOROOT` are set up correctly. 
+ Go the chaincode directory and build it:

    `go build`


### Deployment

All the following methods return a chaincodeID, a unique hash for identifying the chaincode. It is basically a
hash of:

+ The source code
+ Init arguments passed to constructor
+ The path used during deployment. Thus, the same source code with the same init arguments but deployed
using two different paths are considered as different.   

**vs. Ethereum contracts.** Unlike [Ethereum contracts](../ethereum/contracts.md), a chaincodeID is not bound
to identities of the deploying peer (unless the deployment path is bound to the peer identity). Thus, when the
same contract on a NFS partition is deployed multiple times by different nodes, the same chaincodeID is
returned (and effectively, only one chaincode is running). 

### Local deployment 
A chaincode can be deploy from the *peer's filesystem*, for example at
`/data/dinhtta/chaincode/test`. In a cluster, this can be on the peer's NFS directory.  

+ One way to do it is with `$HL_SOURCE/build/bin/peer`, as follows:

    `$HL_SOURCE/build/bin/peer chaincode deploy [-l <language>] -p <path> -c '{"function": "<func name>",
    "args":[<arguments>]'`

  By default, the chaincode is written in Go. For Java chaincode, use `-l java`. 

  **IMPORTANT** For Go chaincode, `<path>` **MUST be relative to `$GOPATH/src`** (absolute paths do not work
  in my experience). For instance, if `$GOPATH=/data/dinhtta`, then one must first do:

      `cp -r /data/dinhtta/chaincode/test $GOPATH/src/`
  
  Having done this, `<path>` is simply `test`. 

  For Java chaincode, `<path>` should be an absolute path. For instance, `/data/dinhtta/chaincode/test`

+ Another way to do it is to send a REST request to the peer's chaincode RPC endpoint `<host>:7050/chaincode`.
In particular:

    `curl -X POST -d '{"jsonrpc"..., "type": <language type>, .. "path":<path>,...}' <host>:7050/chaincode`

where `<path>` is the same as above, and <language type> is an integer (1 for Golang, 4 for Java). 


### Github deployment
A chaincode can be uploaded to a Github repository, and the URL specified as the chaincode path. As the
chaincode path is the same, multiple deployments will result in only one chaincode running (unless init
arguments are different).

**IMPORANT.** As of now, this method works only for Go chaincode. There's a problem with Git communication for
Java chaincode implementation (Git cloning exits with error code 128). 

1. Create a repository, say `<username>/chaincode-dev` on Github. 
2. Add the chaincode directory, say `test` to it. 
3. Specify the following path name when deploying (either using CLI or REST interface as above):

    `https://github.com/<username>/chaincode-dev/test`

  (Viewed in a browser, this path will result in a 404 error. But this is OK, for Hyperledger parses this URL
  differently). 


## Invoking contract methods
Once the contract is deployed, its method is invoked by sending a `invoke` transaction to it via RPC. Compared
to Ethereum, it is vastly simpler, because *no encoding is needed*. 

Hyperledger programming model takes string inputs and relies on the contract logic to parse them. This way, a
method invocation simply consists of a method name and a list of string as arguments. Suppose the contract
implements a method `set(string[] args)`.  Invoking this method with two strings "hello" and "world" is done
as follows: 

```
curl -X POST -d \
'{\
 "jsonrpc": "2.0",\
 "method": "invoke",\
 "params": {\
   "type": "1",\
   "chaincodeID": "<chaincodeId>",\
   "ctorMsg": {\
     "function": "set",
     "args": ["hello", "world"]
   }\
 },\
 "id": "1"\
}'\
<host>:7050/chaincode
```
