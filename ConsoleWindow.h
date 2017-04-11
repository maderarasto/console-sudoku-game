#pragma once
#include <iostream>
#include <conio.h>
#include <string>
#include <Windows.h>
#include "Sudoku.h"

// Mapping cell to position in console window
struct Cell
{
	int xPos;
	int yPos;
};

// Represents namespace of constants
namespace GameWindowSettings
{
	const int GAME_BOARD_WIDTH = 71;
	const int GAME_BOARD_HEIGHT = 35;
	const int GAME_BOARD_X_POS = 10;
	const int GAME_BOARD_Y_POS = 14;
	const int CELL_WIDTH = 7;
	const int CELL_HEIGHT = 3;
	const int HORIZONTAL_SEPARATOR[] = { 7, 15, 23, 31, 39, 47, 55, 63 };
	const int VERTICAL_SEPARATOR[] = { 3, 7, 11, 15, 19, 23, 27, 31 };
}

// Represent key event constants
enum class KeyEvent
{
	UP, DOWN, RIGHT, LEFT
};

// Represents info about message
enum class MessageInfo
{
	WIN_MESSAGE, ERROR_FILE
};

// Represents console window and its settings
class ConsoleWindow
{
public:
	ConsoleWindow();
	~ConsoleWindow();

	/*
		Prints border of the game
	*/
	void print();

	/*
		Represents game loop
	*/
	void run();
private:
	/*
		Sets size of console window
	*/
	void setSize();

	/*
		Disables that window can be resizable
	*/
	void disableResize();

	/*
		Hides cursor of the console window
	*/
	void hideCursor();

	/*
		Prints title of game
	*/
	void printTitle();
	
	/*
		Prints game after each update
	*/
	void printGame();

	/*
		Shows message to base info

		@param message Specific message
		@param info Info about message
	*/
	void showMessage(std::string message, MessageInfo info);
private:
	// Represents row in window
	int iRows_;
	
	// Represents column in window
	int iCols_;
	
	// Represent cell that cursor is pointing
	int iActualCell_;
	
	// Represents container of cells
	Cell cTable[9][9];

	// Represents sudoku game
	Sudoku* sudoku_;
};

