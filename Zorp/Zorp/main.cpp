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
const int PLAYER_INPUT_X = 30;
const int PLAYER_INPUT_Y = 11;
const int WEST = 4;
const int EAST = 6;
const int NORTH = 8;
const int SOUTH = 2;

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

void drawRoom(int map[MAZE_HEIGHT][MAZE_WIDTH], int x, int y) {
    // find the console output position
    int outX = INDENT_X + (6 * x) + 1;
    int outY = MAP_Y + y;
    // jump to the correct location
    std::cout << CSI << outY << ";" << outX << "H";
    // draw the room
    switch (map[y][x]) {
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
    // reset draw colors
    std::cout << RESET_COLOR;
    for (int y = 0; y < MAZE_HEIGHT; y++)
    {
        std::cout << INDENT;
        for (int x = 0; x < MAZE_WIDTH; x++) {
            drawRoom(map, x, y);
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

void drawPlayer(int x, int y)
{
    x = INDENT_X + (6 * x) + 3;
    y = MAP_Y + y;
    // draw the player's position on the map
    // move cursor to map pos and delete character at current position
    std::cout << CSI << y << ";" << x << "H";
    std::cout << MAGENTA << "\x81" << RESET_COLOR;
}

void drawValidDirections(int x, int y) {
    // reset draw colors
    std::cout << RESET_COLOR;
    // jump to the correct location
    std::cout << CSI << MOVEMENT_DESC_Y + 1 << ";" << 0 << "H";
    std::cout << INDENT << "You can see paths leading to the " <<
        ((x > 0) ? "west, " : "") <<
        ((x < MAZE_WIDTH - 1) ? "east, " : "") <<
        ((y > 0) ? "north, " : "") <<
        ((y < MAZE_HEIGHT - 1) ? "south, " : "") << std::endl;
}

int getMovementDirection()
{
    // jump to the correct location
    std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
    std::cout << INDENT << "Where to now?";
    int direction;
    // move cursor to position for player to enter input
    std::cout << CSI << PLAYER_INPUT_Y << ";" << PLAYER_INPUT_X << "H" << YELLOW;
    // clear the input buffer, ready for player input
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin >> direction;
    std::cout << RESET_COLOR;
    if (std::cin.fail())
        return 0;
    return direction;
}



void main() 
{

    // create a 2D array
    int rooms[MAZE_HEIGHT][MAZE_WIDTH];
    bool gameOver = false;
    int playerX = 0;
    int playerY = 0;

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
        drawRoomDescription(rooms[playerY][playerX]);
        drawPlayer(playerX, playerY);
        if (rooms[playerY][playerX] == EXIT) {
            gameOver = true;
            continue;
        }
        // list the directions the player can take
        drawValidDirections(playerX, playerY);
        int direction = getMovementDirection();
        // before updating the player position, redraw the old room
        // character over the old position
        drawRoom(rooms, playerX, playerY);
        // update the player's position using the input movement data
        switch (direction) {
        case EAST:
            if (playerX < MAZE_WIDTH - 1)
                playerX++;
            break;
        case WEST:
            if (playerX > 0)
                playerX--;
            break;
        case NORTH:
            if (playerY > 0)
                playerY--;
            break;
        case SOUTH:
            if (playerY < MAZE_HEIGHT - 1)
                playerY++;
        default:
            // the direction was not valid,
            // do nothing, go back to the top of the loop and ask again
            break;
        }
    } // end game loop
    // jump to the correct location
    std::cout << CSI << PLAYER_INPUT_Y << ";" << 0 << "H";
    std::cout << std::endl << INDENT << "Press 'Enter' to exit the program.";
    std::cin.clear();
    std::cin.ignore(std::cin.rdbuf()->in_avail());
    std::cin.get();


    int height = 0;
    char firstLetterOfName = 0;
    int avatarHP = 0;

  //  // create a 2D array
  //  int rooms[MAZE_HEIGHT][MAZE_WIDTH];
  //  srand(time(nullptr));

  //  // fill the arrays with random room types
  //  for (int y = 0; y < MAZE_HEIGHT; y++) 
  //  {
  //      for (int x = 0; x < MAZE_WIDTH; x++) 
  //      {
  //          int type = rand() % (MAX_RANDOM_TYPE * 2);
  //          if (type < MAX_RANDOM_TYPE)
  //          {
		//		rooms[y][x] = type;
		//	}
  //          else
  //          {
  //              rooms[y][x] = rand() % MAX_RANDOM_TYPE;
  //          }
  //      }
  //  }
  //  // set the entrance and exit of the maze
  //  rooms[0][0] = ENTRANCE;
  //  rooms[MAZE_HEIGHT - 1][MAZE_WIDTH - 1] = EXIT;

  //  std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << std::endl;
  //  std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
  //  std::cout << INDENT << "It is definitely not related to any other text-based adventure game." << std::endl << std::endl;


  // // std::cout << INDENT << "First, some questions..." << std::endl;

  //  // save cursor position
  //  std::cout << SAVE_CURSOR_POS;
  //
  //  // output the map
  ///*  std::cout << std::endl;
  //  std::cout << std::endl;*/
  //
  //  std::cout << CSI << MAP_Y << ";" << 0 << "H";

  //  for (int y = 0; y < MAZE_HEIGHT; y++) 
  //  {
  //      std::cout << INDENT;
  //      for (int x = 0; x < MAZE_WIDTH; x++)
  //      {
  //          switch (rooms[y][x])
  //          {
  //          case EMPTY:
  //              std::cout << "[" << GREEN << "\xb0" << RESET_COLOR << "]";
  //              break;
  //          case ENEMY:
  //              std::cout << "[" << RED << "\x94" << RESET_COLOR << "]";
  //              break;
  //          case TREASURE:
  //              std::cout << "[" << YELLOW << "$" << RESET_COLOR << "]";
  //              break;
  //          case FOOD:
  //              std::cout << "[" << WHITE << "\xcf" << RESET_COLOR << "]";
  //              break;
  //          case ENTRANCE:
  //              std::cout << "[" << WHITE << "\x9d" << RESET_COLOR << "]";
  //              break;
  //          case EXIT:
  //              std::cout << "[" << WHITE << "\xFE" << RESET_COLOR << "]";
  //              break;
  //          }   
  //          
  //      }
  //      std::cout << std::endl;
  //  }

  //  // move the cursor back to the top of the map
  // 
  //  bool gameOver = false;
  //  int playerX = 0;
  //  int playerY = 0;

  //  // game loop
  //  while (!gameOver)
  //  {
  //      std::cout << RESTORE_CURSOR_POS << CSI << "A" << CSI << "4M" << CSI << "4L" << std::endl;

  //      // write description of current room
  //      switch (rooms[playerY][playerX]) 
  //      {
  //      case EMPTY:
  //          std::cout << INDENT << "You are in an empty meadow. There is nothing of note here." << std::endl;
  //          break;
  //      case ENEMY:
  //          std::cout << INDENT << "BEWARE. An enemy is approaching." << std::endl;
  //          break;
  //      case TREASURE:
  //          std::cout << INDENT << "Your journey has been rewarded. You have found some treasure" << std::endl;
  //          break;
  //      case FOOD:
  //          std::cout << INDENT << "At last! You collect some food to sustain you on your journey." << std::endl;
  //          break;
  //      case ENTRANCE:
  //          std::cout << INDENT << "The entrance you used to enter this maze is blocked. There is no going back." << std::endl;
  //          break;
  //      case EXIT:
  //          std::cout << INDENT << "Despite all odds, you made it to the exit. Congratulations." << std::endl;
  //          gameOver = true;
  //          continue;
  //      }

  //      // list the directions the player can take
  //      std::cout << INDENT << "You can see paths leading to the "
  //          << ((playerX > 0) ? "west, " : "")
  //          << ((playerX < MAZE_WIDTH - 1) ? "east, " : "")
  //          << ((playerY > 0) ? "north, " : "")
  //          << ((playerY < MAZE_HEIGHT - 1) ? "south, " : "")
  //          << std::endl;

  //      std::cout << INDENT << "Where to now?";
  //      
  //      int x = INDENT_X + (6 * playerX) + 3;
  //      int y = MAP_Y + playerY;

  //      // draaw the players position on the map
  //      // move cursor to map pos and delte chartacter at cuirrrent position
  //      std::cout << CSI << y << ";" << x << "H";
  //      std::cout << MAGENTA << "\x81";
  //      // move cursor to position for player to enter input
  //      std::cout << CSI << y << ";" << PLAYER_INPUT_X << "H" << YELLOW;

  //      //clear the input bufferm ready for player input
  //      std::cin.clear();
  //      std::cin.ignore(std::cin.rdbuf()->in_avail());

  //      int direction = 0;
  //      std::cin >> direction;
  //      std::cout << RESET_COLOR;

  //      if (std::cin.fail())
  //      {
  //          continue;       // go back to the top of the game loop and ask again
  //      }

  //      // before updating the player position, redraw the old room character over
  //      // the old position
  //      std::cout << CSI << y << ";" << x << "H";
  //      switch (rooms[playerY][playerX])
  //      {
  //      case EMPTY:
  //          std::cout << GREEN << "\xb0" << RESET_COLOR;
  //          break;
  //      case ENEMY:
  //          std::cout << RED << "\x94" << RESET_COLOR;
  //          break;
  //      case TREASURE:
  //          std::cout << YELLOW << "$" << RESET_COLOR;
  //          break;
  //      case FOOD:
  //          std::cout << WHITE << "\xcf" << RESET_COLOR;
  //          break;
  //      case ENTRANCE:
  //          std::cout << WHITE << "\x9d" << RESET_COLOR;
  //          break;
  //      case EXIT:
  //          std::cout << WHITE << "\xFE" << RESET_COLOR;
  //          break;
  //      }

  //      switch (direction) 
  //      {
  //      case EAST:
  //          if (playerX < MAZE_WIDTH - 1)
  //              playerX++;
  //          break;
  //      case WEST:
  //          if (playerX > 0)
  //              playerX--;
  //          break;
  //      case NORTH:
  //          if (playerY > 0)
  //              playerY--;
  //          break;
  //      case SOUTH:
  //          if (playerY < MAZE_HEIGHT - 1)
  //              playerY++;
  //      default:
  //          // do nothing, go back to the top of the loop and ask again
  //          break;
  //      }   
  //  }           // end of game loop
}