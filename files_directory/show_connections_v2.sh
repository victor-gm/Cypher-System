#!/bin/bash
for PORT in $(eval echo {$1..$2}); do
  	timeout 0.2 bash -c "</dev/tcp/$3/$PORT &>/dev/null" &&  echo "port $PORT is open"
done




