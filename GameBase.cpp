// GameBase.cpp : Defines the methods for the gamebase
// Authors: F Huang 
// Contains the constructors and the definitions for the methods save, prompt, instance, play, isAllDigit

#include "stdafx.h"
#include "Sudoku.h"
#include "Gomoku.h"
#include "DerivedTicTacToe.h"
#include "TicTacToe.h"
#include "UltimateTicTacToe.h"
#include "GameBase.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <iomanip>
using namespace std;

shared_ptr<gamebase> gamebase::game_base = nullptr;

gamebase::gamebase() :
	row(0), col(0), num_pieces(0), winner(""), longest(0) {
};

gamebase::gamebase(int r_, int c_) :
	row(r_), col(c_), num_pieces(0), winner(""), longest(to_string(r_).length()) {
	//add in all the default pieces
	game_piece default_piece(" ", true);
	for (int size = 0; size < r_*c_; ++size)
	{
		pieces.push_back(default_piece);
	}
};

gamebase::gamebase(const gamebase & other)
{
	row = other.row;
	col = other.col;
	num_pieces = other.num_pieces;
	longest = other.longest;
	winner = other.winner;
	pieces = other.pieces;
	game_choices = other.game_choices;
};

gamebase::gamebase(gamebase && other)
{
	row = other.row;
	col = other.col;
	num_pieces = other.num_pieces;
	longest = other.longest;
	winner = other.winner;
	pieces = other.pieces;
	game_choices = other.game_choices;

	//reset
	other.row = 0;
	other.col = 0;
	other.num_pieces = 0;
	other.longest = 0;
	other.winner.clear();
	other.pieces.clear();
	other.game_choices.clear();
}

gamebase & gamebase:: operator=(const gamebase & other)
{
	if (&other == this)
	{
		return *this;
	}
	row = other.row;
	col = other.col;
	num_pieces = other.num_pieces;
	longest = other.longest;
	winner = other.winner;
	pieces = other.pieces;
	game_choices = other.game_choices;
	return *this;
}

gamebase & gamebase:: operator=(gamebase && other)
{
	if (&other == this)
	{
		return *this;
	}
	row = other.row;
	col = other.col;
	num_pieces = other.num_pieces;
	longest = other.longest;
	winner = other.winner;
	pieces = other.pieces;
	game_choices = other.game_choices;

	// reset
	other.row = 0;
	other.col = 0;
	other.num_pieces = 0;
	other.longest = 0;
	other.winner.clear();
	other.pieces.clear();
	other.game_choices.clear();
	return *this;
}

gamebase::~gamebase()
{
}

void gamebase::save(string status)
{
	ofstream output_file;
	string class_excess = "class ";
	string class_name = typeid(*this).name();
	class_name = class_name.substr(class_excess.length());
	string input = class_name;

	output_file.open(input); //case insensitive
	if (output_file.is_open()) 
	{
		if (status.compare("yes") == 0) // if save
		{
			output_file << class_name << endl;
		}
		else if (status.compare("done") == 0)
		{
			output_file << "COMPLETE" << endl;
		}
		else 
		{ //if not save
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
		}
		output_file.close();
	}
	else
	{
		throw prompt_code::open_file_fail;
	}
}



int gamebase::prompt(unsigned int & x_, unsigned int & y_)
{
	bool invalid = true;
	string line;
	while (invalid) 
	{
		cout << "Please input valid coordinate or enter quit" << endl;
		getline(cin, line); //use getline to eliminate any repeats in teh cin buffer

		line.erase(remove(line.begin(), line.end(), ' '), line.end()); //remove whitespace

		for (unsigned int i = 0; i < line.length(); ++i) 
		{
			line[i] = tolower(line[i]); //to lowercase
		}

		if (line.compare("quit") == 0) //if it is quit, return quit
		{
			int quit = prompt_code::quit;
			string save_file;
			string class_name = typeid(*this).name();

			if (class_name.find("ult_tictactoe") == string::npos) { //if not ultimate
				while (save_file.compare("yes") != 0 && save_file.compare("no") != 0) //if not yes or no
				{
					cout << "Do you want to save the game? Enter yes or no." << endl;

					getline(cin, save_file);
					save_file.erase(remove(save_file.begin(), save_file.end(), ' '), save_file.end()); //remove any whitespace
					for (unsigned int i = 0; i < save_file.length(); ++i) {
						save_file[i] = tolower(save_file[i]); //to lowercase
					}
				}
			}

			save(save_file);
			throw quit;
		}
		else 
		{
			replace(line.begin(), line.end(), ',', ' '); //replace , with space
			istringstream iss(line);
			if (iss >> x_ && iss >> y_) {
				invalid = false; //done prompting
			}
		}
	}
	return prompt_code::success; //return success
}

