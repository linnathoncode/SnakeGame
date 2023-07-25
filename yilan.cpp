#include <iostream>
#include <stdlib.h>
#include <windows.h>
#include <unistd.h>
#include <conio.h>
#include <ctime>
#include <chrono> 


using namespace std;

// Global variables
bool gameOver;
const int width = 20;
const int height = 10;
int x, y, xFruit, yFruit, score;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

const int maxSnakeLength = 100;
int xSnakeBody[maxSnakeLength];
int ySnakeBody[maxSnakeLength];
int snakeLength = 1;

// Function to generate a new fruit at a random location
void fruitGenerate() {
    xFruit = rand() % width;
    yFruit = rand() % height;
}

// Setup the game variables and initial state
void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitGenerate();
}

// Function to draw the game screen
void Draw() {
    system("cls");

    // Draw the top border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Draw the game area
    for (int j = 0; j < height + 1; j++) {
        for (int i = 0; i < width + 2; i++) {
            // Draw the left and right border
            if (i == 0 || i == width + 1)
                cout << "#";
            // Draw the snake's head
            else if (j == y && i == x)
                cout << 'O';
            // Draw the fruit
            else if (j == yFruit && i == xFruit)
                cout << 'F';
            // Draw the snake's body
            else {
                bool printSnakeBody = false;
                for (int k = 0; k < snakeLength; k++) {
                    if (xSnakeBody[k] == i && ySnakeBody[k] == j) {
                        cout << "o";
                        printSnakeBody = true;
                    }
                }
                if (!printSnakeBody)
                    cout << " ";
            }
        }
        cout << endl;
    }

    // Draw the bottom border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    // Display the score
    cout << "Score:" << score << endl;
}

// Function to handle user input
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'W':
		case 'w':
            dir = UP;
            break;
        case 'A':
		case 'a':
            dir = LEFT;
            break;
        case 'S':
		case 's':
            dir = DOWN;
            break;
        case 'D':
		case 'd':
            dir = RIGHT;
            break;
        case 'X':
		case 'x':
            gameOver = true;
            break;
        }
    }
}

// Function to update the game logic
void Logic() {
    // Move the snake's body
    for (int i = snakeLength - 1; i > 0; i--) {
        xSnakeBody[i] = xSnakeBody[i - 1];
        ySnakeBody[i] = ySnakeBody[i - 1];
    }

    // Update the snake's head position based on the direction
    switch (dir) {
    case UP:
        y--;
        break;
    case LEFT:
        x--;
        break;
    case DOWN:
        y++;
        break;
    case RIGHT:
        x++;
        break;
    }

    // Check if the snake collides with the boundaries or itself
    if (x >= width || x < 0 || y >= height || y < 0)
        gameOver = true;

    for (int i = 0; i < snakeLength; i++) {
        if (xSnakeBody[i] == x && ySnakeBody[i] == y)
            gameOver = true;
    }

    // Check if the snake eats the fruit
    if (x == xFruit && y == yFruit) {
        score += 10;
        snakeLength++;
        fruitGenerate();
    }

    // Update the snake's body position
    xSnakeBody[0] = x;
    ySnakeBody[0] = y;
}

int main() {
	
    Setup();

    const double frameRate = 1.0 / 2.0; // Set your desired frame rate (e.g., 5 frames per second)
    auto prevTime = std::chrono::high_resolution_clock::now();
    double accumulator = 0.0;

    while (!gameOver) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> deltaTime = currentTime - prevTime;
        prevTime = currentTime;
        accumulator += deltaTime.count();

        // Process input
        Input();

        while (accumulator >= frameRate) {
            // Update game logic
            Logic();

            // Reduce accumulator by the frame rate to maintain a smooth game loop
            accumulator -= frameRate;
        }

        // Draw the game
        Draw();

        // Optional: Add a custom delay to avoid unnecessary CPU usage
        Sleep(20); // 10 milliseconds (adjust as needed)
    }

    return 0;
}
