#!/bin/bash

container_id=$(docker ps -aqf "name=z3host")


Help() {
   	# Display Help
	echo "Usage: ./manage.sh [options]"
	echo
	echo "options:"
	echo "build       Build z3host image"
	echo "run         Run docker image"
	echo "logs        Display logs of z3Host image"
	echo "exec        Docker exec to z3Host image"
	echo "attach      Attach to z3Host image, (usefull to write in z3Host CLI, !!WARNING!! will restart z3GatewayHost on exit)"
	echo "-h, --help  Display help"
	echo
}
function main() {
    case "$1" in
        "generate")
          docker build -t test-z3 -f etc/Dockerfile .
          docker run -it --rm --name TTTT -v $(pwd)/src:/app/src  -v $(pwd)/output:/app/output test-z3
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
