#!/bin/bash

container_id=$(docker ps -aqf "name=z3host")


Help() {
   	# Display Help
	echo "Usage: ./manage.sh [options]"
	echo
	echo "options:"
	echo "build       Build z3host image"
	echo "run         Run standalone z3host docker image"
	echo "logs        Display logs of standalone z3Host image"
	echo "exec        Docker exec to standalone z3Host image"
	echo "attach      Attach to standalone z3Host image, (usefull to write in z3Host CLI, !!WARNING!! will restart z3GatewayHost on exit)"
	echo "-h, --help  Display help"
	echo
}
function main() {
    case "$1" in
        "generate")
          docker build -t z3builder -f etc/Dockerfile .
          TTY=$(cat .env | grep DEVICE_TTY= | cut -d '=' -f2)
	  echo $TTY
          docker run -it --rm  --device=$TTY --env-file .env --name z3builder -v $(pwd)/src:/app/src  -v $(pwd)/output:/app/output z3builder
          ;;
        "build")
          docker build -t z3host -f etc/z3Host-standalone.dockerfile .
          ;;
        "run")
          docker stop $container_id
          docker rm $container_id
          TTY=$(cat .env | grep DEVICE_TTY= | cut -d '=' -f2)
          docker run --restart=always -d -it --device=$TTY --env-file .env --network="host" --name="z3host"	z3host
        ;;
        "logs")
          docker logs -f $container_id
          ;;
        "exec")
          docker exec -it $container_id sh
          ;;
        "attach")
          docker container attach $container_id
          ;;
        "-h")
          Help
          ;;
        "--help")
          Help
          ;;
    esac
}

main "$@"
