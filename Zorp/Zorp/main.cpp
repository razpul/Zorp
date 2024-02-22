#include <iostream>
#include <windows.h>
#include <random>
#include <time.h>

const char* ESC = "\x1b";
const char* CSI = "\x1b[";
const char* TITLE = "\x1b[5;20H";
const char* INDENT = "\x1b[5C";

const char* YELLOW = "\x1b[93m";
const char* MAGENTA = "\x1b[95m";
const char* RED = "\x1b[91m";
const char* BLUE = "\x1b[94m";
const char* WHITE = "\x1b[97m";
const char* GREEN = "\x1b[92m";

const char* RESET_COLOR = "\x1b[0m";


const int EMPTY = 0;
const int ENEMY = 1;
const int TREASURE = 2;
const int FOOD = 3;
const int ENTRANCE = 4;
const int EXIT = 5;
const int MAX_RANDOM_TYPE = FOOD + 1;
const int MAZE_WIDTH = 10;
const int MAZE_HEIGHT = 6;
const int INDENT_X = 5;
const int ROOM_DESC_Y = 8; 
const int MOVEMENT_DESC_Y = 9;
const int MAP_Y = 13;

const char* EXTRA_OUTPUT_POS = "\x1b[25;6H";

const int PLAYER_INPUT_X = 30;
const int PLAYER_INPUT_Y = 23;

const int WEST = 4;
const int EAST = 6;
const int NORTH = 8;
const int SOUTH = 2;

const int LOOK = 9;
const int FIGHT = 10;

struct Point2D
{
	int x;
	int y;
};

bool enableVirtualTerminal()
{
    // Set output mode to handle virtual terminal sequences
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

void initialize(int map[MAZE_HEIGHT][MAZE_WIDTH])
{
    srand(time(nullptr));
    // fill the arrays with random room types
    for (int y = 0; y < MAZE_HEIGHT; y++)
    {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            int type = rand() % (MAX_RANDOM_TYPE * 2);
            if (type < MAX_RANDOM_TYPE)
                map[y][x] = type;
            else
                map[y][x] = EMPTY;
        }
    }
    // set the entrance and exit of the maze
    map[0][0] = ENTRANCE;
    map[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;
}

void drawWelcomeMessage()
{
    std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR
        << std::endl;
    std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
    std::cout << INDENT << "It is definitely not related to any other text - based adventure game." << std::endl << std::endl;
}

void drawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH],Point2D position) {
    // find the console output position
    int outX = INDENT_X + (6 * position.x) + 1;
    int outY = MAP_Y + position.y;
    // jump to the correct location
    std::cout << CSI << outY << ";" << outX << "H";
    // draw the room
    switch (map[position.y][position.x]) {
    case EMPTY:
        std::cout << "[ " << GREEN << "\xb0" << RESET_COLOR << " ] ";
        break;
    case ENEMY:
        std::cout << "[ " << RED << "\x94" << RESET_COLOR << " ] ";
        break;
    case TREASURE:
        std::cout << "[ " << YELLOW << "$" << RESET_COLOR << " ] ";
        break;
    case FOOD:
        std::cout << "[ " << WHITE << "\xcf" << RESET_COLOR << " ] ";
        break;
    case ENTRANCE:
        std::cout << "[ " << WHITE << "\x9d" << RESET_COLOR << " ] ";
        break;
    case EXIT:
        std::cout << "[ " << WHITE << "\xFE" << RESET_COLOR << " ] ";
        break;
    }
}

void drawMap(int map[MAZE_HEIGHT][MAZE_WIDTH]) 
{
    Point2D position = { 0, 0 };
    // reset draw colors
    std::cout << RESET_COLOR; for (position.y = 0; position.y < MAZE_HEIGHT; position.y++) 
    {
        std::cout << INDENT; for (position.x = 0; position.x < MAZE_WIDTH; position.x++) 
        {
            drawRoom(map, position);
        }
        std::cout << std::endl;
    }
}

void drawRoomDescription(int roomType)
{
    // reset draw colors
    std::cout << RESET_COLOR;
    // jump to the correct location
    std::cout << CSI << ROOM_DESC_Y << ";" << 0 << "H";
    // Delete 4 lines and insert 4 empty lines
    std::cout << CSI << "4M" << CSI << "4L" << std::endl;
    // write description of current room
    switch (roomType) {
    case EMPTY:
        std::cout << INDENT << "You are in an empty meadow. There is nothing of note here." << std::endl;
        break;
    case ENEMY:
        std::cout << INDENT << RED << "BEWARE." << RESET_COLOR << " An enemy is approaching." << std::endl;
        break;
    case TREASURE:
        std::cout << INDENT << "Your journey has been rewarded. You have found some treasure." << std::endl;
        break;
    case FOOD:
        std::cout << INDENT << "At last! You collect some food to sustain you on your journey." << std::endl;
        break;
    case ENTRANCE:
        std::cout << INDENT << "The entrance you used to enter this maze is blocked.There is no going back." << std::endl;
        break;
    case EXIT:
        std::cout << INDENT << "Despite all odds, you made it to the exit Congratulations." << std::endl;
        break;
    }
}