void gamebase::prompt(unsigned int & x_, unsigned int & y_, unsigned int & val) {
	bool invalid = true;
	string line;
	while (invalid) 
	{

		cout << "Please input valid coordinate and a value, or enter quit" << endl;
		getline(cin, line); //use getline to eliminate any repeats in teh cin buffer

		line.erase(remove(line.begin(), line.end(), ' '), line.end()); //remove whitespace

		for (unsigned int i = 0; i < line.length(); ++i) 
		{
			line[i] = tolower(line[i]); //to lowercase
		}

		if (line.compare("quit") == 0) //if it is quit, return quit
		{
			string save_file;
			while (save_file.compare("yes") != 0 && save_file.compare("no") != 0) //if not yes or no
			{
				cout << "Do you want to save the game? Enter yes or no." << endl;

				getline(cin, save_file);
				save_file.erase(remove(save_file.begin(), save_file.end(), ' '), save_file.end()); //remove any whitespace
				for (unsigned int i = 0; i < save_file.length(); ++i) {
					save_file[i] = tolower(save_file[i]); //to lowercase
				}
			}
			save(save_file);


			int quit = prompt_code::quit;
			throw quit;
		}
		else {
			replace(line.begin(), line.end(), ',', ' '); //replace , with space
			istringstream iss(line);
			if (iss >> x_ && iss >> y_ && iss >> val) 
			{
				invalid = false; //done prompting
			}
		}
	}
}

int gamebase::play()
{
	int turn_var = prompt_code::success;
	bool done_var = done();
	bool draw_var = draw();

	while (turn_var == prompt_code::success && !done_var && !draw_var) //while 1. done != true 2. draw == false, and 3. user doesn't quit
	{
		try
		{
			turn_var = turn();
		}
		catch (int x)
		{
			if (x == prompt_code::quit) {
				cout << "Player has quit. Game has ended." << endl; //if player has quit
				string class_name = typeid(*this).name();
				if (class_name.find("ult_tictactoe") == string::npos) { //if not ultimate
					cout << num_pieces << " pieces were placed" << endl;
				}
				return prompt_code::quit;
			}
		}

		done_var = done();
		draw_var = draw();
	}

	save("done");

	if (done_var) //if a player won the game, print out the winner
	{
		cout << "Player " << winner << " wins the game!" << endl;
		return prompt_code::success;
	}
	cout << num_pieces << " turns were played" << endl;
	cout << "No winning moves remain--it's a draw!" << endl;
	return prompt_code::no_moves;
}

bool gamebase::isAllDigit(string content) {
	for (size_t j = 0; j < content.size(); ++j)
	{
		if (!isdigit(content[j]))
		{
			return false;
		}
	}
	return true;
}
void gamebase::program_check(int argc, char * argv[])
{
	if (argc != dim) //if more than one
	{
		int e = prompt_code::num_wrong_args;;
		throw e;
	}
	for (unsigned int i = 0; argv[game_name][i]; ++i) {
		argv[game_name][i] = tolower((unsigned char)argv[game_name][i]); //to lowercase
	}

	if (game_base != nullptr)
	{
		int e = prompt_code::null_ptr;
		throw e;
	}

	try {
		string name = "gomoku";
		if (argv[game_name] == name) // if it is gomomku
		{
			game_base.reset(new gomoku(19, 19));
			return;
		}

		name = "tictactoe";
		if (argv[game_name] == name)
		{
			game_base.reset(new tictactoe(5, 5));
			return;
		}

		name = "ultimatetictactoe";
		if (argv[game_name] == name)
		{
			game_base.reset(new ult_tictactoe(10, 10));
			return;
		}

		name = "sudoku";
		if (argv[game_name] == name)
		{
			game_base.reset(new sudokugame());
			return;
		}
	}
	catch (std::bad_alloc& ba)
	{
		std::cerr << "bad_alloc caught: " << ba.what() << '\n';
	}

	int e = prompt_code::wrong_arg_name;
	throw e;
}

shared_ptr<gamebase> gamebase::instance() {
	if (game_base != nullptr)
	{
		return game_base;
	}
	int e = prompt_code::null_ptr;
	throw e;
}