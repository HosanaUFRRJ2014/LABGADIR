#!/bin/bash
################################################################
#
# Criação de estrutura de pastas para Lab de Grafos e Algoritmos.
#
################################################################

if [ $# -ne 2 ]; then
    echo $0: Uso correto: install.sh "<"número matrícula">" "<"pasta raiz de destino">"
    exit 1
fi

if [[ ! -d "$2" && ! -L "$2" ]]; then
    echo Pasta $2 não encontrada
    exit 1
fi

if [ ! -d "../$1" ]; then
    echo Pasta referente ao número de matrícula $1 não encontrada
    exit 1
fi

if [ ! -d "../$1/grafalg16.2" ]; then
    echo Pasta $1/grafalg16.2 não encontrada
    exit 1
fi

ln -s "${PWD}" "$2/grafalg16.2"

cd ..
ln -s "${PWD}/datast" "$2/datast"
ln -s "${PWD}/graph" "$2/graph"
ln -s "${PWD}/lib" "$2/lib"
ln -s "${PWD}/instancias" "$2/instancias"
ln -s "${PWD}/$1/grafalg16.2" "$2/grafalg16.2/autor_$1"
