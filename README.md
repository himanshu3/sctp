

# SCTP Server

SCTP server written in C for testing purpose. It binds to any available IP address and takes the port number as a command-line argument.

## Compilation

Install `gcc` and `libsctp-dev` library, if not installed installed on your system.

```
sudo apt-get install build-essential libsctp-dev
```

To compile use following command:

```
gcc sctp_server.c -o sctp_server -lsctp
```

## Usage


```
./sctp_server [PORT]
```

`[PORT]` is the port number that the server should listen on.

## Docker

Use following command to build a Docker image:

```
docker build -t sctp-server .
```

To run a container using this image, use following command:

```
docker run -it --rm -p [HOST_PORT]:[CONTAINER_PORT] sctp-server [PORT]

docker run -d --name test-sctp -p 36412:9090/sctp sctp-server 9090
```

`[HOST_PORT]` is the port number on the host machine that should be mapped to the container's port
`[CONTAINER_PORT]` is the port number that the server is listening on inside the container   
`[PORT]` is the port number that the server should listen on.

## Kubernetes

Two deployment YAML files are there, to deploy the SCTP server on a Kubernetes cluster. To deploy with NodePort service use following command:

```
kubectl apply -f deployment_nodeport.yaml
```

To deploy with HostPort use following command:

```
kubectl apply -f deployment_hostport.yaml
```

