FROM bitnami/kubectl:latest

# Copier le script dans un dossier accessible
COPY script.sh /tmp/script.sh

# Éviter l’ENTRYPOINT de kubectl
ENTRYPOINT []

# Lancer le script avec bash
CMD ["bash", "/tmp/script.sh"]
