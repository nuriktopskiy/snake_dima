#include <conio.h>
#include <iostream>
#include <windows.h>
using namespace std;

// Размер игровой зоны
const int gameWidth = 80;
const int gameHeight = 20;

// Игровые переменные
struct GameEntity {
    int posX, posY;
} player, food;

// Служебные переменные
struct GameState {
    int score;
    int tailSize;
    int tailX[100], tailY[100];
    bool gameEnded;
    enum MoveDirection { NONE = 0, WEST, EAST, NORTH, SOUTH } direction;
} game;

// Инициализация игровых компонентов
void SetupGame()
{
    game.gameEnded = false;
    game.direction = GameState::NONE;
    player.posX = gameWidth / 2;
    player.posY = gameHeight / 2;
    food.posX = rand() % gameWidth;
    food.posY = rand() % gameHeight;
    game.score = 0;
    game.tailSize = 0;
}

// Рендер игрового интерфейса
void DisplayGame(string username)
{
    system("cls"); // Чистим консоль

    // Верхняя граница
    cout << "+";
    for (int i = 0; i < gameWidth; i++)
        cout << "-";
    cout << "+" << endl;

    // Игровая зона
    for (int i = 0; i < gameHeight; i++) {
        cout << "|"; // левая граница

        for (int j = 0; j < gameWidth; j++) {
            // рисуем игрока
            if (i == player.posY && j == player.posX) {
                cout << "@";
            }
            // рисуем еду
            else if (i == food.posY && j == food.posX) {
                cout << "*";
            }
            // рисуем хвост змеи
            else {
                bool drewSegment = false;
                for (int k = 0; k < game.tailSize; k++) {
                    if (game.tailX[k] == j && game.tailY[k] == i) {
                        cout << "o";
                        drewSegment = true;
                        break;
                    }
                }
                if (!drewSegment) cout << " ";
            }
        }

        cout << "|" << endl; // правая граница
    }

    // нижняя граница
    cout << "+";
    for (int i = 0; i < gameWidth; i++)
        cout << "-";
    cout << "+" << endl;

    // Игровая статистика
    cout << username << "'s Game | Points: " << game.score
        << " | Tail: " << game.tailSize << endl;
}

// Обновить состояние игры
void ProcessGame()
{
    int prevX = game.tailX[0];
    int prevY = game.tailY[0];
    game.tailX[0] = player.posX;
    game.tailY[0] = player.posY;

    for (int i = 1; i < game.tailSize; i++) {
        int tempX = game.tailX[i];
        int tempY = game.tailY[i];
        game.tailX[i] = prevX;
        game.tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    switch (game.direction) {
    case GameState::WEST:  player.posX--; break;
    case GameState::EAST:  player.posX++; break;
    case GameState::NORTH: player.posY--; break;
    case GameState::SOUTH: player.posY++; break;
    default: break;
    }

    if (player.posX < 0 || player.posX >= gameWidth ||
        player.posY < 0 || player.posY >= gameHeight) {
        game.gameEnded = true;
    }

    for (int i = 0; i < game.tailSize; i++) {
        if (game.tailX[i] == player.posX && game.tailY[i] == player.posY) {
            game.gameEnded = true;
        }
    }

    if (player.posX == food.posX && player.posY == food.posY) {
        game.score += 5;
        food.posX = rand() % gameWidth;
        food.posY = rand() % gameHeight;
        game.tailSize++;
    }
}

// Конфигурация игровых настроек
int ConfigureGame()
{
    int speed, choice;
    cout << "\nGAME SETTINGS\n1: Beginner\n2: Intermediate\n3: Expert\n"
        "Select option (default Intermediate): ";
    cin >> choice;

    switch (choice) {
    case 1: speed = 70; break;
    case 3: speed = 30; break;
    default: speed = 50;
    }
    return speed;
}

// Отслеживание нажатия кнопок
void CheckInput()
{
    if (_kbhit()) {
        switch (tolower(_getch())) {
        case 'a': game.direction = GameState::WEST; break;
        case 'd': game.direction = GameState::EAST; break;
        case 'w': game.direction = GameState::NORTH; break;
        case 's': game.direction = GameState::SOUTH; break;
        case 'q': game.gameEnded = true; break;
        }
    }
}


int main()
{
    string playerName;
    cout << "Enter player name: ";
    cin >> playerName;

    int gameSpeed = ConfigureGame();
    SetupGame();

    while (!game.gameEnded) {
        DisplayGame(playerName);
        CheckInput();
        ProcessGame();
        Sleep(gameSpeed);
    }

    cout << "\nGame Over! Final Score: " << game.score << endl;
    system("pause");
    return 0;
}