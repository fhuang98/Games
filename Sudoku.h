#pragma once
// SudokuGame.h : Declare the methods for the gomoku board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, as well as the 
// overloaded << operator

#pragma once
#include <vector>
#include <string>
#include "GamePieces.h"
#include "GameBase.h"
using namespace std;

class sudokugame : public gamebase {
	static const int BOARD_SIZE = 9;

	friend ostream & operator << (ostream &, const sudokugame &);
	virtual void print() override; //override gamebase's print function
	virtual bool draw();
	virtual int turn();

public:
	sudokugame();
	sudokugame(const sudokugame & other);
	sudokugame(sudokugame && other);
	sudokugame & operator=(const sudokugame & other);
	sudokugame & operator=(sudokugame && other);
	~sudokugame();
	virtual bool done();

private:
	void fill(char c, int x_, int y_, bool mutate);

	bool mutation;
	unsigned int upper_bound;
	unsigned int lower_bound;
	vector<string> current_row; // will have n content with strings of the row
	vector<string> current_col; // will have n content with strings of the col
	vector<string> current_block; // will have contents of strings of diagonals from top left to bottom right, will calculate index by summing the coordinates
};

ostream & operator << (ostream &, const sudokugame &);