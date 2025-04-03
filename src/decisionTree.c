#include "decisionTree.h"
#include "game.h"
#include "piece.h"

Move minimax(Game *game, int depth, int alpha, int beta, int isMaximizingPlayer) {
    Move bestMove;
    bestMove.value = isMaximizingPlayer ? MOINS_INFINI : PLUS_INFINI;
    bestMove.idPiece = -1; // Par défaut, aucun coup trouvé

    if (depth == MAX_DEPTH) {
        bestMove.value = evaluateState(game->board);
        return bestMove;
    }

    Piece *pieceCpy = NULL;
    Move move;

    for (int pieceID = 0; pieceID < NB_PIECES; pieceID++) {
        if (game->pieces[game->currentPlayer - 1][pieceID]) {
            move.idPiece = pieceID;
            for (int row = 0; row < GRID_SIZE; row++) {
                move.row = row;
                for (int col = 0; col < GRID_SIZE; col++) {
                    move.col = col;
                    for (int rotation = 0; rotation < 4; rotation++) {                        
                        pieceCpy = copyPiece(game->pieces[game->currentPlayer - 1][pieceID]);
                        move.rotation = rotation;
                        for (int rota = 0; rota < rotation; rota++)
                        {
                            pieceCpy = rotatePiece(pieceCpy);
                        }
                        if (isValidPosition(game, pieceCpy, row, col)) {
                            placePiece(game, pieceCpy, pieceID, row, col);
                            switchPlayer(game);
                            
                            Move nextMove = minimax(game, depth + 1, alpha, beta, !isMaximizingPlayer);
                            
                            move.value = nextMove.value;
                            switchPlayer(game);
                            removePieceFromBoard(game, move);

                            if (isMaximizingPlayer) {
                                if (nextMove.value > bestMove.value) {
                                    bestMove.value = nextMove.value;
                                    bestMove.idPiece = pieceID;
                                    bestMove.row = row;
                                    bestMove.col = col;
                                    bestMove.rotation = rotation;
                                }
                                alpha = (alpha > nextMove.value) ? alpha : nextMove.value;
                            } else {
                                if (nextMove.value < bestMove.value) {
                                    bestMove.value = nextMove.value;
                                    bestMove.idPiece = pieceID;
                                    bestMove.row = row;
                                    bestMove.col = col;
                                    bestMove.rotation = rotation;
                                }
                                beta = (beta < nextMove.value) ? beta : nextMove.value;
                            }

                            if (beta <= alpha) return bestMove; // Coupure Alpha-Bêta
                        }
                    }
                }
            }
        }
    }
    
    return bestMove;
}


Piece *copyPiece(Piece *piece)
{
    Piece *pieceCpy = malloc(sizeof(Piece));
    pieceCpy->size = piece->size;
    pieceCpy->id = piece->id;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            pieceCpy->shape[i][j] = piece->shape[i][j];
        }
    }
    return pieceCpy;
}

// // Fonction qui enleve une piece joué, la rend au joueur concerné
void removePieceFromBoard(Game *game, Move move)
{
    int id = move.idPiece;
    int row = move.row;
    int col = move.col;
    // copie de la pièce :
    Piece *piece = copyPiece(game->globalPieces[id]);
    for (int r = 0; r < move.rotation; r++)
    {
        piece = rotatePiece(piece);
    }
    for (int i = -2; i < 3; i++)
    {
        for (int j = -2; j < 3; j++)
        {
            if (piece->shape[i + 2][j + 2] == 1)
            {
                game->board->grid[row + i][col + j] = 0;
            }
        }
    }
    // remet la pièce dans la table du joueur
    game->pieces[game->currentPlayer - 1][id] = copyPiece(game->globalPieces[id]);
}

void printBoard(Board *board)
{
    for (int k = -1; k < BOARD_SIZE; k++)
    {
        if (k >= 0 && k < 10)
            printf("%d  ", k);
        else if (k >= 10)
            printf("%d ", k);
        else
            printf("    ");
    }
    printf("\n");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (i < 10)
            printf("%d - ", i);
        else
            printf("%d- ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%d  ", board->grid[i][j]);
        }
        printf("\n");
    }
}

// Fonction pour compter les case possédés sur la grille par un joueur
int countCase(Board *board, int joueur)
{
    int counter = 0;
    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board->grid[i][j] == joueur)
            {
                counter++;
            }
        }
    }
    return counter;
}

// fonction qui compte le nombre de pièces d'un joueur et son nombre d'angles disponible
int countCorners(Board *board, int joueur)
{
    int rowOffsetsDiag[] = {-1, -1, 1, 1};
    int colOffsetsDiag[] = {-1, 1, -1, 1};
    int rowOffsetsAround[] = {-1, 1, 0, 0};
    int colOffsetsAround[] = {0, 0, -1, 1};
    int counter = 0;
    int k = 0;
    int neighbourRow = 0, neighbourCol = 0;
    int neighbourRowDiag = 0, neighbourColDiag = 0;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (board->grid[i][j] == joueur)
            {
                // counter -= abs((i - center) + (j - center));

                // pièce proche du centre
                if (i >= 6 && j >= 6)
                {
                    counter += 5;
                }
                while (k < 4)
                {
                    neighbourRowDiag = i + rowOffsetsDiag[i];
                    neighbourColDiag = j + colOffsetsDiag[i];
                    neighbourRow = i + rowOffsetsAround[i];
                    neighbourCol = j + colOffsetsAround[i];

                    // Vérification si les voisins sont dans les limites du tableau
                    if (neighbourRowDiag >= 0 && neighbourRowDiag < BOARD_SIZE && neighbourColDiag >= 0 && neighbourColDiag < BOARD_SIZE)
                    {
                        if (neighbourRow >= 0 && neighbourRow < BOARD_SIZE && neighbourCol >= 0 && neighbourCol < BOARD_SIZE)
                        {
                            if (board->grid[neighbourRowDiag][neighbourColDiag] == 0 && board->grid[neighbourRow][neighbourCol] == 0)
                            {
                                // compteur augmente dès qu'un angle de la pièce est libre
                                counter++;
                            }
                        }
                    }
                    k++;
                }
                k = 0;
                // compteur augmente plus pour donner plus de valeur aux grosses pièces
                counter += 2;
            }
        }
    }

    return counter;
}

// Fonction pour évaluer un état de jeu
int evaluateState(Board *board)
{ /*
     int nbCase1 = 0;
     int nbCase2 = 0;
     nbCase1 = countCase(board, 1);
     nbCase2 = countCase(board, 2);
     return nbCase1 - nbCase2;
     */
    int nbCase1 = 0;
    int nbCase2 = 0;
    nbCase1 = countCorners(board, 1);
    nbCase2 = countCorners(board, 2);
    return nbCase1 - nbCase2;
}