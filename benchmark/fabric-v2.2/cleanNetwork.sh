echo "Stop all containers"
docker stop `docker ps -qa`

echo "Remove all containers"
docker rm `docker ps -qa`

echo "Remove all volumes"
docker volume rm $(docker volume ls -q)

echo "Remove all networks"
docker network rm `docker network ls -q`
