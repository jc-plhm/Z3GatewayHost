FROM ubuntu:18.04

WORKDIR /app
RUN apt clean
RUN apt update && apt -y upgrade
RUN apt install -y build-essential \
 libreadline7 \
 libncurses5-dev \
 libncursesw5-dev \
 libreadline-dev

RUN mkdir -p /app/src

COPY etc/init.sh /app/
COPY src /app/src/
ENTRYPOINT ["./init.sh" ]
