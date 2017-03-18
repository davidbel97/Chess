#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

struct Piece{
	bool side;  // false is white, true is black.   on board Capital is white, Lowercase is Black.
	string type;
	int moves;
};
struct Place{
	Piece* piece;	
};
struct Board{
	Place board[8][8];
};

void newGame(Board& gameBoard); // Initializes the gameBoard to have the starting position of the game.

void printBoard(Place board[][8], bool turn); //Prints the board from the perspective of the person who's turn it is.

bool valid(string& pos1, string& pos2, Board& gameBoard); //Checks if a move is valid. Returns true if it is valid.

bool moveset(string& pos1, string& pos2, Board& gameBoard); //Checks if a move is within the allowable moveset of the specific piecetype. Returns true if it is. 

bool nightRules(string& pos1, string& pos2); //Checks if the move is in the allowable moveset for a Knight. Returns true if it is. 

bool bishopRules(string& pos1, string& pos2, Place board[][8]); // Checks if the move is in the allowable moveset for a Bishop. Returns true if it is.

bool rookRules(string& pos1, string& pos2, Place board[][8]); // Checks if the move is in the allowable moveset for a Rook. Returns true if it is.

bool queenRules(string& pos1, string& pos2, Place board[][8]); // Checks if the move is in the allowable moveset for a Queen. Returns true if it is.

bool kingRules(string& pos1, string& pos2, Board& gameBoard); // Checks if the move is in the allowable moveset for a King. Returns true if it is.

bool pawnRules(string& pos1, string& pos2, Place board[][8]); // Checks if the move is in the allowable moveset for a pawn. Returns true if it is.

bool legal(string& pos1, string& pos2, Board& gameBoard); // Checks if the move results in the mover being in check. Returns true if it doesn't and the move is legal.

bool check(bool& side, Board& gameBoard); // Checks if there is a check for the particular player. returns true if it is.

bool movesLeft(bool& side, Board& gameBoard); // Checks if there are any moves left for the particular player. returns true if there are.

void promotePawn(string& pos, Board& gameBoard, ofstream& output); //promotes a pawn to a desired piece.

void playMove(string& pos1, string& pos2, Board& gameBoard, ofstream& output); //Plays the move specified, updates the gameBoard, and outputs the move notation to the output file.

void playMove(string& pos1, string& pos2, Board& gameBoard); //Used for checking hypothetical moves on copyBoards. same as above but no output file. 

void countMoves(bool& turn, string& pos, Board& gameBoard); //Increments the number of moves for all of the specified players pieces if they have ever moved. 

bool gameOver(bool& turn, string& status, Board& gameBoard, ofstream& output); //Returns true if the game is over. also updates status to contain the status of the game. 

