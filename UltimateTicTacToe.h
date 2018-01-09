// UltimateTicTacToe.h : Declare the methods for the tictactoe board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods following the rules of ultimatetictactoe.
// Inherits der_tictactoe and uses objects tictactoe

#pragma once
#include "TicTacToe.h"
#include "DerivedTicTacToe.h"

class ult_tictactoe : public der_tictactoe{ //public inheritance
	friend ostream & operator << (ostream &, const ult_tictactoe &);

public:
	ult_tictactoe();
	ult_tictactoe(int r_, int c_);
	~ult_tictactoe();

protected:
	virtual int turn() override;
	virtual void print() override;

private:
	player::player curr_player;
	int total_pieces;
	int tot_row;
	int tot_col;
	unsigned int lower_bound;
	unsigned int upper_bound;
	int last_cell;
	vector<tictactoe> games;
	vector<game_piece> ult_pieces;
	string winner_board;
	tictactoe win;
	vector<string> playerOcoords;
	vector<string> playerXcoords;
};

ostream & operator << (ostream &, const ult_tictactoe &);