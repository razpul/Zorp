#include <iostream>

void main()
{
	int height = 0;
	char firstLetterOfName = 0;
	int avatarHP = 0;

	std::cout << "Welcome to ZORP!" << std::endl; 
	std::cout << "ZORP is a game of adventure, danger, and low cunning." << std::endl; 
	std::cout << "It is definitely not related to any other text-based adventure game." << std::endl << std::endl;

	std::cout << "First, some questions..." << std::endl;
	std::cout << "How tall are you, in centimeters? " << std::endl;

	std::cin >> height;
	if (std::cin.fail())
	{
		std::cout << "You have failed the first challenge and are eaten by a grue." << std::endl;
	}
	else
	{
		std::cout << "You entered " << height << std::endl;
	}
	// Clears error flags
	std::cin.clear();
	// Clears all chars in input buffer
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	std::cout << "What is the first letter of your name ?" << std::endl;

	std::cin >> firstLetterOfName;
							// !isaplha checks if input was a-z charcter
	if (std::cin.fail() || !isalpha(firstLetterOfName))
	{
		std::cout << "You have failed the second challenge and are eaten by a grue" << std::endl;
	}
	else
	{
		std::cout << "You entered " << firstLetterOfName << std::endl;
	}
	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());

	if (firstLetterOfName != 0)
	{
		avatarHP = (float)height / (firstLetterOfName * 0.02f);
	}
	else
	{
		avatarHP = 0;
	}
	std::cout << std::endl << "Using a complex deterministic algorithm, it has been calculated that you have " << avatarHP << " hit point(s)" << std::endl;

	std::cout << std::endl << "Press 'Enter' to exit the program." << std::endl;
	std::cin.get();

	return;

}