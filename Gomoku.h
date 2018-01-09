// Gomoku.h : Declare the methods for the gomoku board
// Authors: F. Huang
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, as well as the 
// overloaded << operator

#pragma once
#include <vector>
#include <string>
#include "GamePieces.h"
using namespace std;

enum color { black, white };
class gomoku : public gamebase {
	friend ostream & operator << (ostream &, const gomoku &);
	virtual void print() override; //override gamebase's print function
	virtual bool draw();
	virtual int turn();
	//virtual void save(string status);

public:
	gomoku();
	gomoku(int r_, int c_);
	gomoku(int r_, int c_, int win);
	gomoku(const gomoku & other);
	gomoku(gomoku && other);
	gomoku & operator=(const gomoku & other);
	gomoku & operator=(gomoku && other);
	~gomoku();

	virtual bool done();

private:
	string avail_draw;
	string black_win;
	string white_win;
	bool input;
	void fill(char character, int x_, int y_);
	int win_tot;
	int diagonal_total; //total diagonals divided by 2, also size of teh two vectors holding the diagonals 
	color curr_color; //black stone always goes first
	unsigned int lower_bound;
	unsigned int upper_bound;
	vector<int> row_freq;
	vector<int> col_freq;
	vector<int> diag_sum_freq;
	vector<int> diag_diff_freq;
	vector<string> current_row; // will have n content with strings of the row
	vector<string> current_col; // will have n content with strings of the col
	vector<string> all_length; //stores all the different lengths a diagonal can have
	vector<string> current_diag_sum; // will have contents of strings of diagonals from top left to bottom right, will calculate index by summing the coordinates
	vector<string> current_diag_diff; //will have contents of strings of diagonals from top right to bottom left, will calculate index by subtracting the coordinates
};

ostream & operator << (ostream &, const gomoku &);