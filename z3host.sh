#!/bin/bash

container_id=$(docker ps -aqf "name=z3host")


Help() {
   	# Display Help
	echo "Usage: ./z3host.sh [options]"
	echo
	echo "options:"
	echo "run         Run z3host image"
	echo "logs        Display logs of z3Host image"
	echo "attach      Attach to z3Host image, (usefull to write in z3Host CLI, !!WARNING!! will restart z3GatewayHost on exit)"
	echo "-h, --help  Display help"
	echo
}
function main() {
    case "$1" in
        "run")
          docker stop $container_id
          docker rm $container_id
          TTY=$(cat .env | grep DEVICE_TTY= | cut -d '=' -f2)
          docker run --restart=always -d -it --device=$TTY --env-file .env --network="host" --name="z3host"	z3host
        ;;
        "logs")
          docker logs -f $container_id
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
