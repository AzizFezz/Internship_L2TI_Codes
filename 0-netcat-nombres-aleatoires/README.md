# ⚙️ Générateur de nombres aléatoires entre Control Plane et Worker Node (netcat)

**Objectif :**
Ce premier projet sert de preuve de concept pour vérifier la faisabilité d’une communication inter-Pods dans Kubernetes.
Dans un contexte de supervision et de gestion de ressources, il est indispensable que les différents composants puissent échanger alertes et métriques en temps réel.

**Problème :**
Comment établir un canal de communication fiable entre un pod Control-Plane et un ou plusieurs pods Workers dans Kubernetes, sans utiliser d’outils complexes au départ ?

**Solution mise en œuvre :**
- Création de deux images Docker :
   - Control-Plane : génère périodiquement un nombre aléatoire et l’envoie aux workers.
   - Worker : reçoit le nombre et l’affiche.
- Utilisation de Minikube pour simuler le cluster Kubernetes.
- Communication testée pour valider le réseau interne entre pods.

**Intérêt pour la suite du projet :**

Cette étape a permis de valider la communication inter-Pods, ce qui est la base pour :
- Envoyer des alertes CPU/Mémoire.
- Transmettre des métriques à un système de supervision (Prometheus, Alertmanager).
- Implémenter une logique de traitement distribué.


---

## 🚧 Étapes de déploiement

### 1. 🔨 Construction des images Docker

```bash
sudo docker build -t control-plane:v1 ./control
sudo docker build -t worker-node:v1 ./worker
```

### 2. 📦 Chargement des images dans Minikube
   
```bash
minikube image load control-plane:v1
minikube image load worker-node:v1
```

### 3. ☸️ Déploiement sur Kubernetes

```bash
kubectl apply -f k8s/
```

Cette commande applique tous les fichiers YAML contenus dans le dossier k8s/.

---

## ✅ Vérification du fonctionnement

Pour s’assurer que tout fonctionne correctement, il est recommandé de consulter les logs d’un des pods workers :

```bash
kubectl logs -p <nom_du_pod_worker>
```

    Remplace <nom_du_pod_worker> par le nom exact d’un pod déployé (par exemple : machine-1-6f8d9f9fdd-abc12)

---

## 🧼 Nettoyage de l’environnement

Pour supprimer toutes les ressources déployées :

```bash
kubectl delete -f k8s/
```

