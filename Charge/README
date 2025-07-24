# 🦅 Récepteur Webhook Prometheus en C++ avec Crow et JSON

Ce projet est une application C++ simple basée sur [Crow](https://github.com/CrowCpp/crow), un micro-framework HTTP pour C++ (similaire à Flask), et la bibliothèque [nlohmann/json](https://github.com/nlohmann/json) pour le traitement de JSON.  
Elle reçoit des alertes Prometheus via webhook, les analyse et exécute une logique (par exemple, étiqueter des pods Kubernetes).

---

## 📁 Structure du projet

```
.
├── label.cpp # Fichier source principal
├── alert-sample.json # Exemple d'alerte Prometheus
├── crow/ # Framework Crow cloné
├── json.hpp # Bibliothèque JSON (header unique)
├── third_party/
    └── asio-*/ # ASIO standalone
```

---

## ⚙️ Prérequis

- Ubuntu (testé sur 20.04+)
- `g++-9` (support de C++17 et `string_view`)
- `libcurl4-openssl-dev`
- Crow
- Asio (version standalone)
- `json.hpp` de nlohmann

---

## 📦 Installation

```bash
# Cloner le framework Crow
git clone https://github.com/CrowCpp/crow.git

# Télécharger json.hpp
curl -LO https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp

# Installer les dépendances
sudo apt-get update
sudo apt-get install libcurl4-openssl-dev software-properties-common
```

## 🛠️ Installer g++-9 (pour support string_view)

```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install g++-9 gcc-9
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 90
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 90
sudo update-alternatives --config g++
sudo update-alternatives --config gcc
g++ --version
```

## 📥 Télécharger ASIO (requis par Crow)

```bash
mkdir -p third_party
cd third_party
wget https://github.com/chriskohlhoff/asio/archive/refs/tags/asio-1-28-1.tar.gz
tar -xzf asio-1-28-1.tar.gz
cd ..
```

## 🧪 Compilation et Exécution

Compiler :

```bash
g++ -std=c++17 label.cpp \
    -Icrow/include \
    -Ithird_party/asio-asio-1-28-1/asio/include \
    -I. \
    -lpthread -lcurl -o app
```

Lancer le serveur :

```bash
./app
```

Sortie attendue :

```bash
[INFO] Webhook server listening on 0.0.0.0:8080
...
```

## 📤 Tester avec un Webhook

Dans un autre terminal :


```bash
curl -X POST http://localhost:8080/ \
     -H "Content-Type: application/json" \
     -d @alert-sample.json
```

Sortie attendue :

```bash
Alerts processed
```