int main()
{
	ofstream output;
	output.open("GameLog.txt");
	Board gameBoard;
	newGame(gameBoard);
	bool turn = false, end = false, test, test2;
	int turnCounter = 0;
	string pos1, pos2, status = "\n" ;
	while(!end)
	{
		if(!turn)
		{
			output << ++turnCounter << '.';
		}
		if(turn)
		{
			output << ',';
			cout << "Black's Turn" << endl;
		}
		else
		{
			cout << "White's Turn" << endl;
		}
		output << ' ';
		cout << status;
		printBoard(gameBoard.board, turn);
		test = true;
		test2 = true;
		while(test2)
		{
			while(test)
			{
				cout << "Please enter the position of the piece you would like to move(ex. a5), or enter \"forfeit\" or \"draw\": ";
				cin >> pos1;
				if(pos1 == "forfeit")
				{
					if(!turn)
					{
						output << "White ";
					}
					else
					{
						output << "Black ";
					}
					output << "Resigns.";
					output.close();
					return 0;
				}
				else if(pos1 == "draw")
				{
					cout << "Draw offer was proposed. Do you accept. Enter \"accept\" to accept the draw offer, anything else will resume the opponent's turn. ";
					cin >> pos1;
					if(pos1 == "accept")
					{
						output << "Draw Offer Accepted";
						output.close();
						return 0;
					}
					else
					{
						cout << "Draw offer declined." << endl;
						continue;
					}
				}
				else if(pos1.length() != 2)
				{
					cout << "That is an invalid entry. The position must be exactly 2 characters long." << endl;
					continue;
				}
				else if(!(pos1[0] <= 'h' && pos1[0] >= 'a'))
				{
					cout << "The first character must be a letter between a and h." << endl;
					continue;
				}
				else if(!(pos1[1] <= '8' && pos1[1] >= '1'))
				{
					cout << "The second character must be a number between 1 and 8." << endl;
					continue;
				}
				else if(gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece == NULL)
				{
					cout << "There is no piece at that square." << endl;
					continue;
				}
				else if(gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->side != turn)
				{
					cout << "You cannot move your opponent's piece." << endl;
					continue;
				}
				else
				{
					test = false;
				}
			}
			cout << "Please enter the position you would like to move the piece to(ex. d5) or \"back\" to select a new piece or attempt to end the match: ";
			cin >> pos2;
			if(pos2 == "back")
			{
				test = true;
				continue;
			}
			else if(pos2.length() != 2)
			{
				cout << "That is an invalid entry. The position must be exactly 2 characters long or the word \"back\"." << endl;
				continue;
			}
			else if(!(pos2[0] <= 'h' && pos2[0] >= 'a'))
			{
				cout << "The first character must be a letter between a and h." << endl;
				continue;
			}
			else if(!(pos2[1] <= '8' && pos2[1] >= '1'))
			{
				cout << "The second character must be a number between 1 and 8." << endl;
				continue;
			}
			else if(pos1 == pos2)
			{
				cout << "You must move the piece." << endl;
				continue;
			}
			if(gameBoard.board[pos2[0] - 97][pos2[1] - 49].piece == NULL)
			{
				if(!valid(pos1, pos2, gameBoard))
				{
					continue;
				}
				else
				{
					test2 = false;
				}
			}
			else if(gameBoard.board[pos2[0] - 97][pos2[1] - 49].piece->side == turn)
			{
				cout << "You cannot take your own piece." << endl;
				continue;
			}
			else if(!valid(pos1, pos2, gameBoard))
			{
				continue;
			}
			else
			{
				test2 = false;
			}
		}
		playMove(pos1, pos2, gameBoard, output);
		if((pos2[1]-49 == 7 || pos2[1]-49 == 0) && (gameBoard.board[pos2[0]-97][pos2[1]-49].piece->type == "Pawn"))
		{
			promotePawn(pos2, gameBoard, output);
		}
		countMoves(turn, pos2, gameBoard);
		turn = !turn;
		end = gameOver(turn, status, gameBoard, output);
		if(!turn)
		{
			output << endl;
		}
	}
	cout << status;
	if(turn && status == "Checkmate\n")
	{
		output << "White Wins!";
	}
	else if(!turn && status == "Checkmate\n")
	{
		output << "Black Wins!";
	}
	printBoard(gameBoard.board, turn);
	output.close();
	cin.ignore(2); 
	return 0;
}

bool gameOver(bool& turn, string& status, Board& gameBoard, ofstream& output)
{
	status = "\n";
	if(movesLeft(turn, gameBoard))
	{
		if(check(turn, gameBoard))
		{
			status = "Check" + status;
			output << '+';
		}
		return false;
	}
	else
	{
		if(check(turn, gameBoard))
		{
			status = "Checkmate" + status;
			output << '#' << endl << "Checkmate";
		} 
		else
		{
			output << endl << "Stalemate";
			status = "Stalemate" + status;
		}
		return true;
	}
}

void countMoves(bool& turn, string& pos, Board& gameBoard)
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(gameBoard.board[i][j].piece != NULL)
			{
				if((gameBoard.board[i][j].piece->side == turn) && (gameBoard.board[i][j].piece->type == "Pawn") && (gameBoard.board[i][j].piece->moves != 0))
				{
					gameBoard.board[i][j].piece->moves++;
				}
			}
		}
	}
	gameBoard.board[pos[0] -97][pos[1] -49].piece->moves++;
	return;
}

