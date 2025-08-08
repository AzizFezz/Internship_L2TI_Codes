# ⚙️ Déploiement d’un Système Control-Plane / Worker sur Kubernetes

Ce projet repose sur une architecture **control-plane / worker**.  
L’objectif est de construire deux images Docker (une pour le control-plane et une pour les workers), de les charger dans **Minikube**, puis de déployer le tout à l’aide de fichiers YAML.

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

