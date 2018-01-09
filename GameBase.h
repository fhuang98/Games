// Gamebase.h : Declare the methods for the base game board
// Authors: F Huang 
// Contains the constructors and the definitions for the methods done, drawn, prompt, turn, and play
#pragma once
#include "GamePieces.h"
#include <string>
#include <vector>
#include <memory>

using namespace std;

enum prompt_code { success, quit, no_moves, num_wrong_args, null_ptr, wrong_num_lines, wrong_length_input, invalid_input, open_file_fail, new_game, wrong_arg_name };
enum arr_position { program_name, game_name, dim };

class gamebase
{
public:
	static void program_check(int argv, char * argc[]);
	static shared_ptr<gamebase>  instance();
	int play();
	string winner;
	virtual void prompt(unsigned int & x_, unsigned int & y_, unsigned int & val);
protected:
	gamebase();
	gamebase(int r_, int c_);
	gamebase(const gamebase & other);
	gamebase(gamebase && other);
	gamebase & operator=(const gamebase & other);
	gamebase & operator=(gamebase && other);
	virtual ~gamebase();
	unsigned int row;
	unsigned int col;
	static bool isAllDigit(string check);
	int num_pieces;
	int longest;
	vector<game_piece> pieces;
	virtual int prompt(unsigned int & x_, unsigned int & y_);
	virtual void print() = 0;
	virtual bool done() = 0;
	virtual bool draw() = 0;
	virtual int turn() = 0;
	virtual void save(string status);
	static shared_ptr<gamebase> game_base;
private:
	vector<string> game_choices;

};