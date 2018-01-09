// DerivedTicTacToe.h : Declare the methods for the tictactoe board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, and inherits gamebase
// Is inherited by tictactoe.h and ultimatetictactoe.h

#pragma once
#include <vector>
#include <string>
#include "GamePieces.h"
#include "GameBase.h"
using namespace std;

namespace player {
	enum player { playerO, playerX };
}

class der_tictactoe : public gamebase { //public inheritance

public:
	der_tictactoe();
	der_tictactoe(int r_, int c_);
	virtual ~der_tictactoe();

protected:
	virtual bool done(); //return true if 3-Xs or Os is in a vertical, horizontal or diagnoal line
	virtual bool draw(); //false if moveBs are returned
	player::player curr_player;
	const int max_pieces;
	unsigned int lower_bound;
	unsigned int upper_bound;
	vector<string> playerOcoords;
	vector<string> playerXcoords;
};