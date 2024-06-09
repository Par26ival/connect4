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


void display_board(ConnectFour *game)
{
    for (int i = 0; i < game->height; ++i)
    {
        for (int j = 0; j < game->width; ++j)
        {
            printf("|%c", game->board[i][j]);
        }
        printf("|\n");
    }
    for (int j = 0; j < game->width; ++j)
    {
        printf("--");
    }
    printf("-\n");
}

int drop_token(ConnectFour *game, int column)
{
    if (column < 0 || column >= game->width)
    {
        return 0;
    }
    for (int i = game->height - 1; i >= 0; --i)
    {
        if (game->board[i][column] == ' ')
        {
            game->board[i][column] = game->current_player;
            return 1;
        }
    }
    return 0;
}

void switch_player(ConnectFour *game)
{
    game->current_player = (game->current_player == 'X') ? 'O' : 'X';
}
