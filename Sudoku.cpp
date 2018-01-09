// sudoku.cpp : Initialize the methods for the sudoku board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, as well as the
// overloaded << operator

#include "stdafx.h"
#include "GameBase.h"
#include "Sudoku.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

sudokugame::sudokugame() : gamebase(BOARD_SIZE, BOARD_SIZE), mutation(false), upper_bound(8), lower_bound(0)
{
	num_pieces = 0;
	int error = 0;
	string content = ""; //can use one string because it's a nxn block
	for (int i = 0; i < BOARD_SIZE; ++i)
	{
		content += "0"; // add 'blank' spaces into the string for however many columns they have
	}
	for (int j = 0; j < BOARD_SIZE; ++j)
	{
		current_row.push_back(content);
		current_col.push_back(content); //add the 'empty' game board into the row and col vectors keeping track of the contents
		current_block.push_back(content);
	}

	bool saved = true;
	vector<string> saved_row;
	ifstream ifs;
	string line;
	int count = 0;
	ifs.open("sudokugame");
	if (ifs.is_open())
	{
		if (getline(ifs, line)) {
			if (line.compare("NO DATA") == 0 || line.compare("COMPLETE") == 0) //if it is not a saved sodokugame file
			{
				ifs.close();
				ifs.open("sudoku0");
				error = prompt_code::new_game;
			}
			else if (line.compare("sudokugame") != 0)
			{
				++count;
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				saved_row.push_back(line);
			}
		}
	}
	else
	{
		ifs.close();
		ifs.open("sudoku0");
		error = prompt_code::open_file_fail;
	}
	if (ifs.is_open()) {
		while (saved && getline(ifs, line))
		{
			++count;
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			saved_row.push_back(line);
		}
	}
	else
	{
		saved = false;
		error = prompt_code::open_file_fail;
	}
	ifs.close();

	if (saved && count != BOARD_SIZE)
	{
		saved = false;
		error = prompt_code::wrong_num_lines;
	}
	else
	{
		for (int i = 0; i < BOARD_SIZE && saved; i++)
		{
			string row = saved_row[i];
			if (row.length() != BOARD_SIZE) //if the dimensions are not right
			{
				saved = false;
				error = prompt_code::wrong_length_input;
			}
			for (int j = 0; j < BOARD_SIZE && saved; j++)
			{
				if (!isAllDigit(row)) //if not all numbers
				{
					saved = false;
					error = prompt_code::invalid_input;
				}
			}
		}
	}

	if (saved)
	{
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			string row = saved_row[i];
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				if (row[j] != '0')
				{
					fill(row[j], j, i, mutation);
				}
			}
		}
	}
	else
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
		else if (error == prompt_code::new_game) {
			cout << "Default game loaded" << endl;
		}

	}
	mutation = true;
};

sudokugame::sudokugame(const sudokugame & other) : gamebase(other) {
	mutation = other.mutation;
	upper_bound = other.upper_bound;
	lower_bound = other.lower_bound;
	current_row = other.current_row;
	current_col = other.current_col;
	current_block = other.current_block;
}

sudokugame::sudokugame(sudokugame && other) : gamebase(other) {
	mutation = other.mutation;
	upper_bound = other.upper_bound;
	lower_bound = other.lower_bound;
	current_row = other.current_row;
	current_col = other.current_col;
	current_block = other.current_block;

	//reset
	other.mutation = false;
	other.upper_bound = 0;
	other.lower_bound = 0;
	other.current_row.clear();
	other.current_col.clear();
	other.current_block.clear();
}

sudokugame & sudokugame:: operator=(const sudokugame & other)
{
	if (&other == this)
	{
		return *this;
	}
	gamebase::operator=(other);
	mutation = other.mutation;
	upper_bound = other.upper_bound;
	lower_bound = other.lower_bound;
	current_row = other.current_row;
	current_col = other.current_col;
	current_block = other.current_block;
	return *this;
}

