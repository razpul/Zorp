#include <iostream>
#include <windows.h>

const char* ESC = "\x1b";
const char* CSI = "\x1b[";

const char* TITLE = "\x1b[5;20H";
const char* INDENT = "\x1b[5C";
const char* YELLOW = "\x1b[93m";
const char* MAGENTA = "\x1b[95m"; 
const char* RESET_COLOR = "\x1b[0m";
const char* SAVE_CURSOR_POS = "\x1b[s";
const char* RESTORE_CURSOR_POS = "\x1b[u";


void main()
{
	// Set output mode to handle virtual terminal sequences
	DWORD dwMode = 0;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	int height = 0;
	char firstLetterOfName = 0;
	int avatarHP = 0;

	std::cout << TITLE << MAGENTA << "Welcome to ZORP!" << RESET_COLOR << std::endl; 
	std::cout << INDENT << "ZORP is a game of adventure, danger, and low cunning." << std::endl;
	std::cout << INDENT << "It is definitely not related to any other text-based adventure game." << std::endl;
	std::cout << std::endl;
	std::cout << "First, some questions..." << std::endl;
	std::cout << std::endl;
	// save cursor position
	std::cout << SAVE_CURSOR_POS;
	std::cout << INDENT << "How tall are you, in centimeters? " << INDENT << YELLOW << std::endl;

	std::cin >> height;
	std::cout << RESET_COLOR << std::endl;

	if (std::cin.fail())
	{
		std::cout << INDENT << "You have failed the first challenge and are eaten by a grue." << std::endl;
	}
	else
	{
		std::cout << INDENT << "You entered " << height << std::endl;
	}

	// Clear input buffer
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();

	// Move the cursor to the start of the 1st question
	std::cout << RESTORE_CURSOR_POS;
	// Delete the next 4 lines of text
	std::cout << CSI << "4m";


	std::cout << INDENT  << "What is the first letter of your name ?" << INDENT << YELLOW;

	std::cin >> firstLetterOfName;
	std::cout << RESET_COLOR << std::endl;

							// !isaplha checks if input was a-z charcter
	if (std::cin.fail() || !isalpha(firstLetterOfName))
	{
		std::cout << INDENT <<"You have failed the second challenge and are eaten by a grue" << std::endl;
	}
	else
	{
		std::cout << INDENT << "You entered " << firstLetterOfName << std::endl;
	}
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();

	// Move the cursor to the start of the 1st question
	std::cout << RESTORE_CURSOR_POS;
	std::cout << CSI << "A";		// cursor up 1
	std::cout << CSI << "4M";		// delete the next 4 lines of text

	if (firstLetterOfName != 0)
	{
		avatarHP = (float)height / (firstLetterOfName * 0.02f);
	}
	else
	{
		avatarHP = 0;
	}
	std::cout << std::endl << INDENT << "Using a complex deterministic algorithm, it has been calculated that you have " << avatarHP << " hit point(s)" << std::endl;

	std::cout << std::endl << INDENT << "Press 'Enter' to exit the program." << std::endl;
	std::cin.get();

	//system("cls"); - Used to clear the screen.

	return;

}