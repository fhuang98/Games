// TicTacToe.cpp : Defines the methods for the tictactoe board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, as well as the 
// overloaded << operator

#include "stdafx.h"
#include "GamePieces.h"
#include "DerivedTicTacToe.h"
#include "TicTacToe.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

tictactoe::tictactoe() :
	der_tictactoe()
{
};

tictactoe::tictactoe(int r_, int c_) :
	der_tictactoe(r_, c_) //calls the parent class constructor (der_tictactoe)
{
	ifstream ifs;
	int error = 0;
	string line;
	string character;
	int O_count = 0;
	int X_count = 0;
	bool data = true;
	ifs.open("tictactoe"); //TODO: throw exception
	if (ifs.is_open())
	{
		string line;
		int count = 0;
		vector<string> saved_row;
		if (getline(ifs, line))
		{
			if (line.compare("COMPLETE") == 0 || line.compare("NO DATA") == 0)
			{
				data = false;
				error = prompt_code::new_game;
			}
			else if (line.compare("tictactoe") != 0 && data)
			{
				++count;
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				saved_row.push_back(line);
			}
		}

		while (getline(ifs, line) && data)
		{
			++count;
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			saved_row.push_back(line);
		}

		ifs.close();

		if (count != r_ + 2 && data) //two extra for the player O and player X 
		{
			data = false;
			error = prompt_code::wrong_num_lines;
		}
		else {
			int num_row = 0;
			for (; num_row < r_ && data; ++num_row)
			{
				string row = saved_row[num_row];
				if (row.length() != c_)
				{
					data = false;
					error = prompt_code::wrong_length_input;
				}
				for (int num_col = 0; num_col < c_ && data; ++num_col)
				{
					if (row[num_col] != 'X' && row[num_col] != 'O' && row[num_col] != '0') {
						data = false;
						error = prompt_code::invalid_input;
					}
					else
					{
						if (row[num_col] == 'X')
						{
							++X_count;
						}
						else if (row[num_col] == 'O')
						{
							++O_count;
						}
						if (row[num_col] == 'X' || row[num_col] == 'O') {
							pieces[c_*num_row + num_col].piece_display = row[num_col];
						}
					}
				}
			}

			for (; num_row < r_ + 2 && data; ++num_row)
			{
				string row = saved_row[num_row];
				row.erase(remove(row.begin(), row.end(), ' '), row.end());
				player::player play_list;
				string excess = "PlayerO:";

				if (row.length() < excess.length())
				{
					data = false;
					error = prompt_code::invalid_input;
				}

				if (data)
				{
					if (row.find("PlayerO") == 0) {
						play_list = player::player::playerO;
					}
					else if (row.find("PlayerX") == 0) {
						play_list = player::player::playerX;
					}
					else {
						data = false;
						error = prompt_code::invalid_input;
					}

					if (data) {
						row = row.substr(excess.length());
						row.erase(remove(row.begin(), row.end(), ','), row.end());
						row.erase(remove(row.begin(), row.end(), ';'), row.end());
						if (row.length() % 2 == 0)
						{
							if (play_list == player::player::playerO) {
								if (row.length() / 2 == O_count) //elae
								{
									for (size_t num_col = 0; num_col < row.length(); num_col = num_col + 2)
									{
										string add;
										add += row[num_col];
										add += ",";
										add += row[num_col + 1];
										playerOcoords.push_back(add);
									}
								}
							}
							else if (play_list == player::player::playerX) {
								if (row.length() / 2 == X_count)
								{
									for (size_t num_col = 0; num_col < row.length(); num_col = num_col + 2)
									{
										string add;
										add += row[num_col];
										add += ",";
										add += row[num_col + 1];
										playerXcoords.push_back(add);
									}
								}
							}
						}
						else
						{
							data = false;
							error = prompt_code::invalid_input;
						}
					}
				}
			}
		}
	}
	else
	{
		error = prompt_code::open_file_fail;
	}

	ifs.close();

	if (abs(O_count - X_count) > 1) { //if difference between number of O and X
		data = false;
		error = prompt_code::invalid_input;
	}
	else if (O_count == X_count + 1) { //more O than X
		curr_player = player::player::playerX;
	}
	else {
		curr_player = player::player::playerO;
	}

	if (!data)
	{
		if (error == prompt_code::invalid_input)
		{
			cout << "Unexpected character or information in the file" << endl;
		}
		else if (error == prompt_code::wrong_length_input)
		{
			cout << "Wrong number of coluumns in the file " << endl;
		}
		else if (error == prompt_code::open_file_fail) {
			cout << "Failed to open input file" << endl;
		}
		else if (error == prompt_code::wrong_num_lines) {
			cout << "Wrong number of lines in the file" << endl;
		}
		else if (error == prompt_code::new_game)
		{
			cout << "New game started" << endl;
		}
	}
	num_pieces = O_count + X_count;
};

