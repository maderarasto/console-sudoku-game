#include "ConsoleWindow.h"


ConsoleWindow::ConsoleWindow()
{
	iRows_ = 53;
	iCols_ = 90;
	iActualCell_ = 0;
	
	setSize();
	disableResize();
	hideCursor();

	try {
 		sudoku_ = new Sudoku();
	}
	catch (std::exception)
	{
		printTitle();
		print();
		showMessage("Couldn't open file", MessageInfo::ERROR_FILE);
		exit(1);
	}

	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			cTable[i][j] = Cell{ 
				j*GameWindowSettings::CELL_WIDTH + j,
				i*GameWindowSettings::CELL_HEIGHT + i 
			};
		}
	}

}



void ConsoleWindow::print()
{
	for (int i = 0; i < GameWindowSettings::GAME_BOARD_HEIGHT + 2; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), 
			COORD{ 
				GameWindowSettings::GAME_BOARD_X_POS - 2, 
				GameWindowSettings::GAME_BOARD_Y_POS - 1 + (short)i 
		});
		
		for (int j = 0; j < GameWindowSettings::GAME_BOARD_WIDTH + 4; j++)
		{
			if (i == 0 || i == GameWindowSettings::GAME_BOARD_HEIGHT + 1 ||
				j >= 0 && j <= 1 ||
				j >= GameWindowSettings::GAME_BOARD_WIDTH + 2 && 
				j <= GameWindowSettings::GAME_BOARD_WIDTH + 3)
			{
				std::cout << "#";
			}
			else
			{
				std::cout << " ";
			}
		}

		std::cout << std::endl;
	}
}

void ConsoleWindow::run()
{
	printTitle();
	print();

	bool isRunning = true;
	char c;

	do {
		printGame();

		if (sudoku_->isGameWon())
		{
			showMessage("YOU WON!", MessageInfo::WIN_MESSAGE);
		}

		c = _getch();
		switch (c)
		{
		case 'w':
			iActualCell_ -= 9;
			iActualCell_ = iActualCell_ < 0 ? iActualCell_ + 9 : iActualCell_;
			break;
		case 's':
			iActualCell_ += 9;
			iActualCell_ = iActualCell_ >= 81 ? iActualCell_ - 9 : iActualCell_;
			break;
		case'd':
			iActualCell_++;
			iActualCell_ = iActualCell_ >= 81 ? iActualCell_ - 1 : iActualCell_;
			break;
		case 'a':
			iActualCell_--;
			iActualCell_ = iActualCell_ < 0 ? iActualCell_ + 1 : iActualCell_;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),
				COORD{ (short)(GameWindowSettings::GAME_BOARD_X_POS + cTable[iActualCell_ / 9][iActualCell_ % 9].xPos + GameWindowSettings::CELL_WIDTH / 2),
					   (short)(GameWindowSettings::GAME_BOARD_Y_POS + cTable[iActualCell_ / 9][iActualCell_ % 9].yPos + GameWindowSettings::CELL_HEIGHT / 2)
			});
			if (sudoku_->isNumberEditable(iActualCell_ % 9, iActualCell_ / 9))
			{
				sudoku_->setValue(int(c - '0'), iActualCell_ % 9, iActualCell_ / 9);
				sudoku_->isValueValid(int(c - '0'), iActualCell_ % 9, iActualCell_ / 9, false);
				
				if (!sudoku_->isAnyPositionEmpty() && sudoku_->checkPlayerSolution())
				{
					sudoku_->setGameWon(true);
				}
			}
			break;
		}
	} while (isRunning);
}


ConsoleWindow::~ConsoleWindow()
{
	delete sudoku_;
}


void ConsoleWindow::setSize()
{
	char str[80];
	sprintf_s(str, "mode %d, %d", iCols_, iRows_);
	system(str);
}


void ConsoleWindow::disableResize()
{
	long l = GetWindowLong(GetConsoleWindow(), GWL_STYLE);
	l &= ~(WS_THICKFRAME);
	SetWindowLong(GetConsoleWindow(), GWL_STYLE, l);
}

void ConsoleWindow::hideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
	cursor.bVisible = false;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

void ConsoleWindow::printTitle()
{
	std::string line = "==========================================================";
	std::string title[] = {
		" ######   #     #   ######     #####     #     #   #     #",
		"#         #     #   #     #   #     #    #    #    #     #",
		" #####    #     #   #     #   #     #    #####     #     #",
		"      #   #     #   #     #   #     #    #    #    #     #",
		"######     #####    ######     #####     #     #    ##### ",
	};
	std::string subtitle[]{
		"                   ---------------------                  ",
		"                    by Rastislav Madera                   ",
		"                   ---------------------                  "
	};
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 15, 3 });
	std::cout << line << std::endl;
	
	for (int i = 0; i < ARRAYSIZE(title); i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{15, 4 + (short) i});
		std::cout << title[i] << std::endl;
	}
	
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 15, 9 });
	std::cout << line << std::endl;

	for (int i = 0; i < ARRAYSIZE(subtitle); i++)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 15, 10 + (short)i });
		std::cout << subtitle[i] << std::endl;
	}
}

