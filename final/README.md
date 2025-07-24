# ⚙️ Gestionnaire d'Alertes CPU pour Kubernetes

Ce projet fournit un système automatisé pour traiter les alertes CPU dans un cluster Kubernetes à l’aide de **Prometheus**, **Alertmanager** et d’une **application Flask** personnalisée.

Lorsque l’utilisation CPU d’un pod dépasse certains seuils prédéfinis, ce système applique automatiquement des **labels Kubernetes** aux pods concernés. Ces labels peuvent ensuite être utilisés pour déclencher des actions correctives (scaling, éviction, alertes, etc.).

---

## 📁 Structure du projet
```
.
├── receiver.py # Application Flask pour recevoir et traiter les alertes
├── label_cpu_normal.cpp # Script C++ : label pour CPU normal
├── label_cpu_moderate.cpp # Script C++ : label pour CPU élevé
├── label_cpu_critical.cpp # Script C++ : label pour CPU critique (panne)
└── alert.json # Exemple de payload d’alerte pour test
```

---

## ⚙️ Fonctionnement

1. **Prometheus** surveille les métriques du cluster Kubernetes.
2. **Alertmanager** envoie les alertes via webhook à l’application Flask.
3. L’application Flask analyse l’alerte et exécute le script C++ approprié :
   - `PodCpuNormal` → `label_cpu_normal`
   - `PodHighCpuUsage` → `label_cpu_moderate`
   - `PodCpuFailure` → `label_cpu_critical`
4. Chaque script applique un label sur le pod concerné via `kubectl`.

---

## 🧰 Prérequis

- Python 3.5
- Flask (`pip install flask`)
- g++ (pour compiler les scripts C++)
- `kubectl` configuré pour accéder à votre cluster Kubernetes
- Prometheus + Alertmanager (déjà déployés et configurés avec webhook)

---

## 🛠️ Installation & Lancement

1. **Cloner le dépôt**

```bash
git clone https://github.com/AzizFezz/Internship_L2TI_Codes.git
cd final
```

2. **Installer Flask**

```bash
pip install flask
```

3. **Compiler les scripts C++**

```bash
g++ -o label_cpu_normal label_cpu_normal.cpp
g++ -o label_cpu_moderate label_cpu_moderate.cpp
g++ -o label_cpu_critical label_cpu_critical.cpp
```

4. **Démarrer l’application Flask**

```bash
FLASK_APP=receiver.py flask run --host=0.0.0.0 --port=5000
```

Le serveur démarre sur http://0.0.0.0:5000/alert.

---

## 🧪 Tester avec un exemple d’alerte

Utilisez le fichier alert.json fourni pour simuler une alerte :

```bash
curl -X POST http://localhost:5000/alert \
     -H "Content-Type: application/json" \
     -d @alert.json
```

Exemple de contenu dans alert.json :

```bash
{
  "alerts": [
    {
      "labels": {
        "alertname": "PodHighCpuUsage",
        "pod": "mon-pod", #modifier le nom de pod
        "namespace": "default"
      },
      "annotations": {
        "cpu_usage": "92"
      }
    }
  ]
}

```

💡 Vous pouvez tester d'autres cas en modifiant alertname par PodCpuNormal ou PodCpuFailure.

---

## 🧼 Exemple de commande exécutée (dans les scripts)

Chaque script exécute une commande comme celle-ci :

```bash
kubectl label pod mon-pod --namespace=default status=panne --overwrite
```

Cela permet aux autres outils du cluster de détecter automatiquement l’état du pod.

---

## 📌 Remarques

    - L’application Flask doit être accessible par Alertmanager (modifier l'adresse ip de l'application flask dans le script de Alertmanager).

    - Assurez-vous que le port Flask (5000) est ouvert si vous exécutez l'app dans un pod Kubernetes.

    - kubectl doit être configuré avec les bons accès pour modifier les pods.
