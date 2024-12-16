#include <iostream>
#include <conio.h> // for kbhit and getch
#include <cstdlib> // For system("cls")
#include <windows.h> // for color
#include <ctime> // for time
#include <cstdlib> // for rand and srand
#include <fstream> // for file handling

using namespace std;
int score = 0;
int renemy = 3;
int lives = 3;
char grid[20][20];
int playerX = 1, playerY = 1; // Player's initial position

time_t currentTime = time(0);

// Color Codes
const string reset = "\033[0m";          // Reset color
const string red = "\033[31m";           // Red color
const string green = "\033[32m";         // Green color
const string yellow = "\033[33m";        // Yellow color
const string cyan = "\033[36m";          // Cyan color
const string magneta = "\033[35m";       // Magneta
const string blue = "\033[94m";          // Blue

void playerBombed();

void setConsoleCursor(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void intro()
{

    cout << "\n\n\n\n\n\n\n\n\n\n";
    cout << "\t\t\t\t\t" << cyan << "<(BOMBER MAN)>" << endl; // Title in Cyan
    cout << "\t\t\t\t\t" << green << "Controls:" << endl;
    cout << "\t\t\t\t\t" << green << "UP" << " : upper arrow" << endl;
    cout << "\t\t\t\t\t" << green << "DOWN" << " : down arrow" << endl;
    cout << "\t\t\t\t\t" << green << "LEFT" << " : left arrow" << endl;
    cout << "\t\t\t\t\t" << green << "RIGHT" << " : right arrow" << endl;
    cout << "\t\t\t\t\t" << green << "QUIT" << " : 'q'" << endl;  // Quit in Red
    cout << "\t\t\t\t\t" << red << "BOMB" << " : 'b'" << endl; // Bomb in Yellow
    cout << "\t\t\t\t\t" << magneta << "==================================" << endl;
    system("pause");
}
void setConsoleColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void drawGrid(char grid[20][20]) {
    setConsoleCursor(0, 0);
    cout << red << "Enemies: " << renemy << "     Score: " << score << "    Lives: " << lives << endl;  // For debugging

    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 20; j++)
        {
            if (grid[i][j] == 'O')
            {
                setConsoleColor(3); // blue for destructible obstacles
            }
            else if (grid[i][j] == '#') {
                setConsoleColor(11); // light blue for indestructible obstacles
            }
            else if (grid[i][j] == 'P') {
                setConsoleColor(5); // Purple for player
            }
            else if (grid[i][j] == 'E') {
                setConsoleColor(4); // Red for enemies
            }
            else if (grid[i][j] == 'B') {
                setConsoleColor(6); // Yellow for bomb
            }
            else if (grid[i][j] == 'X') {
                setConsoleColor(12); // Bright red for explosion
            }
            cout << grid[i][j] << ' ';
        }
        cout << endl;
    }
    setConsoleColor(7); // Reset to default color after drawing

}
void generate_Undes_Obstacles(char grid[20][20]) {
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            if (i % 2 != 0 && j % 2 != 0 && (i != 1 || j != 1) && (i != 1 || j != 2) && (i != 2 || j != 1) && (i != 2 || j != 2)) {
                grid[i][j] = '#'; // indestructible obstacle
            }
        }
    }
}
void generate_Des_Obstacles(char grid[20][20])
{
    for (int i = 0; i < 50; i++)
    {
        int x = rand() % 20;
        int y = rand() % 20;
        if (grid[x][y] == ' ' && (x != 1 || y != 1) && (x != 1 || y != 2) && (x != 2 || y != 1) && (x != 2 || y != 2)) { // empty space
            grid[x][y] = 'O'; // destructible obstacle
        }
    }
}
void gen_Enemy(char grid[20][20]) {
    for (int i = 0; i < renemy; i++) {
        int EnemyX, EnemyY;
        do {
            EnemyX = rand() % 20;
            EnemyY = rand() % 20;
        } while (grid[EnemyX][EnemyY] != ' '); // empty space
        grid[EnemyX][EnemyY] = 'E'; // enemy
    }
}
void releaseBomb(char grid[20][20], int playerX, int playerY, int& bombX, int& bombY, int& bombTimer)
{
    bombX = playerX;
    bombY = playerY;
    bombTimer = 30; // bomb will explode after 3  seconds, and clear after 3
    grid[bombX][bombY] = 'B'; // mark the bomb on the grid
}
void bombExplosion(char grid[20][20], int bombX, int bombY) {
    if (bombX == -1 || bombY == -1) {
        return;
    }
    if (grid[bombX][bombY] == 'P') playerBombed();
    grid[bombX][bombY] = 'X'; // mark the explosion at the bomb's location
    Beep(250, 500);
    // Up direction
    if (bombX - 1 > 0 && (grid[bombX - 1][bombY] == ' ' || grid[bombX - 1][bombY] == 'O')) {
        if (grid[bombX - 1][bombY] == 'O') score += 10; // Add score if 'O' is destroyed
        grid[bombX - 1][bombY] = 'X'; // mark the explosion one cell up
    }
    else if (bombX - 1 > 0 && grid[bombX - 1][bombY] == 'E') {
        grid[bombX - 1][bombY] = 'X'; // mark the explosion one cell up
        renemy--;
        score += 50; //50 score + enemy
    }
    else if (bombX - 1 > 0 && grid[bombX - 1][bombY] == 'P')
    {
        grid[bombX - 1][bombY] = 'X';
        playerBombed();
    }

    // Down direction
    if (bombX + 1 < 20 && (grid[bombX + 1][bombY] == ' ' || grid[bombX + 1][bombY] == 'O')) {
        if (grid[bombX + 1][bombY] == 'O') score += 10; // Add score if 'O' is destroyed
        grid[bombX + 1][bombY] = 'X'; // mark the explosion one cell down
    }
    else if (bombX + 1 < 20 && grid[bombX + 1][bombY] == 'E') {
        grid[bombX + 1][bombY] = 'X'; // mark the explosion one cell down
        renemy--;
        score += 50; //50 score + enemy
    }
    else if (bombX + 1 > 0 && grid[bombX + 1][bombY] == 'P')
    {
        grid[bombX + 1][bombY] = 'X';
        playerBombed();
    }

    // Right direction
    if (bombY + 1 < 20 && (grid[bombX][bombY + 1] == ' ' || grid[bombX][bombY + 1] == 'O')) {
        if (grid[bombX][bombY + 1] == 'O') score += 10; // Add score if 'O' is destroyed
        grid[bombX][bombY + 1] = 'X'; // mark the explosion one cell right
    }
    else if (bombY + 1 < 20 && grid[bombX][bombY + 1] == 'E') {
        grid[bombX][bombY + 1] = 'X'; // mark the explosion one cell right
        renemy--;
        score += 50; //50 score + enemy
    }
    else if (bombY + 1 < 20 && grid[bombX][bombY + 1] == 'P')
    {
        grid[bombX][bombY + 1] = 'X';
        playerBombed();
    }

    // Left direction
    if (bombY - 1 > 0 && (grid[bombX][bombY - 1] == ' ' || grid[bombX][bombY - 1] == 'O')) {
        if (grid[bombX][bombY - 1] == 'O') score += 10; // Add score if 'O' is destroyed
        grid[bombX][bombY - 1] = 'X'; // mark the explosion one cell left
    }
    else if (bombY - 1 > 0 && grid[bombX][bombY - 1] == 'E') {
        grid[bombX][bombY - 1] = 'X'; // mark the explosion one cell left
        renemy--;
        score += 50; //50 score + enemy
    }
    else if (bombY - 1 < 20 && grid[bombX][bombY - 1] == 'P')
    {
        grid[bombX][bombY - 1] = 'X';
        playerBombed();
    }

    // Reset bomb position after explosion
    bombX = -1;
    bombY = -1;
}

