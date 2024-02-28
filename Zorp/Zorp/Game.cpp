#include "Game.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <time.h>


Game::Game()
    : m_gameOver{false}
{
}

Game::~Game()
{
}

bool Game::startup()
{
	if (enableVirtualTerminal() == false)
	{
		std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
		std::cout << "Press 'Enter' to exit." << std::endl;
		std::cin.get();
		return false;
	}
	initialiseMap();

	m_player.setPosition(Point2D{ 0, 0 });

	drawWelcomeMessage();

	return true;
}

void Game::update()
{
    Point2D playerPos = m_player.getPosition();

    if (m_map[playerPos.y][playerPos.x].getType() == EXIT)
    {
        m_gameOver = true;
        return;
    }
    int command = getCommand();

    if (m_player.executeCommand(command, &m_map[playerPos.y][playerPos.x]))
        return;

    m_map[playerPos.y][playerPos.x].executeCommand(command);
}

void Game::draw()
{
    Point2D playerPos = m_player.getPosition();

    // draw the description of the current room
    m_map[playerPos.y][playerPos.x].drawDescription();

    //list the directions the player can take
    drawValidDirections();

    // Redraw the map
    drawMap();
    // Draw the player on the map
    m_player.draw();
}

bool Game::isGameOver()
{
	return m_gameOver;
}


bool Game::enableVirtualTerminal()
{
    // Set output more to handle virtual terminal sequences
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
    {
        return false;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
    {
        return false;
    }
    return true;
}

void Game::initialiseMap()
{
    srand(time(nullptr));

    // Fill the Arrays with random room types
    for (int y = 0; y < MAZE_HEIGHT; y++)
    {
        for (int x = 0; x < MAZE_WIDTH; x++)
        {
            int type = rand() % (MAX_RANDOM_TYPE * 2);
            if (type < MAX_RANDOM_TYPE)
            {
                if (type == TREASURE)
                    type = rand() % 3 + TREASURE_HP;
                m_map[y][x].setType(type);
            }
            else
                m_map[y][x].setType(EMPTY);
            m_map[y][x].setPosition(Point2D{ x, y });
        }
    }
    // Set the entrance and exit of the Maze
    m_map[0][0].setType(ENTRANCE);
    m_map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1].setType(EXIT);
}

void Game::drawWelcomeMessage()
{
    std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << std::endl;
    std::cout << INDENT << "ZORP is a game of Adventure, danger and low cunning" << std::endl;
    std::cout << INDENT << "Most CERTAINLY not a shameless rip of a much more popular game." << std::endl << std::endl;
}

void Game::drawMap()
{
    Point2D position = { 0, 0 };
    // reset draw colors
    std::cout << RESET_COLOR;
    for (position.y = 0; position.y < MAZE_HEIGHT; position.y++)
    {
        for (position.x = 0; position.x < MAZE_WIDTH; position.x++)
        {
            m_map[position.y][position.x].draw();
        }
        std::cout << std::endl;
    }
}

void Game::drawValidDirections()
{
    Point2D position = m_player.getPosition();

    // Reset draw colours
    std::cout << RESET_COLOR;
    // Jump to the correct Location
    std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
    // List the Directions the player can move
    std::cout << INDENT << "You can see paths leading to the " <<
        ((position.y > 0) ? "north, " : "") <<
        ((position.x < MAZE_WIDTH - 1) ? "east, " : "") <<
        ((position.x > 0) ? "west, " : "") <<
        ((position.y < MAZE_HEIGHT - 1) ? "south, " : "") << std::endl;
}

int Game::getCommand()
{
    char input[50] = "\0";      // Input set to 50 characters for input arguments.

    // Jump to correct location.
    std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
    // clear any existing text
    std::cout << CSI << "4M";
    // insert 4 lines, for inventory

    std::cout << INDENT << "Enter a command.";
    // Move cursor to position for player to enter input
    std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

    // Clear the input buffer, ready for player input
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());

    std::cin >> input;
    std::cout << RESET_COLOR;

    bool bMove = false;
    bool bPickup = false;
    while (input) {
        // Check for command keyword.
        if (strcmp(input, "move") == 0)
        {
            bMove = true;
        }
        else if (bMove == true) {
            if (strcmp(input, "north") == 0)
                return NORTH;
            if (strcmp(input, "east") == 0)
                return EAST;
            if (strcmp(input, "south") == 0)
                return SOUTH;
            if (strcmp(input, "west") == 0)
                return WEST;
        }

        if (strcmp(input, "look") == 0)
        {
            return LOOK;
        }
        if (strcmp(input, "fight") == 0) {
            return FIGHT;
        }
        if (strcmp(input, "pick") == 0) {
            bPickup = true;
        }
        else if (bPickup == true) {
            if (strcmp(input, "up") == 0) {
                return PICKUP;
            }
        }
        if (strcmp(input, "consume") == 0) {
            return CONSUME;
        }
        if ((strcmp(input, "die") == 0) || (strcmp(input, "suicide") == 0)) {
            return SUICIDE;
        }
        char next = std::cin.peek();
        if (next == '\n' || next == EOF)
            break;
        std::cin >> input;
    }
    return 0;
}
