#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <string.h>

// Generate random numbers between 0 to n-1 inclusive
#define random(n) (rand()%(n))

char *input(char *buffer, size_t n);
char inputc();

// Clear the grid for a new game
#define clearGrid(grid) memset(grid, ' ', 9)

void gridOut(char grid[9]);
char switchPlayer(char player);
void getEmptySpaces(char grid[9], int indexes[9]);
int placeToken(char player, char grid[9], char *coordinate);
int checkWinner(char player, char grid[9]);

// Used for determining when to minimise/maximise the scores
typedef enum _compare{
    max = 1,
    min = -1,
} compare;

int normalAI(char grid[9], char player, int emptySpaces);
int minimax(char grid[9], int emptySpaces, char player, compare cmp);
int impossibleAI(char grid[9], char player, int emptySpaces);





char *input(char *buffer, size_t n)
{
    if (fgets(buffer, n, stdin) == NULL)
    {
        return NULL;
    };
    size_t nl = strlen(buffer) - 1;
    if (buffer[nl] == '\n')
    {
        buffer[nl] = '\0';
    }
    else
    {
        while (1)
        {
            char c;
            c = getchar();
            if (c == '\n')
            {
                break;
            }
        }
    }
    return buffer;
}

char inputc()
{
    char ch = getchar();
    if (feof(stdin) != 0)
    {
        return 0;
    };
    if (ch != '\n')
    {
        while (1)
        {
            char c;
            c = getchar();
            if (c == '\n')
            {
                break;
            }
        }
    }
    return ch;
}

void gridOut(char grid[9])
{
    printf(" |A|B|C|\n");
    printf("-+-----+\n");
    printf("1|%c|%c|%c|\n", grid[0], grid[1], grid[2]);
    printf("-|-----|\n");
    printf("2|%c|%c|%c|\n", grid[3], grid[4], grid[5]);
    printf("-|-----|\n");
    printf("3|%c|%c|%c|\n", grid[6], grid[7], grid[8]);
    printf("-+-----+\n\n");
}

char switchPlayer(char player)
{
    if (player == 'X')
    {
        return 'O';
    }
    else
    {
        return 'X';
    }
}

void getEmptySpaces(char grid[9], int indexes[9])
{
    int count = 0;
    for (int i = 0; i < 9; i++)
    {
        if (grid[i] == ' ')
        {
            indexes[count] = i;
            count++;
        }
    }
}

int placeToken(char player, char grid[9], char *coordinate)
{
    int y;
    char x;
    sscanf(coordinate, "%1d%1c", &y, &x);
    y--;
    x = toupper(x) - 65;
    if ((x < 0 || x > 2) || (y < 0 || y > 2))
    {
        printf("Please enter a coordinate between 1A and 3C\n\n");
        return -1;
    };

    // Check if a token has already been placed in the grid
    if (grid[y * 3 + x] != ' ')
    {
        printf("A token has already been placed here!\n\n");
        return -1;
    }

    grid[y * 3 + x] = player;
    return 0;
}

// Checks if a certain player has won
int checkWinner(char player, char grid[9])
{
    if (
        (grid[0] == player && grid[4] == player && grid[8] == player) ||
        (grid[2] == player && grid[4] == player && grid[6] == player) ||
        (grid[0] == player && grid[1] == player && grid[2] == player) ||
        (grid[3] == player && grid[4] == player && grid[5] == player) ||
        (grid[6] == player && grid[7] == player && grid[8] == player) ||
        (grid[0] == player && grid[3] == player && grid[6] == player) ||
        (grid[1] == player && grid[4] == player && grid[7] == player) ||
        (grid[2] == player && grid[5] == player && grid[8] == player))
    {
        return 1;
    }

    return 0;
}

int normalAI(char grid[9], char player, int emptySpaces)
{
    int indexes[9];
    getEmptySpaces(grid, indexes);
    grid[indexes[random(emptySpaces)]] = player;

    return 0;
}

