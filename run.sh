#!/bin/bash

IP="localhost"
PORT="8888"

gnome-terminal -- /bin/bash -c "./TetrisServer $PORT 180"
gnome-terminal -- /bin/bash -c "./TetrisClient $IP $PORT"
gnome-terminal -- /bin/bash -c "./TetrisClient $IP $PORT"
