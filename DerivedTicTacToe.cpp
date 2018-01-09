// DerivedTicTacToe.cpp : Defines the methods for the tictactoe board
// Authors: F Huang
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, and inherits gamebase
// Is inherited by tictactoe.h and ultimatetictactoe.h

#include "stdafx.h"
#include "GameBase.h"
#include "GamePieces.h"
#include "DerivedTicTacToe.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

der_tictactoe::der_tictactoe() :
	gamebase(), curr_player(player::player::playerO), max_pieces(0), lower_bound(0), upper_bound(0)
{
};

der_tictactoe::der_tictactoe(int r_, int c_) :
	gamebase(r_, c_), max_pieces((r_ - 2)*(c_ - 2)), lower_bound(0), upper_bound(r_ - 1) //calls the parent class constructor (gamebase)
{
};

der_tictactoe::~der_tictactoe()
{
}


bool der_tictactoe::done()
{
	int coordinates = 0;
	int coordinates1 = 0;
	int coordinates2 = 0;

	for (size_t x_ = 1; x_ < col - 1; ++x_)
	{
		coordinates = col + x_;
		coordinates1 = coordinates + col;
		coordinates2 = coordinates + (2 * col);
		if (pieces[coordinates].piece_display.compare(" ") != 0) { //if display is not empty space
			if ((pieces[coordinates].piece_display.compare(pieces[coordinates1].piece_display) == 0) && (pieces[coordinates1].piece_display.compare(pieces[coordinates2].piece_display) == 0)) { //if vertical are equal
				winner = pieces[coordinates].piece_display;
				return true;
			}
		}
	}

	for (size_t y_ = 1; y_ < row - 1; ++y_)
	{
		coordinates = col*y_ + 1;
		coordinates1 = coordinates + 1;
		coordinates2 = coordinates + 2;
		if (pieces[coordinates].piece_display.compare(" ") != 0 && pieces[coordinates].piece_display.compare("") != 0) { //if display is not empty space
			if ((pieces[coordinates].piece_display.compare(pieces[coordinates1].piece_display) == 0) && (pieces[coordinates1].piece_display.compare(pieces[coordinates2].piece_display) == 0)) { //if horizontal are equal
				winner = pieces[coordinates].piece_display;
				return true;
			}
		}
	}

	int x_ = 1; //diagonal from (1,1) to (3,3)
	int y_ = 1;
	coordinates = col*y_ + x_;
	coordinates1 = col*(++y_) + (++x_);
	coordinates2 = col*(++y_) + (++x_);
	if (pieces[col*y_ + x_].piece_display.compare(" ") != 0 && pieces[col*y_ + x_].piece_display.compare("") != 0) { //if display is not empty space
		if ((pieces[coordinates].piece_display.compare(pieces[coordinates1].piece_display) == 0) && (pieces[coordinates1].piece_display.compare(pieces[coordinates2].piece_display) == 0))
		{
			winner = pieces[coordinates].piece_display;
			return true;
		}
	}

	x_ = 1; //diagonal from (1,3) to (3,1)
	y_ = 3;
	coordinates = col*y_ + x_;
	coordinates1 = col*(--y_) + (++x_);
	coordinates2 = col*(--y_) + (++x_);
	if (pieces[col*y_ + x_].piece_display.compare(" ") != 0 && pieces[col*y_ + x_].piece_display.compare("") != 0) { //if display is not empty space
		if ((pieces[coordinates].piece_display.compare(pieces[coordinates1].piece_display) == 0) && (pieces[coordinates1].piece_display.compare(pieces[coordinates2].piece_display) == 0))
		{
			winner = pieces[coordinates].piece_display;
			return true;
		}
	}
	return false;
}

bool der_tictactoe::draw()
{
	return num_pieces == max_pieces && !done(); //return if any valid spots are left
}