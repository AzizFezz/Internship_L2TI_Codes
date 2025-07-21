#!/bin/bash

echo "Starting infinite TCP server on port 8080..."

while true; do
  # Accept connection, read one line, print it, then close
  echo "Waiting for connection..."
  echo "Received:"
  nc -l -p 8080 
done
