#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Usage: $0 <num_subscribers>"
  exit 1
fi

n=$1

gcc -o publisher publisher.c
gcc -o subscriber subscriber.c

if [ $n -lt 1 -o $n -gt 3 ]; then
  echo "Number of subscribers must be between 1 and 3"
  exit 1
fi

rm -rf /tmp/ex1
mkdir /tmp/ex1

./publisher $n &

for i in $(seq 1 $n); do
  ./subscriber $i &  
done

wait