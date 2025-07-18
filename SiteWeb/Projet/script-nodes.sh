#!/bin/bash

set -e  # Exit on any error
set -x  # Debug mode (print each command)

OUTPUT="/tmp/labels.txt"

echo "Fetching labels for all Kubernetes nodes..." > "$OUTPUT"
kubectl get nodes >> "$OUTPUT"

echo -e "\n--- Node Labels ---" >> "$OUTPUT"
for NODE in $(kubectl get nodes -o name | cut -d'/' -f2); do
  echo -e "\n$NODE:" >> "$OUTPUT"
  kubectl get node "$NODE" --show-labels >> "$OUTPUT"
done

echo "Done writing labels."
sleep 500