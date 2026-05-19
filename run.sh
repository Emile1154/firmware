#!/bin/bash

# Check for the required number of arguments
if [ "$#" -ne 5 ]; then
    echo "Usage: <local_port> <remote_port> <webserver_port> <mac_addr> <tcpport>"
    exit 1
fi
MAC_ADDR="${4//:/}"  # This removes all colons


if [ ${#MAC_ADDR} -ne 12 ]; then
    echo "Error: MAC address must be 12 hex digits after formatting."
    exit 1
fi

# Define the path to the executable
MESHTASTICD="$(pwd)/.pio/build/native_virtual/meshtasticd"

"$MESHTASTICD" -s -v -l "$1" -r "$2" -w "$3" -h "$MAC_ADDR" -p "$5"