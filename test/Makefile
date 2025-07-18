# Top level Makefile to automate build/deploy on minikube

CLIENT_IMAGE=my-sla-client:v1
SERVER_IMAGE=my-sla-server:v1

.PHONY: all build-client build-server load-images deploy clean

all: build-client build-server load-images deploy

build-client:
	$(MAKE) -C client
	docker build -t $(CLIENT_IMAGE) client/

build-server:
	$(MAKE) -C server
	docker build -t $(SERVER_IMAGE) server/

load-images:
	minikube image load $(CLIENT_IMAGE)
	minikube image load $(SERVER_IMAGE)

deploy:
	minikube kubectl -- apply -f k8s/control-plane-deployment.yaml
	minikube kubectl -- apply -f k8s/machine-1-deployment.yaml
	minikube kubectl -- apply -f k8s/machine-2-deployment.yaml

clean:
	$(MAKE) -C client clean
	$(MAKE) -C server clean
	minikube kubectl -- delete -f k8s/control-plane-deployment.yaml --ignore-not-found
	minikube kubectl -- delete -f k8s/machine-1-deployment.yaml --ignore-not-found
	minikube kubectl -- delete -f k8s/machine-2-deployment.yaml --ignore-not-found
