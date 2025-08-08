FROM ubuntu:16.04

RUN apt-get update && \
    apt-get install -y netcat bash && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY run-control.sh .

RUN chmod +x run-control.sh

EXPOSE 8080

CMD ["bash", "run-control.sh"]


