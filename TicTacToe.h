// TicTacToe.h : Declare the methods for the tictactoe board
// Authors: F Huang
// Contains the constructors and the definitions for the methods and inherites a base class, der_tictactoe
#pragma once
#include <vector>
#include <string>
#include "GamePieces.h"
#include "DerivedTicTacToe.h"
using namespace std;

class tictactoe : public der_tictactoe { //public inheritance
	friend ostream & operator << (ostream &, const tictactoe &);

public:
	tictactoe();
	tictactoe(int r_, int c_);
	tictactoe(int r_, int c_, int ult_cell);
	~tictactoe();
	int turn(unsigned int x_, unsigned int y, player::player curr_play);
	int cell;
	string get_winner();
	bool get_done();
	bool get_draw();

protected:
	virtual int turn() override;
	virtual void print() override;
	virtual void save(string status) override;

private:
	player::player curr_player;
	vector<string> playerOcoords;
	vector<string> playerXcoords;
};

ostream & operator << (ostream &, const tictactoe &);