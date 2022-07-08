#!/bin/bash

function main() {
    case "$1" in
        "generate")
            docker build -t test-z3 -f etc/Dockerfile .
            docker run -it --rm --name TTTT -v $(pwd)/src:/app/src  -v $(pwd)/output:/app/output test-z3
            ;;
    esac
}

main "$@"
