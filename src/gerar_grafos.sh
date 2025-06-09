#!/bin/bash

# Cria pasta se não existir
mkdir -p instancias

echo "Gerando instâncias de grafos..."

# Mesh
./makegraph 1 100 100 100 instancias/mesh.gr

# Square Mesh
./makegraph 5 100 2 100 instancias/square_mesh.gr

# Random Level
./makegraph 2 100 100 100 instancias/random_level.gr

# Random 2-Level
./makegraph 3 100 100 100 instancias/random_2level.gr

# Matching
./makegraph 4 5000 2 100 instancias/matching.gr

# BasicLine
./makegraph 6 100 100 3 100 instancias/basic_line.gr

# ExpLine
./makegraph 7 100 100 3 100 instancias/expline.gr

# DExpLine
./makegraph 8 50 50 3 100 instancias/dexpline.gr

# DinicBad
./makegraph 9 5000 instancias/dinicbad.gr

# GoldBad
./makegraph 10 5000 instancias/goldbad.gr

# Cheryian
./makegraph 11 100 100 2 100 instancias/cheryian.gr

echo "Todos os grafos foram gerados com sucesso na pasta 'instancias/'."

