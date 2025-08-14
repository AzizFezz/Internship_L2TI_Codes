# 🔢 Application Web de Génération et d’Affichage de Nombres aléatoires (Flask + Kubernetes)

**Objectif :**
Ce projet illustre une application web distribuée en architecture client-serveur, déployée dans un cluster Kubernetes.

Il repose sur Flask, avec :
- Un Control-Plane qui génère des nombres aléatoires.
- Un ou plusieurs Workers qui reçoivent ces nombres et les affichent en temps réel via une interface web.

**Pourquoi Flask ?**

Après les limitations rencontrées avec netcat (connexion interrompue après chaque envoi), nous avons décidé de passer à Flask afin de :
- Maintenir une communication continue entre control-plane et workers.
- Offrir un affichage plus ergonomique et dynamique grâce à une page web générée côté worker.
- Simplifier la mise en forme et l’actualisation des données reçues.

**Solution mise en œuvre :**
- Développement d’un service Flask côté worker affichant les nombres en temps réel.
- Déploiement d’un service Flask côté control-plane chargé d’envoyer périodiquement les nombres.
- Utilisation de Docker pour empaqueter les deux rôles (control-plane et worker).
- Chargement des images dans Minikube puis déploiement via YAML Kubernetes.

**Intérêt pour la suite du projet :**

- Mise en place d’une base solide pour la transmission continue de métriques entre pods.
- Possibilité d’adapter ce système pour l’envoi de données de supervision ou d’alertes.
- Première étape vers l’intégration avec des outils comme Prometheus et Alertmanager.

---

## 🚀 Déploiement avec Minikube
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