void checkAndUpdateHighScores(int score) {
    int highScores[3] = { 0, 0, 0 }; // Array to store top 3 scores
    ifstream inputFile("highscore.txt");

    // Read existing high scores from the file
    if (inputFile.is_open()) {
        for (int i = 0; i < 3; ++i) {
            if (!(inputFile >> highScores[i])) {
                highScores[i] = 0; // Default to 0 if less than 3 scores
            }
        }
        inputFile.close();
    }

    // Add the current score and sort manually
    for (int i = 0; i < 3; ++i) {
        if (score > highScores[i]) {
            for (int j = 2; j > i; --j) {
                highScores[j] = highScores[j - 1];
            }
            highScores[i] = score;
            break;
        }
    }

    // Write updated high scores back to the file
    ofstream outputFile("highscore.txt");
    if (outputFile.is_open()) {
        for (int i = 0; i < 3; ++i) {
            outputFile << highScores[i] << endl;
        }
        outputFile.close();
    }

    // Display high scores and rank message
    cout << "High Scores:\n";
    for (int i = 0; i < 3; ++i) {
        cout << (i + 1) << ": " << highScores[i] << endl;
    }

    if (score == highScores[0]) {
        cout << yellow << "Congratulations! You got the 1st high score!\n";
    }
    else if (score == highScores[1]) {
        cout << yellow << "Great! You got the 2nd high score!\n";
    }
    else if (score == highScores[2]) {
        cout << yellow << "Nice! You got the 3rd high score!\n";
    }
    system("pause");
    system("cls");
}
void bombClear(char grid[20][20], int bombX, int bombY)
{
    grid[bombX][bombY] = ' ';
    for (int up = 1; up <= 1; up++)
        if (grid[bombX - up][bombY] == 'X')
            grid[bombX - up][bombY] = ' ';
    for (int down = 1; down <= 1; down++)
        if (grid[bombX + down][bombY] == 'X')
            grid[bombX + down][bombY] = ' ';
    for (int right = 1; right <= 1; right++)
        if (grid[bombX][bombY + right] == 'X')
            grid[bombX][bombY + right] = ' ';
    for (int left = 1; left <= 1; left++)
        if (grid[bombX][bombY - left] == 'X')
            grid[bombX][bombY - left] = ' ';
}
void playerKilled()
{
    if (lives <= 0)
    {
        cout << yellow << "Game Over! You ran out of lives.\n";
        checkAndUpdateHighScores(score);
        system("pause");
        exit(0);
    }
}
void playerBombed()
{
    cout << yellow << "You stepped on a bomb! \n";
    cout << yellow << "Remaining lives: " << --lives << endl;
    playerKilled();
    system("pause");
    system("cls");
}
void playerPunched(int a, int b)
{
    cout << yellow << "You collided with an enemy.\n";
    cout << yellow << "Remaining lives: " << --lives << endl;
    grid[a][b] = ' ';
    playerX = 1, playerY = 1;
    grid[playerX][playerY] = 'P';

    playerKilled();
    system("pause");
    system("cls");
}
void playerMovement(char grid[20][20], int& playerX, int& playerY, int bombX, int bombY)
{
    char direction = _getch(); // Get input from the player

    // Clear player's current position from the grid
    if (grid[playerX][playerY] != 'B') grid[playerX][playerY] = ' ';

    // Move based on input direction
    switch (direction)
    {
    case 72: // UP arrow key
        if (grid[playerX - 1][playerY] == ' ') {
            playerX--;
        }
        else if (grid[playerX - 1][playerY] == 'X')
        {
            playerBombed();
        }
        else if (grid[playerX - 1][playerY] == 'E')
        {
            playerPunched(playerX, playerY);
        }
        break;
    case 80: // DOWN arrow key
        if (grid[playerX + 1][playerY] == ' ') {
            playerX++;
        }
        else if (grid[playerX + 1][playerY] == 'X')
        {
            playerBombed();
        }
        else if (grid[playerX + 1][playerY] == 'E')
        {
            playerPunched(playerX, playerY);
        }
        break;
    case 75: // LEFT arrow key
        if (grid[playerX][playerY - 1] == ' ') {
            playerY--;
        }
        else if (grid[playerX][playerY - 1] == 'X')
        {
            playerBombed();
        }
        else if (grid[playerX][playerY - 1] == 'E')
        {
            playerPunched(playerX, playerY);
        }
        break;
    case 77: // RIGHT arrow key
        if (grid[playerX][playerY + 1] == ' ') {
            playerY++;
        }
        else if (grid[playerX][playerY + 1] == 'X')
        {
            playerBombed();
        }
        else if (grid[playerX][playerY + 1] == 'E')
        {
            playerPunched(playerX, playerY);
        }
        break;
    case 'q': // Quit the game
        cout << yellow << "Game Quit!\n";
        checkAndUpdateHighScores(score);
        exit(0);
    }

    grid[playerX][playerY] = 'P';

}
void enemyMovement(char grid[20][20])
{
    time_t enemyTime = time(0);

    if (enemyTime - currentTime >= 1)
    {
        currentTime = time(0);
        // Iterate through the grid to find all enemies
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++)
            {
                if (grid[i][j] == 'E')
                { // Found an enemy at position (i, j)
                    grid[i][j] = ' '; // Clear the current enemy position
                    // Randomly move the enemy to an adjacent empty space
                    int moveDirection = rand() % 4; // Random number between 0 and 3 to choose a direction
                    switch (moveDirection) {
                    case 0: // Move up
                        if (i > 0 && grid[i - 1][j] == ' ')
                        {
                            i--; // Move the enemy up
                        }
                        else if (grid[i - 1][j] == 'P') {
                            cout << yellow << "You collided with an enemy.\n";
                            playerPunched(playerX, playerY);
                        }
                        else if (grid[i - 1][j] == 'X') {
                            grid[i][j] = ' ';
                            renemy--;
                        }
                        break;
                    case 1: // Move down
                        if (i < 19 && grid[i + 1][j] == ' ') {
                            i++; // Move the enemy down
                        }
                        else if (grid[i + 1][j] == 'P') {
                            cout << yellow << "You collided with an enemy.\n";
                            playerPunched(playerX, playerY);
                        }
                        else if (grid[i + 1][j] == 'X') {
                            grid[i][j] = ' ';
                            renemy--;
                        }
                        break;
                    case 2: // Move left
                        if (j > 0 && (grid[i][j - 1] == ' ')) {
                            j--; // Move the enemy left
                        }
                        else if (grid[i][j - 1] == 'P') {
                            cout << yellow << "You collided with an enemy.\n";
                            playerPunched(playerX, playerY);
                        }
                        else if (grid[i][j - 1] == 'X') {
                            grid[i][j] = ' ';
                            renemy--;
                        }
                        break;
                    case 3: // Move right
                        if (j < 19 && grid[i][j + 1] == ' ') {
                            j++; // Move the enemy right
                        }
                        else if (grid[i][j + 1] == 'P') {
                            cout << yellow << "You collided with an enemy.\n";
                            playerPunched(playerX, playerY);
                        }
                        else if (grid[i][j + 1] == 'X') {
                            grid[i][j] = ' ';
                            renemy--;
                        }
                        break;
                    }
                    // Update the new position of the enemy
                    grid[i][j] = 'E';
                }
            }
        }
    }
}

