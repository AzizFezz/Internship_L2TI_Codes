FROM ubuntu:18.04

# Installer les outils de compilation et Python
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    python3 \
    python3-pip \
    bash \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copier les sources
COPY tcp_server.c .
COPY app.py .
COPY start.sh .
COPY templates ./templates

# Compiler le serveur TCP
RUN gcc -o tcp_server tcp_server.c

# Installer Flask
RUN pip3 install flask

# Donner les droits d'exécution au script de lancement
RUN chmod +x start.sh

# Exposer les ports
EXPOSE 8080
EXPOSE 5000

# Lancer le script de démarrage
CMD ["./start.sh"]
