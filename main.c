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


int check_direction(ConnectFour *game, int row, int col, int delta_row, int delta_col)
{
    char token = game->board[row][col];
    for (int i = 1; i < 4; ++i)
    {
        int r = row + delta_row * i;
        int c = col + delta_col * i;
        if (r < 0 || r >= game->height || c < 0 || c >= game->width || game->board[r][c] != token)
        {
            return 0;
        }
    }
    return 1;
}

int check_win(ConnectFour *game)
{
    for (int i = 0; i < game->height; ++i)
    {
        for (int j = 0; j < game->width; ++j)
        {
            if (game->board[i][j] == ' ')
            {
                continue;
            }
            if (check_direction(game, i, j, 1, 0) ||
                check_direction(game, i, j, 0, 1) ||
                check_direction(game, i, j, 1, 1) ||
                check_direction(game, i, j, 1, -1))
            {
                return 1;
            }
        }
    }
    return 0;
}

int is_full(ConnectFour *game)
{
    for (int i = 0; i < game->width; ++i)
    {
        if (game->board[0][i] == ' ')
        {
            return 0;
        }
    }
    return 1;
}

void save_to_file(ConnectFour *game, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }
    for (int i = 0; i < game->height; ++i)
    {
        for (int j = 0; j < game->width; ++j)
        {
            fprintf(file, "|%c", game->board[i][j]);
        }
        fprintf(file, "|\n");
    }
    for (int j = 0; j < game->width; ++j)
    {
        fprintf(file, "--");
    }
    fprintf(file, "-\n");
    fclose(file);
}

int main()
{
    ConnectFour game;
    printf("Enter board dimensions (height width): ");
    scanf("%d %d", &game.height, &game.width);

    if (game.height < MIN_SIZE || game.height > MAX_HEIGHT || game.width < MIN_SIZE || game.width > MAX_WIDTH)
    {
        printf("Invalid board size.\n");
        return 1;
    }

    initialize_board(&game);
    char filename[256];
    printf("Enter filename to save the game state: ");
    scanf("%s", filename);

    while (1)
    {
        display_board(&game);
        int move;
        printf("Player %c, enter column (1-%d): ", game.current_player, game.width);
        scanf("%d", &move);
        if (!drop_token(&game, move - 1))
        {
            printf("Invalid move. Try again.\n");
            continue;
        }

        save_to_file(&game, filename);

        if (check_win(&game))
        {
            display_board(&game);
            printf("Player %c wins!\n", game.current_player);
            break;
        }
        else if (is_full(&game))
        {
            display_board(&game);
            printf("The game is a draw!\n");
            break;
        }

        switch_player(&game);
    }

    return 0;
}