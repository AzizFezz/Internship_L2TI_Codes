FROM ubuntu:16.04

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential netcat && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN make

CMD ["/app/client"]