void ConsoleWindow::printGame()
{
	for (int i = 0, k = 0, m = 0; i < GameWindowSettings::GAME_BOARD_HEIGHT; i++)
	{
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), 
			COORD{ 
				GameWindowSettings::GAME_BOARD_X_POS, 
				(short)(GameWindowSettings::GAME_BOARD_Y_POS + i) 
		});
		
		for (int j = 0, l = 0, n = 0; j < GameWindowSettings::GAME_BOARD_WIDTH; j++)
		{
			int number = sudoku_->getValueAtIndex(n, m);
			
			if (i >= cTable[iActualCell_ / 9][iActualCell_ % 9].yPos && 
				i <= cTable[iActualCell_ / 9][iActualCell_ % 9].yPos + GameWindowSettings::CELL_HEIGHT - 1 &&
				j >= cTable[iActualCell_ / 9][iActualCell_ % 9].xPos &&
				j <= cTable[iActualCell_ / 9][iActualCell_ % 9].xPos + GameWindowSettings::CELL_WIDTH - 1)
			{

				if (i == cTable[iActualCell_ / 9][iActualCell_ % 9].yPos + GameWindowSettings::CELL_HEIGHT / 2 &&
					j == cTable[iActualCell_ / 9][iActualCell_ % 9].xPos + GameWindowSettings::CELL_WIDTH  / 2)
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					std::cout << sudoku_->getValueAtIndex(iActualCell_ % 9, iActualCell_ / 9);
				}
				else
				{
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
					std::cout << " ";
				}

				int tempN = n;
				n = j >= cTable[m][n].xPos + GameWindowSettings::CELL_WIDTH - 1 ? n + 1 : n;
				m = i >= cTable[m][tempN].yPos + GameWindowSettings::CELL_HEIGHT ? m + 1 : m;
			}
			else if (i >= cTable[m][n].yPos && i <= cTable[m][n].yPos + GameWindowSettings::CELL_HEIGHT &&
				j >= cTable[m][n].xPos && j <= cTable[m][n].xPos + GameWindowSettings::CELL_WIDTH - 1)
			{
				if (i == cTable[m][n].yPos + GameWindowSettings::CELL_HEIGHT / 2 &&
					j == cTable[m][n].xPos + GameWindowSettings::CELL_WIDTH  / 2)
				{
					if (sudoku_->tryFindPositionSameNumber(m, n))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
					}
					else if (!sudoku_->isNumberEditable(n, m))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY);
					}

					else

					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}

					std::cout << number;

				}
				else
				{
					if (sudoku_->tryFindPositionSameNumber(m, n))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
					}
					else if (!sudoku_->isNumberEditable(n, m))
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_INTENSITY);

					}
					else
					{
						SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
					}
					std::cout << " ";
				}
			
				if (sudoku_->tryFindPositionSameNumber(m, n) &&
					i == cTable[m][n].yPos + GameWindowSettings::CELL_HEIGHT &&
					j == cTable[m][n].xPos + GameWindowSettings::CELL_WIDTH - 1)
				{
					sudoku_->removeFromSameNumbers();
				}

				int tempN = n;
				n = j >= cTable[m][n].xPos + GameWindowSettings::CELL_WIDTH - 1     ? n + 1 : n;
				m = i >= cTable[m][tempN].yPos + GameWindowSettings::CELL_HEIGHT    ? m + 1 : m;

			}
			else if (k % 3 == 2 && i == GameWindowSettings::VERTICAL_SEPARATOR[k])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "#";
				k = j >= GameWindowSettings::GAME_BOARD_WIDTH - 1 ? k + 1 : k;
			}
			else if (l % 3 == 2 && j == GameWindowSettings::HORIZONTAL_SEPARATOR[l])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "#";
				l++;
			}
			else if (i == GameWindowSettings::VERTICAL_SEPARATOR[k] && 
					 j == GameWindowSettings::HORIZONTAL_SEPARATOR[l])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "+";
				l++;
			}
			else if (i == GameWindowSettings::VERTICAL_SEPARATOR[k])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "-";
				k = j >= GameWindowSettings::GAME_BOARD_WIDTH - 1 ? k + 1 : k;
			}
			else if (j == GameWindowSettings::HORIZONTAL_SEPARATOR[l])
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << "|";
				l++;
			}
			else
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
				std::cout << " ";
			}
		}

		std::cout << std::endl;
	}
}

void ConsoleWindow::showMessage(std::string message, MessageInfo info)
{
	bool isPrinted = true;
	char c;

	while (isPrinted)
	{
		

		for (int i = 0; i < 5; i++)
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 28, 28 + short(i) });
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), info == MessageInfo::WIN_MESSAGE ? BACKGROUND_GREEN : BACKGROUND_RED);

			for (int j = 0; j < 40; j++)
			{
				int messageStartIndex = (40 - message.length()) / 2;
				
				if (i == 2 && j >= messageStartIndex && (j - messageStartIndex) >= 0 &&
					(j - messageStartIndex) < message.length())
				{
					std::cout << message.at(j - messageStartIndex);
				}
				else
				{
					std::cout << " ";
				}
			}

			std::cout << std::endl;
		}
			
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 28, 33 });
		std::cout << "        To exit press <ENTER> ...       " << std::endl;
		
		std::cout << std::endl;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), COORD{ 28, 34});
		for (int i = 0; i < 40; i++)
			std::cout << " ";

		c = _getch();
		if (c == '\r')
		{
			exit((int)info);
		}
	}
}
