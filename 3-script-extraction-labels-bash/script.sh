#!/bin/bash

set -e
set -x

OUTPUT="/tmp/labels.txt"

echo "--- Pod Labels ---" > "$OUTPUT"

# Récupère tous les namespaces
for NS in $(kubectl get namespaces -o jsonpath='{.items[*].metadata.name}'); do
  echo -e "\nNamespace: $NS" >> "$OUTPUT"
  
  # Récupère tous les pods dans ce namespace
  for POD in $(kubectl get pods -n "$NS" -o jsonpath='{.items[*].metadata.name}'); do
    echo -e "\nPod: $POD" >> "$OUTPUT"
    
    # Récupère les labels du pod
    LABELS=$(kubectl get pod "$POD" -n "$NS" -o jsonpath='{.metadata.labels}')
    
    # S'il y a des labels, les afficher en ligne unique
    if [ "$LABELS" != "" ]; then
      echo "LABELS: $(kubectl get pod "$POD" -n "$NS" -o jsonpath='{.metadata.labels}' | jq -r 'to_entries | map("\(.key)=\(.value)") | join(",")')" >> "$OUTPUT"
    else
      echo "LABELS: <none>" >> "$OUTPUT"
    fi
  done
done

echo "Done writing."
sleep 500
