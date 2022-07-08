FROM ubuntu:18.04

WORKDIR /app
RUN apt clean
RUN apt-get update && apt-get -y upgrade
RUN apt install -y libreadline7

RUN mkdir -p /app/bin
COPY ./output /app/bin

COPY ./etc/z3host-standalone-init.sh /app

ENTRYPOINT ["./z3host-standalone-init.sh"]