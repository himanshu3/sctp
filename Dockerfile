FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential \
    libsctp-dev

COPY sctp_server.c /sctp_server.c

RUN gcc sctp_server.c -o sctp_server -lsctp

ENTRYPOINT ["./sctp_server"]