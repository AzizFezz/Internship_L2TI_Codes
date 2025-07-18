#!/bin/bash

IP="192.168.1.1"
INTERFACE="enp0s31f6"

sudo ip addr flush dev $INTERFACE
sudo ip -4 addr add $IP/24 dev $INTERFACE
sudo ip link set $INTERFACE up

echo "Config configuré avec IP: $IP" 
