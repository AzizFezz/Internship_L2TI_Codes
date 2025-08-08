#!/bin/bash

echo "=== Control-plane démarre ==="

# Noms DNS des services
MACHINE1_HOST="sla-project-machine-1-service"
MACHINE2_HOST="sla-project-machine-2-service"
PORT=8080

echo "Machine-1 Host: $MACHINE1_HOST"
echo "Machine-2 Host: $MACHINE2_HOST"

while true
do
  # Générer un nombre aléatoire
  RANDOM_NUMBER=$((RANDOM % 100))

  # Envoyer le nombre à machine-1
  echo "Sending $RANDOM_NUMBER to machine-1 ($MACHINE1_HOST:$PORT)"
  echo "$RANDOM_NUMBER" | nc $MACHINE1_HOST $PORT

  # Envoyer le nombre à machine-2
  echo "Sending $RANDOM_NUMBER to machine-2 ($MACHINE2_HOST:$PORT)"
  echo "$RANDOM_NUMBER" | nc $MACHINE2_HOST $PORT

  # Attendre quelques secondes
  sleep 2
done
