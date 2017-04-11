#include "Sudoku.h"

const int Sudoku::SIZE_BOARD = 9;
const int Sudoku::SIZE_SQUARE = 3;

Sudoku::Sudoku()
{
	solutionBoard_ = new int[SIZE_BOARD*SIZE_BOARD];
	playBoard_ = new sudokuNumber[SIZE_BOARD*SIZE_BOARD];
	squares_ = new square[SIZE_SQUARE*SIZE_SQUARE];
	sameNumbers_ = new std::list<sudokuNumber*>();

	gameWon = false;
	reset();

	for (int i = 0; i < SIZE_SQUARE; i++)
	{
		for (int j = 0; j < SIZE_SQUARE; j++)
		{
			squares_[i*SIZE_SQUARE + j].indexRow = i * SIZE_SQUARE;
			squares_[i*SIZE_SQUARE + j].indexColumn = j * SIZE_SQUARE;
		}
	}

	getSudokuProblem("text.txt");
	copyNumbers();
	findSolution();
}


Sudoku::~Sudoku()
{
	delete[] solutionBoard_;
	delete[] playBoard_;
	delete[] squares_;
	delete sameNumbers_;
}


int Sudoku::getValueAtIndex(int x, int y) const
{
	return playBoard_[y*SIZE_BOARD + x].value;
}


void Sudoku::setValue(int value, int x, int y)
{
	playBoard_[y*SIZE_BOARD + x].value = value;
}


const std::list<sudokuNumber*>* Sudoku::getSameNumbers() const
{
	return sameNumbers_;
}


void Sudoku::removeFromSameNumbers()
{
	auto iterator = sameNumbers_->begin();
	sameNumbers_->erase(iterator);
}


void Sudoku::copyNumbers()
{
	for (int i = 0; i < SIZE_BOARD; i++)
	{
		for (int j = 0; j < SIZE_BOARD; j++)
		{
			int number = solutionBoard_[i*SIZE_BOARD + j];
			playBoard_[i*SIZE_BOARD + j] = sudokuNumber{ number, i , j, number != 0 ? false : true };
		}
	}
}


bool Sudoku::isValueValid(int value, int x, int y, bool fillNumbers)
{
	if (sameNumbers_->size() > 0)
	{
		delete sameNumbers_;
	}

	if (!fillNumbers)
	{
		sameNumbers_ = new std::list<sudokuNumber*>();
		//sameNumbersCount = 0;
	}
	
	return isValueValidInRow(value, x, y, fillNumbers)
		& isValueValidInColumn(value, x, y, fillNumbers)
		& isValueValidInSquare(value, x, y, fillNumbers);
}


bool Sudoku::isValueValidInRow(int value, int x, int y, bool fillNumbers)
{
	for (int column = 0; column < SIZE_BOARD; column++)
	{
		if (fillNumbers && 
			solutionBoard_[y*SIZE_BOARD + column] == value)
		{
			return false;
		}
		else if (!fillNumbers && y*SIZE_BOARD + x != y*SIZE_BOARD + column &&
			playBoard_[y*SIZE_BOARD + column].value == value)
		{
			sameNumbers_->push_back(&playBoard_[y*SIZE_BOARD + column]);
			return false;
		}
	}

	return true;
}


bool Sudoku::isValueValidInColumn(int value, int x, int y, bool fillNumbers)
{
	for (int row = 0; row < SIZE_BOARD; row++)
	{
		if (fillNumbers && 
			solutionBoard_[row*SIZE_BOARD + x] == value)
		{
			return false;
		}
		else if (!fillNumbers && y*SIZE_BOARD + x != row*SIZE_BOARD + x &&
			playBoard_[row*SIZE_BOARD + x].value == value)
		{
			sameNumbers_->push_back(&playBoard_[row*SIZE_BOARD + x]);
			return false;
		}
	}

	return true;
}


