#!/bin/bash

# Prüfen, ob ein Nickname übergeben wurde
if [ -z "$1" ]; then
    echo "Usage: ./login <nickname>"
    exit 1
fi

NICKNAME="$1"
SERVER="localhost"
PORT=6667

# Verbindung herstellen und offen halten
{ 
    echo "PASS 123"
    echo "NICK $NICKNAME"
    echo "USER 1 2 3 4"
    echo "JOIN #tc"
    cat
} | nc $SERVER $PORT

