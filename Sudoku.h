#pragma once
#include <random>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>


/*
	Represents every square that contains 9x9 numbers
	with specific init position
*/
struct square
{
	int indexRow;
	int indexColumn;
};


/*
	Represents every number sudoku with specific value,
	position and if is possible change value
*/
struct sudokuNumber
{
	int value;
	int row;
	int column;
	bool editable;
};

/*
	Represents whole sudoku game and provides game logic
*/
class Sudoku
{
public:
	Sudoku();
	~Sudoku();

	/*
		Get value of sudoku number at specific position

		@param x Horizontal position in array
		@param y Vertical position in array
		@return Number at specific position
	*/
	int getValueAtIndex(int x, int y) const;

	/*
		Set specific value of sudoku number at specific position

		@param value New value of number
		@param x Horizontal position in board
		@param y Vertical position in board
	*/
	void setValue(int value, int x, int y);

	/*
		Get list of numbers that are the same in the row,
		column and square
		@return list of the same numbers
	*/
	const std::list<sudokuNumber*>* getSameNumbers() const;

	/*
		Remove first sudoku number from list of same numbers
	*/
	void removeFromSameNumbers();

	/*
		Checks if the number is editable

		@param x Horizontal position in board
		@param y Vertical position in board
		@return If the number is editable
	*/
	bool isNumberEditable(int x, int y) const;

	/*
		Tries find position in list of the same numbers

		@param row Index of row of tried position
		@param column Index of column of tried position
		@return if the position was found
	*/
	bool tryFindPositionSameNumber(int row, int column) const;

	/*
		Checks if is any empty position in playing board

		@return if is any empty position
	*/
	bool isAnyPositionEmpty() const;

	/*
		Checks if specific number is valid with sudoku rules

		@param value New value
		@param x Horizontal position of board
		@param y Vertical position of board
		@param fillNumbers Decides what board used
		@return if the value is valid
	*/
	bool isValueValid(int value, int x, int y, bool fillNumbers = true);

	/*
		Checks if the game is won

		@return If the game is won
	*/
	bool isGameWon() const;

	/*
		Set the decision about the winning game

		@param status Represents status about the winning game
	*/
	void setGameWon(bool status);

	/*
	Checks if the player's solution is correct

	@return If the player's solution is correct
	*/
	bool checkPlayerSolution() const;

	/*
		Sets containers to zeros
	*/
	void reset();
private:
	/*
		Copy numbers from solution into playing board
	*/
	void copyNumbers();
	
	/*
		Checks if the specific value is valid in row

		@param value New value
		@param x Horizontal position in board
		@param y Vertical position in board
		@param fillNumbers Decides what board used
		@return if the value is valid
	*/
	bool isValueValidInRow(int value, int x, int y, bool fillNumbers);

	/*
		Checks if the specific value is valid in column

		@param value New Value
		@param x Horizontal position of board
		@param y Vertical position of board
		@param fillNumbers Decides what board used
		@return if the value is valid
	*/
	bool isValueValidInColumn(int value, int x, int y, bool fillNumbers);

	/*
		Checks if the specific value is valid in square

		@param value New value
		@param x Horizontal position in board
		@param y Vertical position in board
		@param fillNumbers Decides what board used
		@return if the value is valid
	*/
	bool isValueValidInSquare(int value, int x, int y, bool fillNumbers);

	/*
		Checks if there is empty position in solution board and
		then returns it's position to ref params

		@param x Horizontal position of empty position
		@param y Vertical position of empty position
		@return if there is any empty position
	*/
	bool getNextEmptySolPosition(int& x, int& y) const;

	/*
		Finds solution using recursive method

		@return If the recursive is done
	*/
	bool findSolution();

	/*
		Loads sudoku problem from file

		@param filename Name of specific file
	*/
	void getSudokuProblem(std::string filename);

	/*
		Extracts indexes with specific values from string
		to ref variables

		@param str String with data
		@param row Index of row from string
		@param column Index of column from string
		@param value Value from string
	*/
	void getDataFromString(std::string str, int& row, int& column, int& value);
public:
	const static int SIZE_BOARD;
	const static int SIZE_SQUARE;
private:
	// Represents board for finding solution
	int* solutionBoard_;
	// Represents list of invalid numbers in specific positions
	std::list<sudokuNumber*>* sameNumbers_;
	// Represents board for player's solution
	sudokuNumber* playBoard_;
	// Represents order cells in 3x3 squares
	square* squares_;
	// Represents if the game is won
	bool gameWon;
};

