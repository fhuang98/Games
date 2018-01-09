// UltimateTicTacToe.cpp : Defines the methods for the utlimate tictactoe board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods 
// inherits der_tictactoe and calls objects of tictactoe


#include "stdafx.h"
#include "TicTacToe.h"
#include "DerivedTicTacToe.h"
#include "UltimateTicTacToe.h"
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

static int convert[] = { 6,7,8,11,12,13,16,17,18 };

ult_tictactoe::ult_tictactoe() : der_tictactoe(), total_pieces(0), tot_row(0), tot_col(0), last_cell(0), winner_board("") {
};

ult_tictactoe::ult_tictactoe(int r_, int c_) : der_tictactoe((r_ / 3) + 2, (c_ / 3) + 2), total_pieces(0), winner_board(""), win(r_ / 3, c_ / 3, r_), last_cell(r_ + 1), upper_bound(r_), tot_row(r_), tot_col(c_) {
	for (int i = 0; i < r_ - 1; ++i) //add in 9 games of size 5x5
	{
		tictactoe game((r_ / 3) + 2, (c_ / 3) + 2, i);
		games.push_back(game);
	}

	game_piece default_piece(" ", true);
	for (int size = 0; size < r_*c_; ++size)
	{
		ult_pieces.push_back(default_piece);
	}
}

ult_tictactoe::~ult_tictactoe()
{
}

int ult_tictactoe::turn()
{
	unsigned int x_ = 0;
	unsigned int y_ = 0;
	int status = prompt_code::success;
	int coordinates = tot_col*y_ + x_;
	int cell_coords = -1;
	while (status != prompt_code::quit && (((x_ >= upper_bound || x_ <= lower_bound) || (y_ >= upper_bound || y_ <= lower_bound) || ((x_ > lower_bound && x_ < upper_bound && y_ > lower_bound && y_ < upper_bound) && ult_pieces[coordinates].piece_display.compare(" ") != 0)))) //if it is not quitting or invalid coordinate or piece already there, continue
	{
		cell_coords = -1;
		if (winner_board.find(to_string(last_cell)) != string::npos) { //if the square has already been won or has a draw
			last_cell = tot_row + 1;
		}

		if (last_cell != tot_row + 1) { //if cannot find the number for a winning game
			while (cell_coords != last_cell) {
				try {
					status = prompt(x_, y_);
					cell_coords = (tot_col / 3)*((y_ - 1) / (tot_col / 3)) + ((x_ - 1) / (tot_col / 3));
				}
				catch (int x)
				{
					if (x == prompt_code::quit)
					{
						throw x;
					}
				}
			}
		}
		else
		{
			try { //beginning of game, can go anywhere
				status = prompt(x_, y_);
				cell_coords = (tot_col / 3)*((y_ - 1) / (tot_col / 3)) + ((x_ - 1) / (tot_col / 3));
			}
			catch (int x)
			{
				if (x == prompt_code::quit)
				{
					throw x;
				}
			}
		}
		coordinates = tot_col*y_ + x_;
	}
	if (status == prompt_code::success) { //if did not quit, print
		++total_pieces; //add to total number of pieces on the board
		if (total_pieces % 9 == 0)
		{
			++num_pieces;
		}

		if (curr_player == player::player::playerO)
		{
			games[cell_coords].turn(((x_ - 1) % 3) + 1, ((y_ - 1) % 3) + 1, curr_player); // TODO: if 
			coordinates = tot_col*y_ + x_;
			ult_pieces[coordinates].piece_display = "O";
			playerOcoords.push_back(to_string(x_) + "," + to_string(y_)); //store all the coordinates for playerO
			cout << *this << endl; //print out the board
			cout << "Player O: "; //print out the coordinates player O had
			size_t i;
			for (i = 0; i < playerOcoords.size() - 1; ++i)
			{
				cout << playerOcoords[i] << "; ";
			}
			cout << playerOcoords[i] << endl; //print out last one without semicolon
			cout << endl; //extra line
			curr_player = player::player::playerX; //switch player
		}
		else
		{
			games[cell_coords].turn(((x_ - 1) % 3) + 1, ((y_ - 1) % 3) + 1, curr_player);
			coordinates = tot_col*y_ + x_;
			ult_pieces[coordinates].piece_display = "X";
			playerXcoords.push_back(to_string(x_) + "," + to_string(y_));
			cout << *this << endl;
			cout << "Player X: ";
			size_t i;
			for (i = 0; i < playerXcoords.size() - 1; ++i)
			{
				cout << playerXcoords[i] << "; ";
			}
			cout << playerXcoords[i] << endl; //print out last one without semicolon
			cout << endl; //extra line
			curr_player = player::player::playerO; //switch player
		}
		last_cell = games[cell_coords].cell;
		int converter = convert[cell_coords];
		if (pieces[converter].piece_display == " ")//if dont already have winner
		{
			if (games[cell_coords].get_done())
			{
				pieces[converter].piece_display = games[cell_coords].get_winner();
				winner_board += to_string(cell_coords);
				last_cell = tot_row + 1;
			}
			else if (games[cell_coords].get_draw())
			{
				pieces[converter].piece_display = "";
				winner_board += to_string(cell_coords);
				last_cell = tot_row + 1;
			}
		}
	}
	return status;
}

void ult_tictactoe::print()
{
	cout << *this;
}

ostream & operator << (ostream & o, const ult_tictactoe & t_game)
{
	size_t num_row = t_game.tot_row;
	size_t num_col = t_game.tot_col;
	vector<game_piece> pieces = t_game.ult_pieces;
	for (size_t row = num_row - 1; row != (size_t)(-1); --row)
	{
		cout << row << " ";
		for (size_t col = 0; col < num_col; ++col) //print out every coordinate in the vector
		{
			cout << pieces[num_col*row + col].piece_display << setw(t_game.longest) << " "; //adds the length of the longest display string in white space + an extra space
		}
		cout << endl;
	}

	cout << "  ";

	for (size_t col = 0; col < num_col; ++col) {

		cout << col << setw(t_game.longest) << " "; //adds the length of the longest display string in white space + an extra space
	}
	cout << endl;
	return o;
}