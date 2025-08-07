## 🚀 Commandes d’utilisation

### 🔧 1. Tout exécuter (build + déploiement) :

```bash
make all
```

Cela :

- Construit les images Docker pour le client et le serveur
- Charge les images dans Minikube
- Applique les fichiers YAML pour déployer les pods et services

### 🛠️ 2. Construire seulement les images :

```bash
make build-client
make build-server
```

### 📤 3. Charger les images dans Minikube :

```bash
make load-images
```

### ☸️ 4. Déployer les ressources Kubernetes :

```bash
make deploy
```

### 🧹 5. Nettoyer l’environnement :

```bash
make clean
```

Cela :
- Supprime les pods/services déployés
- Appelle make clean dans client/ et server/ (si ces répertoires contiennent leurs propres Makefiles)
Supprime les pods/services déployés

