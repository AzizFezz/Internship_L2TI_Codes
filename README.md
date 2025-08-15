# 📦 Système de surveillance et de gestion automatisée des ressources Kubernetes
## 📖 Présentation générale

Ce dépôt regroupe huit projets successifs qui illustrent l’évolution d’un système complet de surveillance, analyse et réponse automatique aux conditions de charge CPU dans un cluster Kubernetes.

Chaque projet correspond à une étape clé dans notre réflexion et nos expérimentations :

- D’abord, mettre en place la communication entre pods.
- Ensuite, améliorer l’affichage et la collecte d’informations.
- Puis, développer des modules capables de détecter des conditions (métriques CPU, labels Kubernetes) et d’agir automatiquement sur les ressources du cluster.

L’objectif final est de disposer d’un système léger, modulaire et automatisé, capable de :

- Surveiller les pods
- Analyser les métriques (CPU, mémoire)
- Appliquer automatiquement des labels et ajuster les ressources
- Faciliter le scaling vertical et horizontal

---

# 🗂 Structure du dépôt et fil conducteur

**0-netcat-nombres-aleatoires**

    - Test initial pour comprendre comment deux pods peuvent communiquer via Netcat.
    - Limité car la connexion se ferme après chaque envoi.
**1-client-serveur-nombres-aleatoires**

    - Première architecture client-serveur pour échanges continus.
**2-app-flask-nombres-aleatoires**

    - Passage à Flask pour afficher en temps réel les nombres sur le worker.
    - Communication stable et affichage dynamique.
**3-script-extraction-labels-bash**

    - Script Bash pour extraire tous les labels de tous les pods dans tous les namespaces.
    - Servira plus tard pour détecter les pods à gérer.
**4-gestionnaire-charge-vpa**

    - Découverte de Vertical Pod Autoscaler (VPA) : scaling vertical automatique basé sur les métriques CPU.
**5-demo-gestionnaire-koptim**

    - Module en C++ analysant les pods avec fogsla=ok et réajustant CPU/mémoire.
    - Première version de notre optimiseur de ressources.
**6-analyseur-webhook-prometheus**

    - Intégration avec Prometheus et Alertmanager via un webhook en C++.
    - Attribution de labels automatiques selon l’utilisation CPU.
**7-gestionnaire-alertes-cpu-latest**

    - Version optimisée et finale en Flask (plus légère et fluide).
    - Applique automatiquement des labels aux pods selon leur CPU, utilisable sur n’importe quel pod du cluster.

---
## ⚙️ Environnement requis

Ce projet peut être exécuté sur toute machine Ubuntu disposant de :
- Python 3 et Flask
- Docker
- Minikube
- Kubectl

---
## 📥 Installation des prérequis

```bash
# Mettre à jour la machine
sudo apt update && sudo apt upgrade -y

# Installer Python et Flask
sudo apt install python3 python3-pip -y
pip3 install flask

# Installer Docker
sudo apt install docker.io -y
sudo usermod -aG docker $USER
newgrp docker

# Installer Minikube
curl -LO https://storage.googleapis.com/minikube/releases/latest/minikube-linux-amd64
sudo install minikube-linux-amd64 /usr/local/bin/minikube

# Installer Kubectl
sudo snap install kubectl --classic

# Lancer un cluster Kubernetes 3 nœuds avec Docker comme driver et sans vérification VT-x
minikube start --nodes 3 --driver=docker --no-vtx-check
```