sudokugame & sudokugame:: operator=(sudokugame && other)
{
	if (&other == this)
	{
		return *this;
	}
	gamebase::operator=(other);
	mutation = other.mutation;
	upper_bound = other.upper_bound;
	lower_bound = other.lower_bound;
	num_pieces = other.num_pieces;
	current_row = other.current_row;
	current_col = other.current_col;
	current_block = other.current_block;
	other.mutation = false;
	other.upper_bound = 0;
	other.lower_bound = 0;
	other.current_row.clear();
	other.current_col.clear();
	other.current_block.clear();
	return *this;
}

sudokugame::~sudokugame()
{
}

bool sudokugame::draw() {
	return false;
}

int sudokugame::turn() {
	unsigned int x_ = 0;
	unsigned int y_ = 0;
	unsigned int val = 0;
	int status = 0;

	while ((val <= lower_bound || val > upper_bound + 1) || ((x_ > upper_bound || x_ < lower_bound) || (y_ > upper_bound || y_ < lower_bound) || ((x_ >= lower_bound && x_ <= upper_bound && y_ >= lower_bound && y_ <= upper_bound) && !pieces[BOARD_SIZE*y_ + x_].mutate))) //if it is not quitting or invalid coordinate or piece already there, continue
	{
		try {
			prompt(x_, y_, val);
		}
		catch (int x) {
			throw x;
		}
	}
	//if did not quit, print

	char ch = '0' + val;
	fill(ch, x_, y_, mutation);
	print();
	return status;
}

void sudokugame::print()
{
	cout << *this;
}

bool sudokugame::done()
{
	bool done_game = false;

	if (num_pieces == BOARD_SIZE * BOARD_SIZE) {
		for (unsigned int i = 0; i < BOARD_SIZE; i++)  // assune row = col all the time
		{
			string row_tiles = current_row[i];
			string col_tiles = current_col[i];
			string block_tiles = current_block[i];
			for (unsigned int j = 0; j < BOARD_SIZE; j++)
			{
				char r = row_tiles[j];
				char c = col_tiles[j];
				char b = block_tiles[j];
				for (unsigned int k = j + 1; k < BOARD_SIZE; k++)
				{
					if (r == row_tiles[k] || c == col_tiles[k] || b == block_tiles[k])
					{
						return done_game;
					}
				}
			}
		}
		done_game = true;
	}
	return done_game;
}

void sudokugame::fill(char c, int x_, int y_, bool mutation)
{
	if (current_row[y_][x_] == '0') {
		++num_pieces;
	}
	int coordinates = col*y_ + x_;
	pieces[coordinates].piece_display = c;
	pieces[coordinates].mutate = mutation;
	current_row[y_].replace(x_, 1, &c);
	current_col[x_].replace(y_, 1, &c);
	int block_idx = ((y_) / 3) * 3 + (x_) / 3;
	int in_block_idx = ((y_) % 3) * 3 + (x_) % 3;
	current_block[block_idx].replace(in_block_idx, 1, &c);
}

ostream & operator << (ostream & o, const sudokugame & g_game)
{
	size_t num_row = g_game.row;
	size_t num_col = g_game.col;
	int longest = g_game.longest;
	string in_filler = "-";
	string block_filler = "=";
	string filler;
	int coordinates;

	vector<game_piece> pieces = g_game.pieces;
	for (size_t row = num_row - 1; row != (size_t)(-1); --row) //print until 1
	{
		cout << row << "||";
		for (size_t col = 0; col < num_col; ++col) //print out every coordinate in the vector
		{
			coordinates = num_row * row + col;
			cout << pieces[coordinates].piece_display << "|"; //adds the length of the longest display string in white space + |
			if (col % 3 == 2) //if end of a block
			{
				cout << "|";
			}
		}

		cout << endl;

		if (row % 3 == 0)
		{
			filler = block_filler;
		}
		else
		{
			filler = in_filler;
		}

		cout << filler << "||";
		for (size_t col = 0; col < num_col; ++col)
		{
			cout << filler << "|";
			if (col % 3 == 2) //if end of a block
			{
				cout << "|";
			}
		}
		cout << endl;
	}

	cout << "#||";

	for (size_t col = 0; col < num_col; ++col) { //start from 1
		cout << col << "|";
		if (col % 3 == 2) //if end of a block
		{
			cout << "|";
		}
	}
	cout << endl;
	return o;
}
