#!/bin/bash

mkdir -p instancias

echo "Gerando instâncias de grafos otimizadas para análise..."

# Mesh - regular e pequeno
./makegraph 1 30 30 50 instancias/mesh.gr

# Random Level - aleatório e não tão denso
./makegraph 2 40 40 50 instancias/random_level.gr

# Matching - bom pra heurísticas diferentes, não tão grande
./makegraph 4 1000 2 50 instancias/matching.gr

# Basic Line - caminho mais simples, útil pra DFS
./makegraph 6 40 40 3 50 instancias/basic_line.gr

# ExpLine - estrutura tipo pirâmide que força vários caminhos
./makegraph 7 40 40 3 50 instancias/expline.gr

# GoldBad - notório por ser ruim pro DFS, mas pequeno
./makegraph 10 1000 instancias/goldbad.gr

echo "Instâncias leves e diversas geradas em 'instancias/'."
