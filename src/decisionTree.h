#ifndef ARBRE_H
#define ARBRE_H
#include <stdio.h>
#include <stdlib.h>
#define PLUS_INFINI 100000
#define MOINS_INFINI -100000
#define MAX_DEPTH 2
#define GRID_SIZE 14
#include "game.h"

typedef struct Move // il faut renvoyer une struct qui contient l id, la rota, la col, la row
{
    int value;
    int idPiece;
    int rotation;
    int col;
    int row;
} Move;

void printBoard(Board *board);

void removePieceFromBoard(Game *game, Move move);

// Fonction Minimax
Move minimax(Game *game, int depth, int alpha, int beta, int isMaximizingPlayer);

// Fonction pour évaluer un état de jeu
int evaluateState(Board *board);

// Fonction pour compter les case possédés sur la grille par un joueur
int countCase(Board *board, int joueur);

// Fonction pour compter les case possédés sur la grille par un joueur
int countCorners(Board *board, int joueur);

// Fonction qui copie une piece
Piece *copyPiece(Piece *piece);

#endif