void newGame(Board& gameBoard)
{
	for(int f = 0; f < 8; f++)
	{
		for(int r = 2; r < 6; r++)
		{
			gameBoard.board[f][r].piece = NULL;
		}
	}
	for(int f = 0; f < 8; f++)
	{
		gameBoard.board[f][1].piece = new Piece;
		gameBoard.board[f][1].piece->side = false;
		gameBoard.board[f][1].piece->type = "Pawn";
		gameBoard.board[f][1].piece->moves = 0;
	}
	for(int f = 0; f < 8; f++)
	{
		gameBoard.board[f][6].piece = new Piece;
		gameBoard.board[f][6].piece->side = true;
		gameBoard.board[f][6].piece->type = "Pawn";
		gameBoard.board[f][6].piece->moves = 0;
	}
	gameBoard.board[0][0].piece = new Piece;
	gameBoard.board[0][0].piece->side = false;
	gameBoard.board[0][0].piece->type = "Rook";
	gameBoard.board[0][0].piece->moves = 0;
	gameBoard.board[1][0].piece = new Piece;
	gameBoard.board[1][0].piece->side = false;
	gameBoard.board[1][0].piece->type = "Night";
	gameBoard.board[1][0].piece->moves = 0;
	gameBoard.board[2][0].piece = new Piece;
	gameBoard.board[2][0].piece->side = false;
	gameBoard.board[2][0].piece->type = "Bishop";
	gameBoard.board[2][0].piece->moves = 0;
	gameBoard.board[3][0].piece = new Piece;
	gameBoard.board[3][0].piece->side = false;
	gameBoard.board[3][0].piece->type = "Queen";
	gameBoard.board[3][0].piece->moves = 0;
	gameBoard.board[4][0].piece = new Piece;
	gameBoard.board[4][0].piece->side = false;
	gameBoard.board[4][0].piece->type = "King";
	gameBoard.board[4][0].piece->moves = 0;
	gameBoard.board[5][0].piece = new Piece;
	gameBoard.board[5][0].piece->side = false;
	gameBoard.board[5][0].piece->type = "Bishop";
	gameBoard.board[5][0].piece->moves = 0;
	gameBoard.board[6][0].piece = new Piece;
	gameBoard.board[6][0].piece->side = false;
	gameBoard.board[6][0].piece->type = "Night";
	gameBoard.board[6][0].piece->moves = 0;
	gameBoard.board[7][0].piece = new Piece;
	gameBoard.board[7][0].piece->side = false;
	gameBoard.board[7][0].piece->type = "Rook";
	gameBoard.board[7][0].piece->moves = 0;
	gameBoard.board[0][7].piece = new Piece;
	gameBoard.board[0][7].piece->side = true;
	gameBoard.board[0][7].piece->type = "Rook";
	gameBoard.board[0][7].piece->moves = 0;
	gameBoard.board[1][7].piece = new Piece;
	gameBoard.board[1][7].piece->side = true;
	gameBoard.board[1][7].piece->type = "Night";
	gameBoard.board[1][7].piece->moves = 0;
	gameBoard.board[2][7].piece = new Piece;
	gameBoard.board[2][7].piece->side = true;
	gameBoard.board[2][7].piece->type = "Bishop";
	gameBoard.board[2][7].piece->moves = 0;
	gameBoard.board[3][7].piece = new Piece;
	gameBoard.board[3][7].piece->side = true;
	gameBoard.board[3][7].piece->type = "Queen";
	gameBoard.board[3][7].piece->moves = 0;
	gameBoard.board[4][7].piece = new Piece;
	gameBoard.board[4][7].piece->side = true;
	gameBoard.board[4][7].piece->type = "King";
	gameBoard.board[4][7].piece->moves = 0;
	gameBoard.board[5][7].piece = new Piece;
	gameBoard.board[5][7].piece->side = true;
	gameBoard.board[5][7].piece->type = "Bishop";
	gameBoard.board[5][7].piece->moves = 0;
	gameBoard.board[6][7].piece = new Piece;
	gameBoard.board[6][7].piece->side = true;
	gameBoard.board[6][7].piece->type = "Night";
	gameBoard.board[6][7].piece->moves = 0;
	gameBoard.board[7][7].piece = new Piece;
	gameBoard.board[7][7].piece->side = true;
	gameBoard.board[7][7].piece->type = "Rook";
	gameBoard.board[7][7].piece->moves = 0;
	return;
}