int minimax(char grid[9], int emptySpaces, char player, compare cmp)
{

    int indexes[9];
    getEmptySpaces(grid, indexes); // Stores a list of empty spaces in the grid in the array 'indexes'

    if (cmp == min)
    {
        int score = 1000;
        int thisScore;
        for (int i = 0; i < emptySpaces; i++) // Go through each free index
        {
            grid[indexes[i]] = player;
            if (checkWinner(player, grid) == 1)
            {
                thisScore = emptySpaces * cmp;
            }
            else if (emptySpaces - 1 == 0)
            {
                thisScore = 0;
            }
            else
            {
                thisScore = minimax(grid, emptySpaces - 1, switchPlayer(player), max);
            }
            // printf("%d -> ", thisScore);
            if (thisScore < score)
            {
                score = thisScore;
            }
            grid[indexes[i]] = ' ';
        }

        return score;
    }
    else
    {
        int score = -1000;
        int thisScore;
        for (int i = 0; i < emptySpaces; i++)
        {
            grid[indexes[i]] = player;
            if (checkWinner(player, grid) == 1)
            {
                thisScore = emptySpaces * cmp;
            }
            else if (emptySpaces - 1 == 0)
            {
                thisScore = 0;
            }
            else
            {
                thisScore = minimax(grid, emptySpaces - 1, switchPlayer(player), min);
            }
            // printf("%d -> ", thisScore);
            if (thisScore > score)
            {
                score = thisScore;
            }
            grid[indexes[i]] = ' ';
        }

        return score;
    }
}

int impossibleAI(char grid[9], char player, int emptySpaces)
{
    int score = -1000;
    int thisScore;

    int move = -1;

    int indexes[9];
    getEmptySpaces(grid, indexes);

    for (int i = 0; i < emptySpaces; i++)
    {
        grid[indexes[i]] = player;
        if (checkWinner(player, grid) == 1)
        {
            thisScore = emptySpaces;
        }
        else if (emptySpaces - 1 == 0)
        {
            thisScore = 0;
        }
        else
        {
            thisScore = minimax(grid, emptySpaces - 1, switchPlayer(player), min);
        }
        if (thisScore > score)
        {
            score = thisScore;
            move = i;
        }
        grid[indexes[i]] = ' ';
    }

    grid[indexes[move]] = player;
    return 0;
}

int mouseCollide(Vector2 mousePos, Vector2 gridPos, int squareSize, int gridSpace);
int checkButton(Rectangle buttons[3]);

