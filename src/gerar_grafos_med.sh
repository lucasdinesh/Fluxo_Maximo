#!/bin/bash

mkdir -p instancias

echo "Gerando instâncias levemente mais densas..."

# Mesh: malha mais larga e mais conexões
./makegraph 1 80 80 200 instancias/mesh.gr

# Random Level: mais nós e mais conexões
./makegraph 2 100 100 200 instancias/random_level.gr

# Matching: maior e mais conexões por par
./makegraph 4 4000 4 200 instancias/matching.gr

# Basic Line: com mais ramificações
./makegraph 6 100 100 5 200 instancias/basic_line.gr

# ExpLine: densidade e ramificações mais profundas
./makegraph 7 100 100 5 200 instancias/expline.gr

# GoldBad: mais conexões, para aumentar a dificuldade
./makegraph 10 4000 instancias/goldbad.gr

echo "Instâncias levemente mais densas geradas com sucesso!"