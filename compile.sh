#!/bin/bash

# Cambia al directorio 'build'
cd build

# Ejecuta el comando 'make'
make

# Regresa al directorio anterior
cd ..

# Ejecuta el programa 'dental_seg' desde el directorio 'build'
./build/dental_seg