tictactoe::tictactoe(int r_, int c_, int ult_cell) : der_tictactoe(r_, c_), cell(ult_cell)
{
};


void tictactoe::print()
{
	cout << *this;
}

void tictactoe::save(string status) {
	ofstream output_file;
	string class_excess = "class ";
	string class_name = typeid(*this).name();
	class_name = class_name.substr(class_excess.length());
	string input = class_name;

	output_file.open(input); //case insensitive
	if (output_file.is_open()) {
		if (status.compare("yes") == 0) // if save
		{
			output_file << class_name << endl;
		}
		else if (status.compare("done") == 0)
		{
			output_file << "COMPLETE" << endl;
		}
		else { //if not save
			output_file << "NO DATA" << endl;
		}
		if (status.compare("yes") == 0 || status.compare("done") == 0)
		{
			for (size_t r_ = 0; r_ < row; ++r_)
			{
				for (size_t c_ = 0; c_ < col; ++c_) //print out every coordinate in the vector
				{
					if (pieces[col*r_ + c_].piece_display.compare(" ") == 0)
					{
						output_file << "0" << " ";
					}
					else
					{
						output_file << pieces[col*r_ + c_].piece_display << " ";
					}
				}
				output_file << endl;
			}

			output_file << "Player O: "; //print out the coordinates player O had
			size_t i;
			for (i = 0; i < playerOcoords.size(); ++i)
			{
				output_file << playerOcoords[i] << "; ";
			}
			output_file << endl;

			output_file << "Player X: ";

			for (i = 0; i < playerXcoords.size(); ++i)
			{
				output_file << playerXcoords[i] << "; ";
			}
			output_file << endl;
		}
		output_file.close();
	}
	else
	{
		throw prompt_code::open_file_fail;
	}
}

tictactoe::~tictactoe()
{

}

bool tictactoe::get_done()
{
	return done();
}

bool tictactoe::get_draw()
{
	return draw();
}
int tictactoe::turn()
{
	unsigned int x_ = 0;
	unsigned int y_ = 0;
	int status = prompt_code::success;
	int coordinates = col*y_ + x_;
	while (status != prompt_code::quit && (((x_ >= upper_bound || x_ <= lower_bound) || (y_ >= upper_bound || y_ <= lower_bound) || ((x_ > lower_bound && x_ < upper_bound && y_ > lower_bound && y_ < upper_bound) && pieces[coordinates].piece_display.compare(" ") != 0)))) //if it is not quitting or invalid coordinate or piece already there, continue
	{
		try {
			status = prompt(x_, y_);
		}
		catch (int x)
		{
			if (x == prompt_code::quit)
			{
				throw x;
			}
		}
		coordinates = col*y_ + x_;
	}
	if (status == prompt_code::success) { //if did not quit, print
		++num_pieces; //add to total number of pieces on the board
		if (curr_player == player::player::playerO)
		{
			coordinates = col*y_ + x_;
			pieces[coordinates].piece_display = "O";
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
			coordinates = col*y_ + x_;
			pieces[coordinates].piece_display = "X";
			playerXcoords.push_back(to_string(x_) + "," + to_string(y_)); //store all the coordinates for playerO
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
	}
	return status;
}

int tictactoe::turn(unsigned int x_, unsigned int y_, player::player curr_play)
{
	int coordinates = col*y_ + x_;
	if ((x_ > lower_bound && x_ < upper_bound && y_ > lower_bound && y_ < upper_bound) && pieces[coordinates].piece_display.compare(" ") == 0) {
		++num_pieces;
		if (curr_play == player::player::playerO)
		{
			coordinates = col*y_ + x_;
			pieces[coordinates].piece_display = "O";
			cell = (row-2)*(y_ - 1) + (x_ - 1);
		}
		else
		{
			coordinates = col*y_ + x_;
			pieces[coordinates].piece_display = "X";
			playerXcoords.push_back(to_string(x_) + "," + to_string(y_)); //store all the coordinates for playerO
			cell = (row - 2)*(y_ - 1) + (x_ - 1);
		}
	}
	return prompt_code::success;
}

string tictactoe::get_winner() {
	return winner;
}

ostream & operator << (ostream & o, const tictactoe & t_game)
{
	size_t num_row = t_game.row;
	size_t num_col = t_game.col;
	vector<game_piece> pieces = t_game.pieces;
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

