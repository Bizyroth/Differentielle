#!bin/bash

echo Table des distributions differentielles
echo
./table
echo
echo Creation du fichier contenant les echantillons
echo \(Les echantillons sont dans le fichier couple\)
echo
./echantillon 5000 > couple
echo 
echo chiffrement du fichier d\'echantillonage
echo \(Les couples chiffrees sont place dans le fichier couple_chiffre\)
echo
./encrypt2linux couple > couple_chiffre
echo 
echo On commence la recherche de la cle
./key_finder couple couple_chiffre

exit 0