void printBoard(Place board[][8], bool turn)
{
	cout << "  ";
	cout << string(15, '_') << endl;
	if(!turn)
	{		
		for(int r = 8; r > 0; r--)
		{
			cout << r << '|';
			for(int f = 0; f < 8; f++)
			{
				if(board[f][r-1].piece == NULL)
				{
					cout << ' ';
				}
				else if(board[f][r-1].piece->side)
				{
					char temp;
					temp = tolower(board[f][r-1].piece->type[0]);
					cout << temp;
				}
				else
				{
					cout << board[f][r-1].piece->type[0];
				}
				cout << '|';
			}
			cout << endl << ' ' << '+' << string(15, '-') << '+' << endl;
		}
		cout << "  ";
		for(char f = 'a'; f <= 'h'; f++)
		{
			cout << f << ' ';
		}
	}
	else
	{
		for(int r = 1; r < 9; r++)
		{
			cout << r << '|';
			for(int f = 7; f >= 0; f--)
			{
				if(board[f][r-1].piece == NULL)
				{
					cout << ' ';
				}
				else if(board[f][r-1].piece->side)
				{
					char temp;
					temp = tolower(board[f][r-1].piece->type[0]);
					cout << temp;
				}
				else
				{
					cout << board[f][r-1].piece->type[0];
				}
				cout << '|';
			}
			cout << endl << ' ' << '+' << string(15, '-') << '+' << endl;
		}
		cout << "  ";
		for(char f = 'h'; f >= 'a'; f--)
		{
			cout << f << ' ';
		}
	}
	cout << endl;
	return;
}

bool valid(string& pos1, string& pos2, Board& gameBoard)
{
	if(!moveset(pos1, pos2, gameBoard))
	{
		cout << "That is not a valid move for a " << gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type << '.' << endl;
		return false;
	}
	else if(!legal(pos1, pos2, gameBoard))
	{
		cout << "Your move cannot result in a check for yourself." << endl;
		return false;
	}
	else
	{
		return true;
	}
}

bool moveset(string& pos1, string& pos2, Board& gameBoard)
{
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "Night")
	{
		return nightRules(pos1, pos2);
	}
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "Bishop")
	{
		return bishopRules(pos1, pos2, gameBoard.board);
	}
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "Rook")
	{
		return rookRules(pos1, pos2, gameBoard.board);
	}
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "Queen")
	{
		return queenRules(pos1, pos2, gameBoard.board);
	}
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "King")
	{
		return kingRules(pos1, pos2, gameBoard);
	}
	if((gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece->type) == "Pawn")
	{
		return pawnRules(pos1, pos2, gameBoard.board);
	}
}

