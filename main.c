#include <stdio.h>
#include <stdlib.h>

#define MAX_HEIGHT 20
#define MAX_WIDTH 40
#define MIN_SIZE 4

typedef struct
{
    int height;
    int width;
    char board[MAX_HEIGHT][MAX_WIDTH];
    char current_player;
} ConnectFour;

void initialize_board(ConnectFour *game)
{
    for (int i = 0; i < game->height; ++i)
    {
        for (int j = 0; j < game->width; ++j)
        {
            game->board[i][j] = ' ';
        }
    }
    game->current_player = 'X';
}
