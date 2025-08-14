# ⚙️ Script d’extraction de labels : conception, test et déploiement

**Objectif :**
Ce projet propose un script Bash avancé permettant d’extraire automatiquement les labels de tous les pods dans tous les namespaces d’un cluster Kubernetes, et de les enregistrer dans un fichier texte lisible.

**Améliorations par rapport à la version simple précédente :**

- Prise en compte de tous les namespaces.
- Récupération de tous les pods présents dans le cluster.
- Extraction de tous les labels associés à chaque pod.

**Solution mise en œuvre :**

- Script exécuté dans un conteneur Docker léger intégrant kubectl.
- Export des labels dans un fichier /tmp/labels.txt facilement consultable.
- Déploiement automatisé via YAML Kubernetes avec les droits nécessaires (RBAC).

**Lien avec la suite du projet :**

Ce module a été conçu comme une brique réutilisable pour des projets ultérieurs, notamment Koptim.
Dans Koptim, ce script servira à identifier les pods ayant un label spécifique (fogsla=ok) afin de leur appliquer un patch Kubernetes adapté à leur usage de CPU et mémoire.

**Intérêt :**

- Permet d’automatiser la collecte d’informations sur les pods.
- Sert de fondation à des mécanismes de supervision et d’optimisation des ressources dans Kubernetes.

---

## 🔍 Exemple de sortie générée

```text
--- Pod Labels ---

Namespace: default
Pod: label-checker
LABELS: env=test, app=demo

Namespace: kube-system
Pod: coredns-abc123
LABELS: k8s-app=kube-dns
```

Le fichier est généré sous : /tmp/labels.txt dans le conteneur.

---

## 📁 Contenu du projet

```text
.
├── script.sh               # Script Bash principal
├── Dockerfile              # Image Docker légère avec kubectl
├── rbacnew.yaml            # Rôles et permissions (RBAC) pour accéder aux pods
└── pod.yaml                # Définition du pod label-checker
```
---

## 🚀 Étapes de déploiement
Construire l’image Docker

```shell
docker build -t test17 .
```

Charger l’image dans Minikube

```shell
minikube image load test17
```
Appliquer les fichiers YAML pour RBAC et pod

```shell
kubectl apply -f rbacnew.yaml
kubectl apply -f pod.yaml
```

---

## 🧪 Tester le script
Une fois le pod label-checker lancé, vous pouvez consulter le fichier généré par le script avec :

```shell
kubectl exec -ti label-checker -- cat /tmp/labels.txt
```

Vous y verrez tous les labels regroupés par namespace et par pod.

---

## 🧼 Nettoyage
Pour supprimer tous les objets créés :

```shell
kubectl delete -f .
```
