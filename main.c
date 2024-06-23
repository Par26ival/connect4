#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_HEIGHT 20
#define MAX_WIDTH 40

#define EMPTY 0
#define PLAYER_PIECE 1
#define AI_PIECE 2

#define PLAYER 0
#define AI 1

int board[MAX_HEIGHT][MAX_WIDTH];
int height, width;

void create_board()
{
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            board[r][c] = EMPTY;
        }
    }
}

void print_board()
{
    printf(" ");
    for (int c = 0; c < width; c++)
    {
        printf("%d ", c + 1);
    }
    printf("\n");

    for (int r = height - 1; r >= 0; r--)
    {
        printf("|");
        for (int c = 0; c < width; c++)
        {
            if (board[r][c] == EMPTY)
            {
                printf(" |");
            }
            else if (board[r][c] == PLAYER_PIECE)
            {
                printf("X|");
            }
            else if (board[r][c] == AI_PIECE)
            {
                printf("O|");
            }
        }
        printf("\n");
    }
    for (int c = 0; c < width; c++)
    {
        printf("--");
    }
    printf("-\n");
}

bool is_valid_location(int col)
{
    return board[height - 1][col] == EMPTY;
}

void drop_piece(int row, int col, int piece)
{
    board[row][col] = piece;
}

int get_next_open_row(int col)
{
    for (int r = 0; r < height; r++)
    {
        if (board[r][col] == EMPTY)
        {
            return r;
        }
    }
    return -1; // If column is full (should not happen if is_valid_location is checked)
}

bool winning_move(int piece)
{
    // Check horizontal
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            if (board[r][c] == piece &&
                board[r][c + 1] == piece &&
                board[r][c + 2] == piece &&
                board[r][c + 3] == piece)
            {
                return true;
            }
        }
    }

    // Check vertical
    for (int c = 0; c < width; c++)
    {
        for (int r = 0; r <= height - 4; r++)
        {
            if (board[r][c] == piece &&
                board[r + 1][c] == piece &&
                board[r + 2][c] == piece &&
                board[r + 3][c] == piece)
            {
                return true;
            }
        }
    }

    // Check diagonal (positive slope)
    for (int r = 0; r <= height - 4; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            if (board[r][c] == piece &&
                board[r + 1][c + 1] == piece &&
                board[r + 2][c + 2] == piece &&
                board[r + 3][c + 3] == piece)
            {
                return true;
            }
        }
    }

    // Check diagonal (negative slope)
    for (int r = 3; r < height; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            if (board[r][c] == piece &&
                board[r - 1][c + 1] == piece &&
                board[r - 2][c + 2] == piece &&
                board[r - 3][c + 3] == piece)
            {
                return true;
            }
        }
    }

    return false;
}

bool is_full()
{
    for (int c = 0; c < width; c++)
    {
        if (board[0][c] == EMPTY)
        {
            return false;
        }
    }
    return true;
}