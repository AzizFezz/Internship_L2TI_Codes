# 📦 Projet : surveillance et répartition dynamique des ressources dans Kubernetes

**Objectif :**
Ce projet met en œuvre un gestionnaire personnalisé pour optimiser dynamiquement les ressources CPU et mémoire des pods dans un cluster Kubernetes.

**Description :**

Le programme effectue les étapes suivantes :

1. Recherche de tous les Deployments ayant le label fogsla=ok.
2. Analyse des métriques CPU en temps réel.
3. Ajustement dynamique des limites (limits) et des demandes (requests) CPU/mémoire pour chaque pod concerné.
4. Mise à jour du label du Deployment en fonction de la charge observée :
    - `low` → faible charge
    - `medium` → charge modérée
    - `high` → forte charge

**Lien avec les autres projets :**
- Ce projet est une suite directe de `3-script-extraction-labels-bash` : le script précédent permettait déjà d’extraire et de filtrer les labels des pods, et Koptim réutilise cette logique pour cibler uniquement ceux qui possèdent `fogsla=ok`.
- Il s’inscrit aussi dans la continuité des travaux sur la communication entre pods et la collecte de métriques (netcat, Flask), car ces éléments sont essentiels pour surveiller et ajuster les ressources.

**Intérêt :**
- Offrir une solution maison d’optimisation des ressources, adaptée aux besoins spécifiques, en alternative à l’approche native de Kubernetes avec le VPA (cf. projet 4).
- Permettre un contrôle plus précis et plus flexible que les solutions standard, en intégrant des critères métiers dans la prise de décision.

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
