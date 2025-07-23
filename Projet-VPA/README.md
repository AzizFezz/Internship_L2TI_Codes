### 🔧 1. Mise à jour des CRD et configuration des droits RBAC

Suivre les instructions officielles ici : [Vertical Pod Autoscaler Installation](https://github.com/kubernetes/autoscaler/blob/master/vertical-pod-autoscaler/docs/installation.md)

```bash
kubectl apply -f https://raw.githubusercontent.com/kubernetes/autoscaler/vpa-release-1.0/vertical-pod-autoscaler/deploy/vpa-v1-crd-gen.yaml
kubectl apply -f https://raw.githubusercontent.com/kubernetes/autoscaler/vpa-release-1.0/vertical-pod-autoscaler/deploy/vpa-rbac.yaml
```

---
### 📁 2. Clonage du repo et lancement de VPA

```bash
git clone https://github.com/kubernetes/autoscaler.git
cd autoscaler/vertical-pod-autoscaler
git checkout origin/vpa-release-0.8
./hack/vpa-up.sh
```

⚠ **Note** : J’ai rencontré des erreurs avec dautres versions. La version `vpa-release-0.8` fonctionne correctement sur Minikube.

---
### ✅ 3. Vérification du déploiement

```bash
kubectl get pods -n kube-system
```

Tous les pods liés au VPA (recommender, updater, admission-controller) doivent être **en Running**.

---
### 🧪 4. Tester VPA sur une application (nginx)

Créez un dossier pour organiser le projet :

```
./
├── autoscaler/
└── demo/
```

commandes :

```bash
mkdir demo
cd demo
```

Créez deux fichiers YAML dans `demo/` :

 `my-nginx-vpa-deploy.yaml` (déploiement NGINX)

```yaml
---
apiVersion: apps/v1
kind: Deployment
metadata:
  name: nginx-vpa
spec:
  selector:
    matchLabels:
      app: nginx-vpa
  replicas: 2
  template:
    metadata:
      labels:
        app: nginx-vpa
    spec:
      securityContext:
        runAsNonRoot: true
        runAsUser: 65534 # nobody
      containers:
        - name: nginx
          image: nginx
          resources:
            requests:
              cpu: 100m
              memory: 50Mi
          command: ["/bin/sh"]
          args:
            - "-c"
            - "while true; do timeout 0.5s yes >/dev/null; sleep 0.5s; done"
```

`my-nginx-vpa.yaml` (configuration VPA)

```yaml
apiVersion: "autoscaling.k8s.io/v1"
kind: VerticalPodAutoscaler
metadata:
  name: nginx-vpa
spec:
  targetRef:
    apiVersion: "apps/v1"
    kind: Deployment
    name: nginx-vpa
  resourcePolicy:
    containerPolicies:
    - containerName: '*'
      minAllowed:
        cpu: 100m
        memory: 50Mi
      maxAllowed:
        cpu: 1
        memory: 500Mi
      controlledResources: ["cpu", "memory"]
```

Appliquer les fichiers :

```bash
kubectl create -f .
```

---
### 🔍 5. Vérifier les recommandations de VPA

```bash
kubectl describe vpa nginx-vpa | grep -A 20 "Target Ref:"
```

Ou bien en live (dans un terminal VS Code ou bash) :

```bash
watch kubectl top pods
```

---
### 🧹 6. Nettoyage

Pour supprimer les ressources de test :

```bash
kubectl delete -f .
```

Puis, pour arrêter le VPA :

```bash
cd autoscaler/vertical-pod-autoscaler/hack
./vpa-down.sh
```

---
## ✅ Résumé

| Étape | Action                                  |
| ----- | --------------------------------------- |
| 📥    | Télécharger les fichiers CRD & RBAC     |
| 💻    | Cloner le repo autoscaler et lancer VPA |
| 🧪    | Déployer un exemple avec NGINX          |
| 📊    | Vérifier les recommandations VPA        |
| 🧼    | Nettoyer les ressources                 |
