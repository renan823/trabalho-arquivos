#!/bin/bash

make clean
make all

# Cria o binário
printf "1 teste.csv teste.bin" | ./trabalho
printf "2 teste.bin" | ./trabalho

# Remove o India
printf "4 teste.bin 1\n2 idAttack 3 country \"INDIA\"" | ./trabalho
printf "2 teste.bin" | ./trabalho

# Printa o Germany
printf "3 teste.bin 1\n1 country \"GERMANY\"" | ./trabalho

# Adiciona dois caras
printf "5 teste.bin 2\n5889 2017 82.93 \"GERMANY\" \"PHISHING\" NULO \"VPN\"\n6845 NULO 13.13 \"FRANCE\" \"SQL INJECTION\" NULO \"ANTIVIRUS\"" | ./trabalho
printf "2 teste.bin" | ./trabalho

# Remove os dois CHINA
printf "4 teste.bin 1\n1 country \"CHINA\"" | ./trabalho
printf "2 teste.bin" | ./trabalho

# Modifica os cara um inplace e outro out
printf "6 teste.bin 2\n1 country \"GERMANY\"\n1 country \"BRAZILI\"\n1 idAttack 5889\n1 country \"FORTALEZA\"" | ./trabalho
printf "2 teste.bin" | ./trabalho