int main(void)
{
	// Definitions of gameplay variables
	char grid[9];
	clearGrid(grid);

	// Value of X represents player 1's turn, value of O represents player 2's turn
	char playerTurn = 'X';
	char human = 'X';
	srand(time(0));
	// Used when determining who won for the output
	char winner = 0;

	int emptySpaces = 9;
	int grid_index;

	// Value of 0 means the game is still going on
	// Value of 1 means the game is on delay
	//  Value of 2 means the game is over
	int gameState = 0;

	// Mode variable. 0 for 2 player, 1 for normal AI, and 2 for impossible AI mode
	char mode = '0';

	// Window initialisation

	const int screenWidth = 800, screenHeight = 800;
	InitWindow(screenWidth, screenHeight, "Tic Tac Toe");
	SetTargetFPS(60);
	const int squareSize = 150, gridSpace = 20;

	RenderTexture2D screen = LoadRenderTexture(screenWidth, screenHeight);

	RenderTexture2D X = LoadRenderTexture(squareSize, squareSize);
	RenderTexture2D O = LoadRenderTexture(squareSize, squareSize);

	// Draw out the X and O textures
	{
		BeginTextureMode(X);
		ClearBackground(WHITE);
		DrawTriangle((Vector2){25, 0}, (Vector2){0, 25}, (Vector2){squareSize - 25, squareSize}, BLACK);
		DrawTriangle((Vector2){25, 0}, (Vector2){squareSize - 25, squareSize}, (Vector2){squareSize, squareSize - 25}, BLACK);
		DrawTriangle((Vector2){squareSize - 25, 0}, (Vector2){25, squareSize}, (Vector2){squareSize, 25}, BLACK);
		DrawTriangle((Vector2){squareSize - 25, 0}, (Vector2){0, squareSize - 25}, (Vector2){25, squareSize}, BLACK);
		EndTextureMode();

		BeginTextureMode(O);
		ClearBackground(WHITE);
		DrawCircle(squareSize / 2, squareSize / 2, squareSize / 2 - 5, BLACK);
		DrawCircle(squareSize / 2, squareSize / 2, 45, WHITE);
		EndTextureMode();
	}

	const int timer = 5 * 60; // Number of seconds to delay * frame rate
	int counter = 0;

	// 0 = Home, 1 = Difficulty, 2 = Player selection, 3 = Main game
	int loopState = 0;

	int pressedButton;

	Rectangle buttons[3] = {
		(Rectangle){250, 260, 300, 100},
		(Rectangle){250, 380, 300, 100},
		(Rectangle){250, 500, 300, 100}};

	// Main gameplay loop
	while (!WindowShouldClose())
	{
		switch (loopState)
		{
		case 0:
			// Home screen
			pressedButton = checkButton(buttons);

			if ((pressedButton == 0 || pressedButton == 1) && IsMouseButtonPressed(0))
			{
				// Check if multiplayer (pressedButton = 0) or single player (pressedButton = 1) has been chosen
				loopState = (pressedButton == 0) ? 3 : 1;
			}
			else if (pressedButton == 2 && IsMouseButtonPressed(0))
			{
				// Exit the game
				goto EXIT;
			}

			BeginDrawing();
			BeginTextureMode(screen);
			ClearBackground(WHITE);
			DrawText("Tic-Tac-Toe", 100, 60, 90, BLACK);
			DrawText("Raylib edition", 240, 155, 50, BLACK);

			// Draw the buttons
			for (int i = 0; i < 3; i++)
			{
				DrawRectangleRounded(buttons[i], 0.2, 5, (pressedButton == i) ? BLACK : DARKGRAY);
				DrawRectangleRounded((Rectangle){buttons[i].x + 10, buttons[i].y + 10, 280, 80}, 0.2, 5, WHITE);
			}

			DrawText("Two Player", 275, 288, 45, (pressedButton == 0) ? BLACK : DARKGRAY);
			DrawText("Vs AI", 330, 408, 45, (pressedButton == 1) ? BLACK : DARKGRAY);
			DrawText("Exit", 360, 528, 45, (pressedButton == 2) ? BLACK : DARKGRAY);

			EndTextureMode();
			DrawTextureRec(screen.texture, (Rectangle){0, 0, screenWidth, -screenHeight}, (Vector2){0, 0}, WHITE);
			EndDrawing();
			break;

		case 1:
			// Difficulty
			pressedButton = checkButton(buttons);

			// Check which difficulty was selected
			if ((pressedButton == 0 || pressedButton == 1) && IsMouseButtonPressed(0))
			{
				mode = pressedButton == 0 ? '1' : '2';
				loopState = 2;
			}
			else if (pressedButton == 2 && IsMouseButtonPressed(0))
			{ // Go back to the home screen
				loopState = 0;
			}

			BeginDrawing();
			BeginTextureMode(screen);
			ClearBackground(WHITE);
			DrawText("Select Difficulty", 85, 100, 75, BLACK);

			// Draw the buttons
			for (int i = 0; i < 3; i++)
			{
				DrawRectangleRounded(buttons[i], 0.2, 5, (pressedButton == i) ? BLACK : DARKGRAY);
				DrawRectangleRounded((Rectangle){buttons[i].x + 10, buttons[i].y + 10, 280, 80}, 0.2, 5, WHITE);
			}

			DrawText("Easy", 345, 288, 45, (pressedButton == 0) ? BLACK : DARKGRAY);
			DrawText("Impossible", 292, 408, 45, (pressedButton == 1) ? BLACK : DARKGRAY);
			DrawText("Home", 350, 528, 45, (pressedButton == 2) ? BLACK : DARKGRAY);

			EndTextureMode();
			DrawTextureRec(screen.texture, (Rectangle){0, 0, screenWidth, -screenHeight}, (Vector2){0, 0}, WHITE);
			EndDrawing();
			break;

		case 2:
			// Player selection
			pressedButton = checkButton(buttons);

			if ((pressedButton != -1) && IsMouseButtonPressed(0))
			{
				if (pressedButton == 0)
				{
					human = 'X';
				}
				else if (pressedButton == 1)
				{
					human = 'O';
				}
				else
				{
					human = (char[2]){'X', 'O'}[random(2)];
				}
				loopState = 3;
			}

			BeginDrawing();
			BeginTextureMode(screen);
			ClearBackground(WHITE);
			DrawText("Play As:", 250, 100, 75, BLACK);

			// Draw the buttons
			for (int i = 0; i < 3; i++)
			{
				DrawRectangleRounded(buttons[i], 0.2, 5, (pressedButton == i) ? BLACK : DARKGRAY);
				DrawRectangleRounded((Rectangle){buttons[i].x + 10, buttons[i].y + 10, 280, 80}, 0.2, 5, WHITE);
			}

			DrawText("X", 385, 288, 45, (pressedButton == 0) ? BLACK : DARKGRAY);
			DrawText("O", 385, 408, 45, (pressedButton == 1) ? BLACK : DARKGRAY);
			DrawText("Random", 320, 528, 45, (pressedButton == 2) ? BLACK : DARKGRAY);

			EndTextureMode();
			DrawTextureRec(screen.texture, (Rectangle){0, 0, screenWidth, -screenHeight}, (Vector2){0, 0}, WHITE);
			EndDrawing();
			break;
		case 3:
			// Main game
			switch (gameState)
			{
			case 0:
				grid_index = mouseCollide(GetMousePosition(), (Vector2){155, 155}, squareSize, gridSpace);
				if ((mode == '0' || human == playerTurn))
				{
					if (IsMouseButtonPressed(0) && grid_index != -1 && grid[grid_index] == ' ')
					{
						grid[grid_index] = playerTurn; // Place the player token on the grid
						playerTurn = switchPlayer(playerTurn);
						emptySpaces--;
					}
				}
				else if (human != playerTurn)
				{
					if (mode == '1')
					{
						normalAI(grid, playerTurn, emptySpaces);
					}
					else
					{
						impossibleAI(grid, playerTurn, emptySpaces);
					}
					playerTurn = switchPlayer(playerTurn);
					emptySpaces--;
				}

				if (checkWinner('X', grid) == 1)
				{
					winner = 'X';
					gameState = 1;
				}
				else if (checkWinner('O', grid) == 1)
				{
					winner = 'O';
					gameState = 1;
				}
				else if (emptySpaces == 0)
				{
					winner = '0';
					gameState = 1;
				}
				break;

			case 1:
				// Delay the game for some time after a win
				if (counter >= timer)
				{
					gameState = 2;
					counter = 0;
				}
				else
				{
					counter++;
				}
				break;

			case 2:
				// Reset the game
				clearGrid(grid);
				winner = 0;
				gameState = 0;
				playerTurn = 'X';
				emptySpaces = 9;
				break;
			}

			pressedButton = -1;
			if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){25, 15, 115, 105}))
				pressedButton = 1;

			// Drawing the main grid
			BeginDrawing();
			BeginTextureMode(screen);
			ClearBackground(WHITE);
			DrawRectangle(145, 145, (squareSize * 3 + gridSpace * 2) + 20, (squareSize * 3 + gridSpace * 2) + 20, BLACK); // 150, 20
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					if (grid[j * 3 + i] != ' ')
					{
						DrawTextureRec((grid[j * 3 + i] == 'X') ? X.texture : O.texture, (Rectangle){0, 0, squareSize, -squareSize}, (Vector2){i * ((float)squareSize + gridSpace) + 155, j * ((float)squareSize + gridSpace) + 155}, WHITE);
					}
					else
					{
						if (CheckCollisionPointRec((Vector2){(float)GetMouseX(), (float)GetMouseY()}, (Rectangle){i * ((float)squareSize + gridSpace) + 155, j * ((float)squareSize + gridSpace) + 155, (float)squareSize, (float)squareSize}))
						{
							DrawRectangle(i * (squareSize + gridSpace) + 155, j * (squareSize + gridSpace) + 155, squareSize, squareSize, LIGHTGRAY);
						}
						else
						{
							DrawRectangle(i * (squareSize + gridSpace) + 155, j * (squareSize + gridSpace) + 155, squareSize, squareSize, WHITE);
						}
					}
				}
			}

			// Home button
			DrawRectangleRounded((Rectangle){45, 45, 75, 75}, 0.2, 5, (pressedButton == 1) ? BLACK : GRAY);
			DrawTriangle((Vector2){25, 55}, (Vector2){140, 55}, (Vector2){82.5, 15}, (pressedButton == 1) ? BLACK : GRAY);

			switch (winner)
			{
			case 'X':
				DrawText("Player X wins!", 145, 675, 75, BLACK);
				break;
			case 'O':
				DrawText("Player O wins!", 145, 675, 75, BLACK);
				break;
			case '0':
				DrawText("Draw", 300, 675, 75, BLACK);
				break;
			}
			EndTextureMode();
			DrawTextureRec(screen.texture, (Rectangle){0, 0, screenWidth, -screenHeight}, (Vector2){0, 0}, WHITE);
			EndDrawing();

			// Exit to the home screen if the home button is pressed
			if ((pressedButton == 1) && IsMouseButtonPressed(0))
			{
				clearGrid(grid);
				mode = '0';
				winner = 0;
				gameState = 0;
				playerTurn = 'X';
				emptySpaces = 9;
				loopState = 0;
			}

			break;
		}
	}
// Used to exit program directly
EXIT:
	UnloadRenderTexture(screen);
	UnloadRenderTexture(X);
	UnloadRenderTexture(O);

	CloseWindow();

	return 0;
}

int mouseCollide(Vector2 mousePos, Vector2 gridPos, int squareSize, int gridSpace)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (CheckCollisionPointRec(mousePos, (Rectangle){i * (squareSize + gridSpace) + gridPos.x, j * (squareSize + gridSpace) + gridPos.y, (float)squareSize, (float)squareSize}))
			{
				return j * 3 + i;
			}
		}
	}

	return -1;
}

int checkButton(Rectangle buttons[3])
{
	Vector2 mousePos = GetMousePosition();
	for (int i = 0; i < 3; i++)
	{
		if (CheckCollisionPointRec(mousePos, buttons[i]))
			return i;
	}

	return -1;
}