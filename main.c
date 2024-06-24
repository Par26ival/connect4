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
        if (board[height - 1][c] == EMPTY)
        {
            return false;
        }
    }
    return true;
}

int evaluate_window(int window[], int piece)
{
    int score = 0;
    int opponent_piece = (piece == PLAYER_PIECE) ? AI_PIECE : PLAYER_PIECE;

    // Evaluate window
    int count_piece = 0;
    int count_opponent = 0;
    int count_empty = 0;

    for (int i = 0; i < 4; i++)
    {
        if (window[i] == piece)
        {
            count_piece++;
        }
        else if (window[i] == opponent_piece)
        {
            count_opponent++;
        }
        else
        {
            count_empty++;
        }
    }

    if (count_piece == 4)
    {
        score += 100;
    }
    else if (count_piece == 3 && count_empty == 1)
    {
        score += 5;
    }
    else if (count_piece == 2 && count_empty == 2)
    {
        score += 2;
    }

    if (count_opponent == 3 && count_empty == 1)
    {
        score -= 4;
    }

    return score;
}

int score_position(int piece)
{
    int score = 0;

    // Score center column
    int center_array[height];
    for (int r = 0; r < height; r++)
    {
        center_array[r] = board[r][width / 2];
    }
    int center_count = 0;
    for (int r = 0; r < height; r++)
    {
        if (center_array[r] == piece)
        {
            center_count++;
        }
    }
    score += center_count * 3;

    // Score horizontal
    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            int window[4] = {board[r][c], board[r][c + 1], board[r][c + 2], board[r][c + 3]};
            score += evaluate_window(window, piece);
        }
    }

    // Score vertical
    for (int c = 0; c < width; c++)
    {
        for (int r = 0; r <= height - 4; r++)
        {
            int window[4] = {board[r][c], board[r + 1][c], board[r + 2][c], board[r + 3][c]};
            score += evaluate_window(window, piece);
        }
    }

    // Score positive slope diagonal
    for (int r = 0; r <= height - 4; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            int window[4] = {board[r][c], board[r + 1][c + 1], board[r + 2][c + 2], board[r + 3][c + 3]};
            score += evaluate_window(window, piece);
        }
    }

    // Score negative slope diagonal
    for (int r = 3; r < height; r++)
    {
        for (int c = 0; c <= width - 4; c++)
        {
            int window[4] = {board[r][c], board[r - 1][c + 1], board[r - 2][c + 2], board[r - 3][c + 3]};
            score += evaluate_window(window, piece);
        }
    }

    return score;
}

bool is_terminal_node()
{
    return winning_move(PLAYER_PIECE) || winning_move(AI_PIECE) || is_full();
}

int minimax(int depth, bool maximizingPlayer)
{
    if (depth == 0 || is_terminal_node())
    {
        if (winning_move(AI_PIECE))
        {
            return 1000000000;
        }
        else if (winning_move(PLAYER_PIECE))
        {
            return -1000000000;
        }
        else if (is_full())
        {
            return 0;
        }
        else
        {
            return score_position(AI_PIECE);
        }
    }

    if (maximizingPlayer)
    {
        int value = -1000000000; // -inf
        for (int col = 0; col < width; col++)
        {
            if (is_valid_location(col))
            {
                int row = get_next_open_row(col);
                int original_value = board[row][col];
                board[row][col] = AI_PIECE;
                int score = minimax(depth - 1, false);
                board[row][col] = original_value;
                if (score > value)
                {
                    value = score;
                }
            }
        }
        return value;
    }
    else
    {
        int value = 1000000000; // +inf
        for (int col = 0; col < width; col++)
        {
            if (is_valid_location(col))
            {
                int row = get_next_open_row(col);
                int original_value = board[row][col];
                board[row][col] = PLAYER_PIECE;
                int score = minimax(depth - 1, true);
                board[row][col] = original_value;
                if (score < value)
                {
                    value = score;
                }
            }
        }
        return value;
    }
}

int pick_best_move()
{
    int best_score = -1000000000; // -inf
    int best_col = 0;
    for (int col = 0; col < width; col++)
    {
        if (is_valid_location(col))
        {
            int row = get_next_open_row(col);
            board[row][col] = AI_PIECE;
            int score = minimax(5, false); // Adjust depth for AI difficulty
            board[row][col] = EMPTY;
            if (score > best_score)
            {
                best_score = score;
                best_col = col;
            }
        }
    }
    return best_col;
}

void save_game_results(const char *filename, int winner)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error opening file %s\n", filename);
        return;
    }

    fprintf(file, "Game results:\n");
    if (winner == PLAYER)
    {
        fprintf(file, "Winner: Player\n");
    }
    else if (winner == AI)
    {
        fprintf(file, "Winner: AI\n");
    }
    else
    {
        fprintf(file, "Game ended in a draw.\n");
    }

    fclose(file);
}

int main()
{
    srand(time(NULL)); // Initialize random seed

    char filename[100];
    printf("Enter the filename to save the game results: ");
    scanf("%s", filename);

    printf("Enter the board height (<= 20) and width (<= 40): ");
    scanf("%d %d", &height, &width);

    if (height > MAX_HEIGHT || width > MAX_WIDTH || height <= 0 || width <= 0)
    {
        printf("Invalid board dimensions.\n");
        return 1;
    }

    int mode;
    do
    {
        printf("Select mode: 1 for Player vs Player, 2 for Player vs Computer: ");
        scanf("%d", &mode);
    } while (mode != 1 && mode != 2);
    create_board();
    int turn = rand() % 2; // Randomly choose who starts
    while (!is_terminal_node())
    {
        print_board();
        if (turn == PLAYER)
        {
            // Player's turn
            int col;
            do
            {
                printf("Player's turn. Enter column (1-%d): ", width);
                scanf("%d", &col);
                col--; // Convert to 0-based index
            } while (col < 0 || col >= width || !is_valid_location(col));
            int row = get_next_open_row(col);
            drop_piece(row, col, PLAYER_PIECE);
            if (winning_move(PLAYER_PIECE))
            {
                print_board();
                printf("Player wins!\n");
                save_game_results(filename, PLAYER);
                break;
            }
        }
        else
        {
            // AI's turn
            if (mode == 1)
            {
                // Player vs Player mode
                int col;
                do
                {
                    printf("Player 2's turn. Enter column (1-%d): ", width);
                    scanf("%d", &col);
                    col--; // Convert to 0-based index
                } while (col < 0 || col >= width || !is_valid_location(col));
                int row = get_next_open_row(col);
                drop_piece(row, col, AI_PIECE);
                if (winning_move(AI_PIECE))
                {
                    print_board();
                    printf("Player 2 wins!\n");
                    save_game_results(filename, AI);
                    break;
                }
            }
            else
            {
                // Player vs Computer mode
                int col = pick_best_move();
                int row = get_next_open_row(col);
                drop_piece(row, col, AI_PIECE);
                if (winning_move(AI_PIECE))
                {
                    print_board();
                    printf("AI wins!\n");
                    save_game_results(filename, AI);
                    break;
                }
            }
        }
        turn = (turn + 1) % 2; // Switch turn
    }

    if (is_full() && !winning_move(PLAYER_PIECE) && !winning_move(AI_PIECE))
    {
        print_board();
        printf("Game over. It's a draw!\n");
        save_game_results(filename, -1);
    }

    return 0;
}