bool Sudoku::isValueValidInSquare(int value, int x, int y, bool fillNumbers)
{
	int srib = y / SIZE_SQUARE;
	int scib = x / SIZE_SQUARE;
	int squareRow = squares_[srib*SIZE_SQUARE + scib].indexRow;
	int squareColumn = squares_[srib*SIZE_SQUARE + scib].indexColumn;

	for (int row = 0; row < SIZE_SQUARE; row++)
	{
		for (int column = 0; column < SIZE_SQUARE; column++)
		{
			
			if (fillNumbers && 
				solutionBoard_[(squareRow + row)*SIZE_BOARD + (squareColumn + column)] == value)
			{
				return false;
			}
			else if (!fillNumbers && y*SIZE_BOARD + x != (squareRow + row)*SIZE_BOARD + (squareColumn + column) &&
				playBoard_[(squareRow + row)*SIZE_BOARD + (squareColumn + column)].value == value)
			{
				sameNumbers_->push_back(&playBoard_[(squareRow + row) * SIZE_BOARD + (squareColumn + column)]);
				return false;
			}
		}
	}

	return true;
}


bool Sudoku::getNextEmptySolPosition(int& x, int& y) const
{
	for (y = 0; y < SIZE_BOARD; y++)
	{
		for (x = 0; x < SIZE_BOARD; x++)
		{
			if (solutionBoard_[y*SIZE_BOARD + x] == 0)
				return true;
		}
	}
	
	return false;
}


bool Sudoku::findSolution()
{
	int row, col;
	
	if (!getNextEmptySolPosition(col, row))
	{
		return true;
	}
	
	for (int i = 1; i <= SIZE_BOARD; i++)
	{
		if (isValueValid(i, col, row))
		{
			solutionBoard_[row*SIZE_BOARD + col] = i;

			if (findSolution())
				return true;

			solutionBoard_[row*SIZE_BOARD + col] = 0;
		}
	}

	return false;
}


void Sudoku::reset()
{
	memset(solutionBoard_, 0, sizeof(int) * SIZE_BOARD * SIZE_BOARD);
	memset(playBoard_, 0, sizeof(sudokuNumber) * SIZE_BOARD * SIZE_BOARD);
	memset(squares_, 0, sizeof(square)*SIZE_SQUARE*SIZE_SQUARE);
}


void Sudoku::getSudokuProblem(std::string filename)
{
	std::ifstream file;
	std::string text;

	file.open(filename);
	if (!file.is_open())
	{
		throw std::invalid_argument("The file doesn't exist!");
	}

	while (!file.eof())
	{
		int row;
		int column;
		int value;
		
		file >> text;
		getDataFromString(text, row, column, value);
		solutionBoard_[row*SIZE_BOARD + column] = value;
	}
}


void Sudoku::getDataFromString(std::string str, int & row, int & column, int & value)
{ 
	std::stringstream ss(str);
	char letter;
	int i = 0;

	while (ss >> letter)
	{
		if (letter >= '0' && letter <= '9')
		{
			switch (i)
			{
			case 0:
				row = letter - '0';
				i++;
				break;
			case 1:
				column = letter - '0';
				i++;
				break;
			case 2:
				value = letter - '0';
			}
		}
	}
}


bool Sudoku::isNumberEditable(int x, int y) const
{
	return playBoard_[y*SIZE_BOARD + x].editable;
}


bool Sudoku::tryFindPositionSameNumber(int row, int column) const
{
	for (int i = 0; static_cast<size_t>(i) < sameNumbers_->size(); i++)
	{
		auto snIterator = sameNumbers_->begin();
		std::advance(snIterator, i);
		
		if ((*snIterator)->row == row && (*snIterator)->column == column)
		{
			return true;
		}
			
	}

	return false;
}


bool Sudoku::isAnyPositionEmpty() const
{
	for (int i = 0; i < SIZE_BOARD; i++)
	{
		for (int j = 0; j < SIZE_BOARD; j++)
		{
			if (playBoard_[i*SIZE_BOARD + j].value == 0)
			{
				return true;
			}
		}
	}

	return false;
}


bool Sudoku::isGameWon() const
{
	return gameWon;
}


void Sudoku::setGameWon(bool status)
{
	gameWon = status;
}


bool Sudoku::checkPlayerSolution() const
{
	for (int i = 0; i < SIZE_BOARD; i++)
	{
		for (int j = 0; j < SIZE_BOARD; j++)
		{
			if (playBoard_[i*SIZE_BOARD + j].value != solutionBoard_[i*SIZE_BOARD + j])
			{
				return false;
			}
		}
	}

	return true;
}
