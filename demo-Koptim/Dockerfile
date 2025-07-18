# Étape 1 : build dans Debian Bullseye (glibc 2.31)
FROM debian:bullseye as builder

RUN apt-get update && apt-get install -y build-essential g++

WORKDIR /app
COPY main.cpp .

RUN g++ -std=c++17 -O2 -static -o app main.cpp

# Étape 2 : image finale
FROM debian:bullseye-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    curl procps ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Installer kubectl
RUN curl -LO "https://dl.k8s.io/release/$(curl -L -s https://dl.k8s.io/release/stable.txt)/bin/linux/amd64/kubectl" \
    && install -o root -g root -m 0755 kubectl /usr/local/bin/kubectl \
    && rm kubectl

COPY --from=builder /app/app /app/app

WORKDIR /app

ENV KUBECONFIG=/root/.kube/config

CMD ["/app/app"]

