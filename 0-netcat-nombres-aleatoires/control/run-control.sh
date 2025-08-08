#!/bin/bash

MACHINE1="sla-project-machine-1-service" # machine 1 
MACHINE2="sla-project-machine-2-service" # machine 2


while true; do
  RAND=$((RANDOM % 1000))
  echo "Generated number: $RAND"
  echo $RAND | nc $MACHINE1 8080
  echo $RAND | nc $MACHINE2 8080
  sleep 10
done
