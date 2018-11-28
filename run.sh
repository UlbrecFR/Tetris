#!/bin/bash

IP="localhost"
PORT="8888"

gnome-terminal -- /bin/bash -c "./TetrisServer $PORT"
gnome-terminal -- /bin/bash -c "./TetrisClient $IP $PORT"
gnome-terminal -- /bin/bash -c "./TetrisClient $IP $PORT"
