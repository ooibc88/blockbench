To build the blockbench_container, change to the parent folder of this directory and run the docker
build command with the -f flag
cd .. && docker build -f container/Dockerfile -t blockbench/blockbench .

Mount the blockbench folder (cd ../../blockbench) to the Docker
container when running the container (the default entry point is bash):
docker run -it -v /Users/myuser/Documents/Projects/blockbench/:/root/blockbench --name=blockbench-dev blockbench/blockbench

Change to directory blockbench/src/macro/kvstore, e.g.:
cd /root/blockbench/src/macro/kvstore

and build the driver by running:
make

Now you can run benchmark as specified in the other readmes