bool pawnRules(string& pos1, string& pos2, Place board[][8])
{
	int moveX, moveY;
	moveX = abs(pos2[0] - pos1[0]);
	moveY = pos2[1] - pos1[1];
	bool side = board[pos1[0] - 97][pos1[1]-49].piece->side;
	if((abs(moveY) == 2) && (moveX == 0) && (board[pos2[0]-97][pos2[1]-49].piece == NULL) && (board[pos1[0]-97][pos1[1]-49].piece->moves == 0))
	{
		return true;
	}
	else if((moveX == 0) && (abs(moveY) == 1) && (board[pos2[0]-97][pos2[1]-49].piece == NULL))
	{
		if(side)
		{
			if(moveY < 0)
			{
				return true;
			}
		}
		else if(moveY > 0)
		{
			return true;
		}
	}
	else if((moveX == 1) && (abs(moveY) == 1))
	{
		if(board[pos2[0]-97][pos2[1]-49].piece != NULL)
		{
			if(side)
			{
				if(moveY < 0)
				{
					return true;
				}
			}
			else if(moveY > 0)
			{
				return true;
			}
		}
		else if(board[pos2[0] - 97][pos1[1]-49].piece != NULL)
		{
			if((board[pos2[0] - 97][pos1[1]-49].piece->type == "Pawn") && (board[pos2[0] - 97][pos1[1]-49].piece->side != board[pos1[0] - 97][pos1[1]-49].piece->side) && (board[pos2[0] - 97][pos1[1]-49].piece->moves == 1))
			{
				if(!side)
				{
					if(((pos1[1]-49) == 4) && (moveY > 0))
					{
						return true;
					}
				}
				else
				{
					if(((pos1[1]-49) == 3) && (moveY < 0))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool rookRules(string& pos1, string& pos2, Place board[][8])
{
	int moveX, moveY;
	moveX = pos2[0] - pos1[0];
	moveY = pos2[1] - pos1[1];
	if(abs(moveX) == 0 xor abs(moveY) == 0)
	{
		string temp = pos1;
		if(moveX != 0)
		{
			if(moveX < 0)
			{
				temp[0]--;
				while(temp[0] != pos2[0])
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]--;
				}
			}
			else
			{
				temp[0]++;
				while(temp[0] != pos2[0])
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]++;
				}
			}
		}
		else if(moveY != 0)
		{
			if(moveY < 0)
			{
				temp[1]--;
				while(temp[1] != pos2[1])
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[1]--;
				}
			}
			else
			{
				temp[1]++;
				while(temp[1] != pos2[1])
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[1]++;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool bishopRules(string& pos1, string& pos2, Place board[][8])
{
	int moveX, moveY;
	moveX = pos2[0] - pos1[0];
	moveY = pos2[1] - pos1[1];
	if(abs(moveX) == abs(moveY))
	{
		string temp = pos1;
		if(moveX == moveY)
		{
			if(moveX < 0)
			{
				temp[0]--;
				temp[1]--;
				while(temp != pos2)
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]--;
					temp[1]--;
				}
			}
			else
			{
				temp[0]++;
				temp[1]++;
				while(temp != pos2)
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]++;
					temp[1]++;
				}
			}
		}
		else
		{
			if(moveX < 0)
			{
				temp[0]--;
				temp[1]++;
				while(temp != pos2)
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]--;
					temp[1]++;
				}
			}
			else
			{
				temp[0]++;
				temp[1]--;
				while(temp != pos2)
				{
					if(board[temp[0]-97][temp[1]-49].piece != NULL)
					{
						return false;
					}
					temp[0]++;
					temp[1]--;
				}
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool nightRules(string& pos1, string& pos2)
{
	int moveX, moveY;
	moveX = abs(pos1[0] - pos2[0]);
	moveY = abs(pos1[1] - pos2[1]);
	if((moveX == 1 && moveY == 2) || (moveX == 2 && moveY == 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool queenRules(string& pos1, string& pos2, Place board[][8])
{
	if(rookRules(pos1, pos2, board) || bishopRules(pos1, pos2, board))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool kingRules(string& pos1, string& pos2, Board& gameBoard)
{
	int moveX, moveY;
	moveX = pos2[0] - pos1[0];
	moveY = pos2[1] - pos1[1];
	if((abs(moveX) <= 1) && (abs(moveY) <= 1))
	{
		return true;
	}
	else if(moveY == 0 && abs(moveX) == 2 && gameBoard.board[pos1[0]-97][pos1[1]-49].piece->moves == 0)
	{
		if(moveX > 0)
		{
			for(int x = pos1[0]-96; x < 7; x++)
			{
				if(gameBoard.board[x][pos1[1]-49].piece != NULL)
				{
					return false;
				}
			}
			if(gameBoard.board[7][pos2[1]-49].piece == NULL)
			{
				return false;
			}
			else if(gameBoard.board[7][pos2[1]-49].piece->type == "Rook" && gameBoard.board[7][pos2[1]-49].piece->moves == 0)
			{
				string testpos = pos1;
				testpos[0] += 1;
				if(!check(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->side, gameBoard) && legal(pos1, testpos, gameBoard))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			for(int x = pos1[0]-98; x > 0; x--)
			{
				if(gameBoard.board[x][pos1[1]-49].piece != NULL)
				{
					return false;
				}
			}
			if(gameBoard.board[0][pos2[1]-49].piece == NULL)
			{
				return false;
			}
			else if(gameBoard.board[0][pos2[1]-49].piece->type == "Rook" && gameBoard.board[0][pos2[1]-49].piece->moves == 0)
			{
				string testpos = pos1;
				testpos[0] -= 1;
				if(!check(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->side, gameBoard) && legal(pos1, testpos, gameBoard))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}

bool legal(string& pos1, string& pos2, Board& gameBoard)
{
	Board copyBoard;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(gameBoard.board[i][j].piece == NULL)
			{
				copyBoard.board[i][j].piece = NULL;
			}
			else
			{
				copyBoard.board[i][j].piece = new Piece;
				copyBoard.board[i][j].piece->type = gameBoard.board[i][j].piece->type;
				copyBoard.board[i][j].piece->moves = gameBoard.board[i][j].piece->moves;
				copyBoard.board[i][j].piece->side = gameBoard.board[i][j].piece->side;
			}
		}
	}
	bool turn = copyBoard.board[pos1[0]-97][pos1[1]-49].piece->side;
	playMove(pos1, pos2, copyBoard);
	bool isCheck;
	isCheck = check(turn, copyBoard);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(copyBoard.board[i][j].piece != NULL)
			{
				delete copyBoard.board[i][j].piece;
			}
		}
	}
	return !isCheck;
}

bool check(bool& side, Board& gameBoard)
{
	bool found = false;
	char kposX, kposY;
	string kpos;
	
	for(int i = 0; i < 8 && !found; i++)
	{
		for(int j = 0; j < 8 && !found; j++)
		{
			if(gameBoard.board[i][j].piece != NULL)
			{
				if(gameBoard.board[i][j].piece->side == side && gameBoard.board[i][j].piece->type == "King")
				{
					found = true;
					kposX = i;
					kposY = j;
				}
			}
		}
	}
	kposX += 97;
	kposY += 49;
	kpos += kposX;
	kpos += kposY;
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(gameBoard.board[i][j].piece != NULL)
			{
				if(gameBoard.board[i][j].piece->side != side)
				{
					char testx, testy;
					string test = "";
					testx = i + 97;
					testy = j + 49;
					test += testx;
					test += testy;
					if(moveset(test, kpos, gameBoard))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool movesLeft(bool& side, Board& gameBoard)
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			if(gameBoard.board[i][j].piece != NULL)
			{
				if(gameBoard.board[i][j].piece->side == side)
				{
					string pos1 = "  ";
					pos1[0] = i + 97;
					pos1[1] = j + 49;
					for(int k = 0; k < 8; k++)
					{
						for(int l = 0; l < 8; l++)
						{
							if(gameBoard.board[k][l].piece != NULL)
							{
								if(gameBoard.board[k][l].piece->side != side)
								{
									string pos2 = "  ";
									pos2[0] = k + 97;
									pos2[1] = l + 49;
									if(moveset(pos1, pos2, gameBoard))
									{
										if(legal(pos1, pos2, gameBoard))
										{
											return true;
										}
									}
								}
							}
							else
							{
								string pos2 = "  ";
								pos2[0] = k + 97;
								pos2[1] = l + 49;
								if(moveset(pos1, pos2, gameBoard))
								{
									if(legal(pos1, pos2, gameBoard))
									{
										return true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return false;
}

void playMove(string& pos1, string& pos2, Board& gameBoard, ofstream& output)
{
	if(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->type != "Pawn")
	{
		if(!(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->type == "King" && abs(pos2[0]-pos1[0]) == 2))
		{
			output << gameBoard.board[pos1[0]-97][pos1[1]-49].piece->type[0];
		}
	}
	if(gameBoard.board[pos2[0] -97][pos2[1] -49].piece != NULL)
	{
		output << pos1;
		delete gameBoard.board[pos2[0] -97][pos2[1] -49].piece;
		output << 'x';
		output << pos2;
	}
	else if((gameBoard.board[pos1[0] -97][pos1[1] -49].piece->type == "Pawn") && (abs(pos2[0]-pos1[0]) == 1))
	{
		output << pos1;
		output << 'x';
		output << pos2;
		delete gameBoard.board[pos2[0]-97][pos1[1]-49].piece;
		gameBoard.board[pos2[0]-97][pos1[1]-49].piece = NULL;
		output << "e.p.";
	}
	else if(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->type == "King" && abs(pos2[0]-pos1[0]) == 2)
	{
		if(pos2[0]-pos1[0] > 0)
		{
			gameBoard.board[5][pos1[1]-49].piece = gameBoard.board[7][pos1[1]-49].piece;
			gameBoard.board[7][pos1[1]-49].piece = NULL;
			output << "0-0";
		}
		else
		{
			gameBoard.board[3][pos1[1]-49].piece = gameBoard.board[0][pos1[1]-49].piece;
			gameBoard.board[0][pos1[1]-49].piece = NULL;
			output << "0-0-0";
		}
	}
	else
	{
		output << pos1;
		output << '-';
		output << pos2;
	}
	gameBoard.board[pos2[0] -97][pos2[1] -49].piece = gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece;
	gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece = NULL;
	return;
}

void playMove(string& pos1, string& pos2, Board& gameBoard)
{
	if(gameBoard.board[pos2[0] -97][pos2[1] -49].piece != NULL)
	{
		delete gameBoard.board[pos2[0] -97][pos2[1] -49].piece;
	}
	else if((gameBoard.board[pos1[0] -97][pos1[1] -49].piece->type == "Pawn") && (abs(pos2[0]-pos1[0]) == 1))
	{
		delete gameBoard.board[pos2[0]-97][pos1[1]-49].piece;
		gameBoard.board[pos2[0]-97][pos1[1]-49].piece = NULL;
	}
	else if(gameBoard.board[pos1[0]-97][pos1[1]-49].piece->type == "King" && abs(pos2[0]-pos1[0]) == 2)
	{
		if(pos2[0]-pos1[0] > 0)
		{
			gameBoard.board[5][pos1[1]-49].piece = gameBoard.board[7][pos1[1]-49].piece;
			gameBoard.board[7][pos1[1]-49].piece = NULL;
		}
		else
		{
			gameBoard.board[3][pos1[1]-49].piece = gameBoard.board[0][pos1[1]-49].piece;
			gameBoard.board[0][pos1[1]-49].piece = NULL;
		}
	}
	gameBoard.board[pos2[0] -97][pos2[1] -49].piece = gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece;
	gameBoard.board[pos1[0] - 97][pos1[1] - 49].piece = NULL;
	return;
}

void promotePawn(string& pos, Board& gameBoard, ofstream& output)
{
	bool test = true;
	string promoted;
	while(test)
	{
		cout << "Your pawn has been promoted. What would you like in its place? (Q, N, B, R): ";
		cin >> promoted;
		if(promoted.length() != 1)
		{
			cout << "That is an invalid entry." << endl;
		}
		else if(promoted == "Q")
		{
			gameBoard.board[pos[0] - 97][pos[1]-49].piece->type = "Queen";
			test = false;
		}
		else if(promoted == "N")
		{
			gameBoard.board[pos[0] - 97][pos[1]-49].piece->type = "Night";
			test = false;
		}
		else if(promoted == "B")
		{
			gameBoard.board[pos[0] - 97][pos[1]-49].piece->type = "Bishop";
			test = false;
		}
		else if(promoted == "R")
		{
			gameBoard.board[pos[0] - 97][pos[1]-49].piece->type = "Rook";
			test = false;
		}
		else
		{
			cout << "That is an invalid entry." << endl;
		}
	}
	output << promoted;
	return;
}
