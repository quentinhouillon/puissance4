# Puissance 4

Ce projet est une implémentation du jeu Puissance 4 en C utilisant la bibliothèque ncurses. Il inclut également un bot basé sur l'algorithme Minimax avec une optimisation moyenne. Pour changer le nombre de joueurs ou de bots, modifiez les lignes 7 et 8. Toutefois le bot est meilleur en 1v1. Pour modifier la grille, modifiez la ligne 6.

## Fonctionnalités

- Interface utilisateur en mode texte avec ncurses
- Jeu à deux joueurs ou +
- Bot Minimax pour jouer contre l'ordinateur
- Détection de victoire et match nul

## Prérequis

- GCC (ou tout autre compilateur C compatible comme CMAKE)
- Bibliothèque ncurses

## Installation

Clonez le dépôt et compilez le projet :

```sh
git clone https://github.com/quentinhouillon/puissance4.git
cd puissance4
gcc -lncurses -o main main.c
./main
```

## Utilisation

Lancez le jeu avec la commande suivante :

```sh
./puissance4
```
## Auteurs

- Quentin HOUILLON
