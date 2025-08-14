# Générateur de nombres aléatoires entre Control Plane et Worker Node (client/serveur)

**Objectif :**
Poursuivre l’expérimentation de la communication inter-Pods en utilisant netcat (nc) comme outil de transmission entre le control-plane et les workers.
L’idée était de tester une solution ultra-légère et simple pour envoyer des données (nombres aléatoires) sans avoir à mettre en place un serveur web ou une API.

**Problème :**
Lors de nos tests, nous avons rapidement constaté une limitation majeure :
netcat ferme automatiquement la connexion dès qu’un message est envoyé.

Conséquences :
- Après chaque transmission, le lien entre le control-plane et les workers est interrompu.
-Il faut réétablir manuellement la connexion pour chaque nouveau message, ce qui empêche un flux continu.

**Solution mise en œuvre :**

- Déploiement d’un pod Control-Plane capable de lancer nc pour envoyer un nombre à un worker.
- Configuration d’un pod Worker écoutant sur un port via nc.
- Automatisation partielle de la reconnexion, mais toujours limitée par le comportement intrinsèque de netcat.

**Intérêt pour la suite du projet :**

Ce test nous a permis :
- De confirmer que netcat fonctionne pour des transmissions ponctuelles simples.
- D’identifier ses limites pour un usage en flux continu dans Kubernetes.
- De justifier le passage à des solutions plus robustes comme Flask et des communications HTTP persistantes.

---
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

