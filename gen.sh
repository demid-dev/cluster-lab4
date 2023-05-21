#!/bin/bash

file="vectors.txt"
vector_size=1800

for ((i = 0; i < vector_size; i++)); do
    num1=$((RANDOM % 100 + 1))
    num2=$((RANDOM % 100 + 1))

    echo -n "$num1 " >>"$file"
    echo -n "$num2 " >>"$file"
done

echo "Vectors generated and saved to $file"