void drawPlayer(Point2D position)
{
    position.x = INDENT_X + (6 * position.x) + 3;
    position.y = MAP_Y + position.y;
    // draw the player's position on the map
    // move cursor to map pos and delete character at current position
    std::cout << CSI << position.y << ";" << position.x << "H";
    std::cout << MAGENTA << "\x81" << RESET_COLOR;
}

void drawValidDirections(Point2D position) {
    // reset draw colors
    std::cout << RESET_COLOR;
    // jump to the correct location
    std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
    std::cout << INDENT << "You can see paths leading to the " <<
        ((position.x > 0) ? "west, " : "") <<
        ((position.x < MAZE_WIDTH - 1) ? "east, " : "") <<
        ((position.y > 0) ? "north, " : "") <<
        ((position.y < MAZE_HEIGHT - 1) ? "south, " : "") << std::endl;
}

int getCommand()
{
    // for now, we can't read commands longer than 50 characters
    char input[50] = "\0";
    // jump to the correct location
    std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
    // clear any existing text
    std::cout << CSI << "4M";
    std::cout << INDENT << "Enter a command.";
    // move cursor to position for player to enter input
    std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;
    // clear the input buffer, ready for player input
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin >> input;
    std::cout << RESET_COLOR;
    bool bMove = false;
    while (input) {
        if (strcmp(input, "move") == 0) {
            bMove = true;
        }
        else if (bMove == true) {
            if (strcmp(input, "north") == 0)
                return NORTH;
            if (strcmp(input, "south") == 0)
                return SOUTH;
            if (strcmp(input, "east") == 0)
                return EAST;
            if (strcmp(input, "west") == 0)
                return WEST;
        }
        if (strcmp(input, "look") == 0) {
            return LOOK;
        }
        if (strcmp(input, "fight") == 0) {
            return FIGHT;
        }
        char next = std::cin.peek();
        if (next == '\n' || next == EOF)
            break;
        std::cin >> input;
    }
    return 0;
}

void main() 
{
    // create a 2D array
    int rooms[MAZE_HEIGHT][MAZE_WIDTH];
    bool gameOver = false;
 
    Point2D player = { 0, 0 };
     
    if (enableVirtualTerminal() == false) {
        std::cout << "The virtual terminal processing mode could not be activated." << std::endl;
            std::cout << "Press 'Enter' to exit." << std::endl;
        std::cin.get();
        return;
    }
    initialize(rooms);

    drawWelcomeMessage();

    // output the map
    drawMap(rooms);

    // game loop
    while (!gameOver)
    {
        drawRoomDescription(rooms[player.y][player.x]);
        drawPlayer(player);
        if (rooms[player.y][player.x] == EXIT) {
            gameOver = true;
            continue;
        }
        // list the directions the player can take
        drawValidDirections(player);
        int command = getCommand();
        // before updating the player position, redraw the old room
        // character over the old position
        drawRoom(rooms, player);
        // update the player's position using the input movement data
        switch (command) {
        case EAST:
            if (player.x < MAZE_WIDTH - 1)
                player.x++;
            break;
        case WEST:
            if (player.x > 0)
                player.x--;
            break;
        case NORTH:
            if (player.y > 0)
                player.y--;
            break;
        case SOUTH:
            if (player.y < MAZE_HEIGHT - 1)
                player.y++;
            break;
       case LOOK:
            drawPlayer(player);
            std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You look around, but see nothing worth mentioning" << std::endl;
            std::cout << INDENT << "Press 'Enter' to continue.";
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();
            break;
        case FIGHT:
            drawPlayer(player);
            std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You could try to fight, but you don't have a weapon" << std::endl;
            std::cout << INDENT << "Press 'Enter' to continue.";
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();
            break;
        default:
            // the direction was not valid,
            // do nothing, go back to the top of the loop and ask again
            drawPlayer(player);
            std::cout << EXTRA_OUTPUT_POS << RESET_COLOR << "You try, but you just can't do it." << std::endl;
            std::cout << INDENT << "Press 'Enter' to continue.";
            std::cin.clear();
            std::cin.ignore(std::cin.rdbuf()->in_avail());
            std::cin.get();
            break;      
        }
    } // end game loop
    // jump to the correct location
    std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
    std::cout << std::endl << INDENT << "Press 'Enter' to exit the program.";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();
}