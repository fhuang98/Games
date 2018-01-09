// Gomoku.cpp : Initialize the methods for the gomoku board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play, as well as the
// overloaded << operator
// Inherits gamebase
#include "stdafx.h"
#include "GameBase.h"
#include "Gomoku.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stdlib.h> 
using namespace std;

gomoku::gomoku() : gamebase(), upper_bound(0), lower_bound(0), curr_color(black), avail_draw("xxxxx"), black_win("BBBBB"), white_win("WWWWW"), diagonal_total(0)
{
};

gomoku::gomoku(int r_, int c_) : gamebase(r_, c_), upper_bound(r_), lower_bound(1), curr_color(black), avail_draw("xxxxx"), black_win("BBBBB"), white_win("WWWWW"), diagonal_total(((r_ * 2) + ((r_ * 2) - 2)) / 2), win_tot(5)
{
	string content = ""; //can use one string because it's a nxn block

	for (int i = 0; i < c_; ++i)
	{
		content += "x"; // add 'blank' spaces into the string for however many columns they have
	}

	for (int j = 0; j < c_; ++j)
	{
		row_freq.push_back(c_);
		col_freq.push_back(c_);
		current_row.push_back(content);
		current_col.push_back(content); //add the 'empty' game board into the row and col vectors keeping track of the contents
	}

	all_length.push_back(""); //to help index into the vector easier so the index and the length of the string correlate

	for (int tot_string = 1; tot_string <= r_; ++tot_string)
	{
		string diag_content = "";
		for (int current_length = 0; current_length < tot_string; ++current_length)
		{
			diag_content += "x"; //get a string with all diagonals lenght's of x
		}
		all_length.push_back(diag_content); //push into vector to store the different vectors
	}

	for (int first_go = 1; first_go <= r_; ++first_go) {
		diag_sum_freq.push_back(c_);
		diag_diff_freq.push_back(c_);
		current_diag_sum.push_back(all_length[first_go]); //fill up with lengths up to the max
		current_diag_diff.push_back(all_length[first_go]);
	}

	for (int second_go = r_ - 1; second_go >= 1; --second_go)
	{
		diag_sum_freq.push_back(c_);
		diag_diff_freq.push_back(c_);
		current_diag_sum.push_back(all_length[second_go]); //fill in remaining diagonals
		current_diag_diff.push_back(all_length[second_go]);
	}

	bool saved = true;
	ifstream ifs;
	int error = 0;
	string line;
	int row_count = 0;
	vector<string> saved_row;
	ifs.open("gomoku");
	if (ifs.is_open())
	{
		string line;
		if (getline(ifs, line)) {
			if (line.compare("NO DATA") == 0 || line.compare("DONE") == 0) //if it is not a saved sodokugame file
			{
				saved = false;
				error = prompt_code::new_game;
			}
			else if (line.compare("gomoku") != 0)
			{
				row_count++;
				line.erase(remove(line.begin(), line.end(), ' '), line.end());
				saved_row.push_back(line);
			}
		}
		while (getline(ifs, line) && saved)
		{
			row_count++;
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			saved_row.push_back(line);
		}
	}

	ifs.close();

	if (row_count != row)
	{
		saved = false;
	}
	else
	{
		int r_ = row, c_ = col;
		int black_count = 0, white_count = 0;
		for (int i = 0; i < r_ && saved; i++)
		{
			string row_str = saved_row[i];
			if (row_str.length() != c_)
			{
				saved = false;
			}
			for (int j = 0; j < c_ && saved; j++)
			{
				char c = row_str[j];
				if (c != 'W' && c != 'B' && (c != 'x' && c != '0'))
				{
					saved = false;
				}
				else if (c == 'W')
				{
					white_count++;
				}
				else if (c == 'B')
				{
					black_count++;
				}
			}
		}
		if (abs(black_count - white_count) > 1)
		{
			saved = false;
		}
		else if (black_count > white_count)
		{
			curr_color = white;
			num_pieces = white_count + black_count;
		}
		else
		{
			curr_color = black;
			num_pieces = white_count + black_count;
		}
	}
	if (saved)
	{
		int r_ = row, c_ = col;
		for (int i = 0; i < r_; i++)
		{
			string row_str = saved_row[i];
			for (int j = 0; j < c_; j++)
			{
				if (row_str[j] != 'x' && row_str[j] != '0')
				{
					fill(row_str[j], j + 1, i + 1);
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
		else if (error == prompt_code::new_game)
		{
			cout << "New game started" << endl;
		}
	}
};

gomoku::gomoku(int r_, int c_, int win) : gomoku(r_, c_) {
	avail_draw = "";
	black_win = "";
	white_win = "";
	win_tot = win;
	for (int j = 0; j < c_; ++j)
	{
		row_freq.push_back(win);
		col_freq.push_back(win);
	}

	for (int i = 0; i < win; ++i) {
		avail_draw += "x";
		black_win += "B";
		white_win += "W";
	}
}

gomoku::gomoku(const gomoku & other) : gamebase(other)
{
	avail_draw = other.avail_draw;
	black_win = other.black_win;
	white_win = other.white_win;
	input = other.input;
	win_tot = other.input;
	diagonal_total = other.diagonal_total;
	curr_color = other.curr_color;
	lower_bound = other.lower_bound;
	upper_bound = other.upper_bound;
	row_freq = other.row_freq;
	col_freq = other.col_freq;
	diag_sum_freq = other.diag_sum_freq;
	diag_diff_freq = other.diag_diff_freq;
	current_row = other.current_row;
	current_col = other.current_col;
	all_length = other.all_length;
	current_diag_sum = other.current_diag_sum;
	current_diag_diff = other.current_diag_diff;
}
gomoku::gomoku(gomoku && other) : gamebase(other)
{
	avail_draw = other.avail_draw;
	black_win = other.black_win;
	white_win = other.white_win;
	input = other.input;
	win_tot = other.input;
	diagonal_total = other.diagonal_total;
	curr_color = other.curr_color;
	lower_bound = other.lower_bound;
	upper_bound = other.upper_bound;
	row_freq = other.row_freq;
	col_freq = other.col_freq;
	diag_sum_freq = other.diag_sum_freq;
	diag_diff_freq = other.diag_diff_freq;
	current_row = other.current_row;
	current_col = other.current_col;
	all_length = other.all_length;
	current_diag_sum = other.current_diag_sum;
	current_diag_diff = other.current_diag_diff;

	// reset 
	other.avail_draw.clear();
	other.black_win.clear();
	other.white_win.clear();
	other.input = false;
	other.win_tot = 0;
	other.diagonal_total = 0;
	other.curr_color = black;
	other.lower_bound = 0;
	other.upper_bound = 0;
	other.row_freq.clear();
	other.col_freq.clear();
	other.diag_sum_freq.clear();
	other.diag_diff_freq.clear();
	other.current_row.clear();
	other.current_col.clear();
	other.all_length.clear();
	other.current_diag_sum.clear();
	other.current_diag_diff.clear();
}
gomoku & gomoku::operator=(const gomoku & other)
{
	if (&other == this)
	{
		return *this;
	}
	gamebase::operator=(other);
	avail_draw = other.avail_draw;
	black_win = other.black_win;
	white_win = other.white_win;
	input = other.input;
	win_tot = other.input;
	diagonal_total = other.diagonal_total;
	curr_color = other.curr_color;
	lower_bound = other.lower_bound;
	upper_bound = other.upper_bound;
	row_freq = other.row_freq;
	col_freq = other.col_freq;
	diag_sum_freq = other.diag_sum_freq;
	diag_diff_freq = other.diag_diff_freq;
	current_row = other.current_row;
	current_col = other.current_col;
	all_length = other.all_length;
	current_diag_sum = other.current_diag_sum;
	current_diag_diff = other.current_diag_diff;
	return *this;

}
gomoku & gomoku::operator=(gomoku && other)
{
	if (&other == this)
	{
		return *this;
	}
	gamebase::operator=(other);
	avail_draw = other.avail_draw;
	black_win = other.black_win;
	white_win = other.white_win;
	input = other.input;
	win_tot = other.input;
	diagonal_total = other.diagonal_total;
	curr_color = other.curr_color;
	lower_bound = other.lower_bound;
	upper_bound = other.upper_bound;
	row_freq = other.row_freq;
	col_freq = other.col_freq;
	diag_sum_freq = other.diag_sum_freq;
	diag_diff_freq = other.diag_diff_freq;
	current_row = other.current_row;
	current_col = other.current_col;
	all_length = other.all_length;
	current_diag_sum = other.current_diag_sum;
	current_diag_diff = other.current_diag_diff;

	// reset 
	other.avail_draw.clear();
	other.black_win.clear();
	other.white_win.clear();
	other.input = false;
	other.win_tot = 0;
	other.diagonal_total = 0;
	other.curr_color = black;
	other.lower_bound = 0;
	other.upper_bound = 0;
	other.row_freq.clear();
	other.col_freq.clear();
	other.diag_sum_freq.clear();
	other.diag_diff_freq.clear();
	other.current_row.clear();
	other.current_col.clear();
	other.all_length.clear();
	other.current_diag_sum.clear();
	other.current_diag_diff.clear();
	return *this;
}

gomoku::~gomoku()
{

}
void gomoku::print()
{
	cout << *this;
}

bool gomoku::draw() {
	bool draw_game = true;

	for (size_t i = 0; i < row; ++i)
	{
		string col_assignment = current_col[i];
		string row_assignment = current_row[i];
		for (size_t j = 0; j < row; ++j) {
			char col_char = col_assignment[j];
			size_t k;
			for (k = j + 1; k < j + 5 && k < row; ++k)
			{
				if (col_assignment[k] != col_char && col_assignment[k] != 'x') {
					break;
				}
			}
			if (k == j + 5)
			{
				return false;
			}
			char row_char = row_assignment[j];
			for (k = j + 1; k < j + 5 && k < row; ++k)
			{
				if (row_assignment[k] != row_char && row_assignment[k] != 'x') {
					break;
				}
			}
			if (k == j + 5)
			{
				return false;
			}
		}
	}

	for (int i = 0; i < diagonal_total; ++i)
	{
		string diag_sum_assignment = current_diag_sum[i];
		string diag_diff_assignment = current_diag_diff[i];
		int diag_size = diag_diff_assignment.size();
		for (int j = 0; j < diag_size; ++j) {
			char sum_char = diag_sum_assignment[j];
			int k;
			for (k = j + 1; k < j + 5 && k < diag_size; ++k)
			{
				if (diag_sum_assignment[k] != sum_char && diag_sum_assignment[k] != 'x') {
					break;
				}
			}
			if (k == j + 5)
			{
				return false;
			}
			char diff_char = diag_diff_assignment[j];
			for (k = j + 1; k < j + 5 && k < diag_size; ++k)
			{
				if (diag_diff_assignment[k] != diff_char && diag_diff_assignment[k] != 'x') {
					break;
				}
			}
			if (k == j + 5)
			{
				return false;
			}
		}
	}
	return draw_game;
}

int gomoku::turn() {
	unsigned int x_ = 0;
	unsigned int y_ = 0;
	int status = 0;
	int coordinates = 0;
	while ((((x_ > upper_bound || x_ < lower_bound) || (y_ > upper_bound || y_ < lower_bound) || ((x_ >= lower_bound && x_ <= upper_bound && y_ >= lower_bound && y_ <= upper_bound) && pieces[coordinates].piece_display.compare(" ") != 0)))) //if it is not quitting or invalid coordinate or piece already there, continue
	{
		try {
			status = prompt(x_, y_);
		}
		catch (int x) {
			throw x;
		}
		coordinates = col*(y_ - 1) + (x_ - 1);
	}

	//if did not quit, print
	++num_pieces;

	char player = ' ';
	if (curr_color == black)
	{
		player = 'B';
		curr_color = white;
	}
	else
	{
		player = 'W';
		curr_color = black;
	}

	fill(player, x_, y_);
	cout << *this << endl; //print out the board
	row_freq[y_ - 1] = row_freq[y_ - 1] - 1;
	col_freq[x_ - 1] = col_freq[x_ - 1] - 1;
	diag_sum_freq[(y_ + x_) - 2] = diag_sum_freq[(y_ + x_) - 2] - 1;
	diag_diff_freq[(y_ - x_) + row - 1] = diag_diff_freq[(y_ - x_) + row - 1] - 1; //subtract free spaces left
	return status;
}

bool gomoku::done()
{
	bool win_game = false;

	for (size_t i = 0; i < row && !win_game; ++i)
	{
		if (current_col[i].find(black_win) != string::npos || current_row[i].find(black_win) != string::npos) { //black has n of B consecutively where n is the number needed to win on a row or column
			win_game = true;
			winner = "Black";
		}
		else if (current_col[i].find(white_win) != string::npos || current_row[i].find(white_win) != string::npos) { //white has n of B consecutively where n is the number needed to win on a row or column
			win_game = true;
			winner = "White";
		}
	}

	for (int j = 0; j < diagonal_total && !win_game; ++j)
	{
		if (current_diag_sum[j].find(black_win) != string::npos || current_diag_diff[j].find(black_win) != string::npos)
		{
			win_game = true;
			winner = "Black";
		}
		else if (current_diag_sum[j].find(white_win) != string::npos || current_diag_diff[j].find(white_win) != string::npos) {
			win_game = true;
			winner = "White";
		}
	}
	return win_game;
}

void gomoku::fill(char character, int x_, int y_)
{
	int coordinates = col*(y_ - 1) + (x_ - 1);
	pieces[coordinates].piece_display = character;
	current_col[x_ - 1].replace(y_ - 1, 1, &character);
	current_row[y_ - 1].replace(x_ - 1, 1, &character);

	if ((y_ + x_) - 2 < (int)row)
	{
		current_diag_sum[x_ + y_ - 2].replace(x_ - 1, 1, &character);
	}
	else
	{
		current_diag_sum[y_ + x_ - 2].replace(row - y_, 1, &character);
	}

	int d = row - y_ + x_ - 1;
	if (d < (int)row)
	{
		current_diag_diff[d].replace(x_ - 1, 1, &character);
	}
	else
	{
		current_diag_diff[d].replace(y_ - 1, 1, &character);
	}
}

ostream & operator << (ostream & o, const gomoku & g_game)
{
	size_t num_row = g_game.row;
	size_t num_col = g_game.col;
	int longest = g_game.longest;

	vector<game_piece> pieces = g_game.pieces;
	for (size_t row = num_row; row != 0; --row) //print until 1
	{
		cout << setw(longest) << left << row << " ";
		for (size_t col = 0; col < num_col; ++col) //print out every coordinate in the vector
		{
			cout << setw(longest) << left << pieces[(num_col)*(row - 1) + col].piece_display << " "; //adds the length of the longest display string in white space + an extra space
		}
		cout << endl;
	}

	cout << setw(longest) << left << "X" << " "; //subtract one to account for the X character

	for (size_t col = 1; col <= num_col; ++col) { //start from 1

		cout << setw(longest) << left << col << " "; //adds the length of the longest display string in white space + an extra space
	}
	cout << endl;
	return o;
}
