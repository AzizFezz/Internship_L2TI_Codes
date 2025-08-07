# 🔢 Application Web de Génération et d’Affichage de Nombres aléatoires (Flask + Kubernetes)

Ce projet illustre une **application web distribuée** en architecture **client-serveur**, déployée dans un cluster **Kubernetes**. Il est basé sur **Flask**, avec un **control-plane** qui génère des nombres aléatoires et les envoie à des **workers** qui les affichent en temps réel via une page web.

---

## 🧱 Architecture

- 🎛️ **Control Plane (Serveur)** : Génère des nombres aléatoires et les envoie périodiquement aux workers via TCP.
- 🖥️ **Workers (Clients)** : Reçoivent les nombres, les stockent localement, et les affichent dans une interface web Flask.

---

🚀 Déploiement avec Minikube
1. Construire les images Docker

Dans le dossier du control-plane
```shell
cd control
docker build -t control-plane:v3 .
```

Dans le dossier du worker
```shell
cd ../worker
docker build -t worker-node:v3 .
```
2. Charger les images dans Minikube
```shell
minikube image load control-plane:v3
minikube image load worker-node:v3
```

3. Appliquer les fichiers YAML Kubernetes

```shell
cd ..
kubectl apply -f .
```

Cela déploie :

Un pod control-plane générant les nombres

Un ou plusieurs pods worker affichant les données reçues

---

## 🌐 Accès à l’interface web
Pour ouvrir l’interface web d’un worker (par exemple machine-1) :

```shell
minikube service machine-1-nodeport
```

Un navigateur s’ouvrira avec une page dynamique, affichant les nombres générés par le control-plane en temps réel.

    💡 Répéter l’opération avec machine-2-nodeport pour voir le comportement multi-worker.

---

## ✅ Comportement attendu

- Le control-plane envoie en boucle des nombres aléatoires.
- Les workers reçoivent les données via TCP, les écrivent dans un fichier.
- Flask lit ce fichier et met à jour l’interface utilisateur automatiquement.
- L'utilisateur peut consulter les nombres sur une interface web simple.
