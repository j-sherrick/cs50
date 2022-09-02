#!/bin/bash
for i in {1..3}
do
    for j in *0.txt
    do
        echo "running: sort$i: $j"
        time ./sort$i $j > /dev/null
        echo "******"
    done
done