int main()
{
    srand(time(0)); // Seed for random number generation
    intro();
    system("cls");


    //char grid[20][20];
    //int playerX = 1, playerY = 1; // Player's initial position

    // Function to initialize grid (reuse code)
    auto initializeGrid = [&]() {
        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (i == 0 || i == 19 || j == 0 || j == 19) {
                    grid[i][j] = '#'; // Wall
                }
                else {
                    grid[i][j] = ' '; // Empty space
                }
            }
        }
        grid[playerX][playerY] = 'P'; // Player's position
        };

    // First stage
    initializeGrid();
    generate_Undes_Obstacles(grid);
    generate_Des_Obstacles(grid);
    gen_Enemy(grid);
    int bombX = -1, bombY = -1;
    int bombTimer = 0;
    currentTime = time(0);

    // First stage game loop
    while (true)
    {
        if (_kbhit())
        {
            char direction = _getch();
            if (direction == 'b')
            {
                releaseBomb(grid, playerX, playerY, bombX, bombY, bombTimer);
            }
            else {
                playerMovement(grid, playerX, playerY, bombX, bombY);
            }
            drawGrid(grid);
        }
        if (bombTimer > 0) {
            bombTimer--;
            if (bombTimer == 3) {
                bombExplosion(grid, bombX, bombY);
                drawGrid(grid);
            }
            else if (bombTimer == 0) {
                bombClear(grid, bombX, bombY);
                drawGrid(grid);
            }
        }
        enemyMovement(grid);
        drawGrid(grid);
        // Sleep(100); // Control game speed

        // Check if all enemies are killed
        if (renemy == 0) {
            cout << blue << "Stage 1 Complete! Moving to Stage 2..." << endl;
            checkAndUpdateHighScores(score);

            break; // Exit first stage loop
        }
    }

    // Second stage setup
    renemy = 5; // Increase enemy count
    initializeGrid(); // Clear and reset the grid
    generate_Undes_Obstacles(grid);
    generate_Des_Obstacles(grid);
    gen_Enemy(grid); // Generate more enemies for stage 2

    // Second stage game loop
    while (true) {
        if (_kbhit()) {
            char direction = _getch();
            if (direction == 'b') {
                releaseBomb(grid, playerX, playerY, bombX, bombY, bombTimer);
            }
            else {
                playerMovement(grid, playerX, playerY, bombX, bombY);
            }
            drawGrid(grid);
        }
        if (bombTimer > 0) {
            bombTimer--;
            if (bombTimer == 5) {
                bombExplosion(grid, bombX, bombY);
                drawGrid(grid);
            }
            else if (bombTimer == 0) {
                bombClear(grid, bombX, bombY);
                drawGrid(grid);
            }
        }
        enemyMovement(grid);
        drawGrid(grid);
        // Sleep(30); // Faster gameplay for added difficulty

        // Check if all enemies are killed
        if (renemy == 0) {
            cout << blue << "Congratulations! You completed Stage 2!" << endl;
            checkAndUpdateHighScores(score);

            exit(0);

        }
    }
    return 0;
}