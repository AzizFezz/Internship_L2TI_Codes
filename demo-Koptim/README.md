# 📦 Projet : Surveillance et Répartition Dynamique des Ressources dans Kubernetes

Ce projet consiste à :

- Rechercher tous les **deployments** ayant le **label `fogsla=ok`**
- Calculer la **charge CPU du cluster**
- Adapter dynamiquement les **limites CPU et mémoire** des containers concernés
- Étiqueter le déploiement avec le niveau de charge (`low`, `medium`, `high`)

---

## ⚙️ Pré-requis

Avant de commencer, assure-toi d’avoir :

- Minikube installé et en cours d’exécution
- Docker installé et accessible
- Un cluster Kubernetes fonctionnel via Minikube
- Le fichier `main.cpp` correctement préparé

---

## 🛠️ Étapes de construction et de déploiement

### 1. 🔨 Compiler le programme

Compile le programme principal en C++ avec `g++` :

```bash
g++ --std=c++11 main.cpp -o main
```

---

### 2. 📦 Construire l'image Docker

Construis l’image :

```bash
sudo docker build -t cpp .
```

---

### 3. 🚢 Charger l’image dans Minikube

```bash
minikube image load cpp
```

---

### 4. ☸️ Déployer sur Kubernetes

Déploie les ressources via les fichiers YAML situés dans le dossier courant :

```bash
kubectl apply -f .
```

---

### 5. ▶️ Lancer le programme

Dans un terminal où le binaire `main` a été compilé, exécute :

```bash
./main
```

Le programme va tourner en boucle, toutes les 60 secondes :

- Lire les déploiements `fogsla=ok`
- Calculer la charge CPU totale du cluster
- Adapter dynamiquement les ressources des containers
- Étiqueter chaque deployment avec le niveau de charge (`patch=low`, `patch=medium`, `patch=high`)

---

## 🧼 Nettoyage

Pour supprimer toutes les ressources créées :

```bash
kubectl delete -f .
rm main
```
