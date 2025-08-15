#!/bin/bash

SERVER="localhost"   # IRC server address
PORT=6667            # IRC server port
CLIENTS=200       # Number of clients to simulate

for i in $(seq 1 $CLIENTS); do
    {
        echo "CAP LS"
        sleep 0.1
        echo "PASS 123"
        sleep 0.1
        echo "NICK user$i"
        echo "USER user$i * 0 :Real Name $i"
        sleep 0.2
        echo "JOIN #testchannel"
        sleep 2
    } | nc $SERVER $PORT  &
    sleep 0.05  # Delay between launching clients
done


# for i in $(seq 1 $CLIENTS); do
#     {
#         echo "CAP LS"
#         sleep 0.1
#         echo "PASS 123"
#         sleep 0.1
#         echo "NICK user$i"
#         echo "USER user$i * 0 :Real Name $i"
#         sleep 0.2
#         echo "JOIN #testchannel"
#         sleep 2
#     } | nc $SERVER $PORT > "client$i.log" &
#     sleep 0.05  # Delay between launching clients
# done
# wait
