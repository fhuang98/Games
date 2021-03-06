﻿// GamesMain.cpp : Defines the entry point for the console application.
// Authors: F Huang 


#include "stdafx.h"
#include "Helper.h"
#include "GameBase.h"
#include <iostream>
#include <memory>
using namespace std;

int main(int argc, char* argv[])
{
	try {
		gamebase::program_check(argc, argv);
	}
	catch (int x) {
		if (x == prompt_code::num_wrong_args || x == prompt_code::wrong_arg_name) {
			usage_message(argv[program_name], "Please input game name (tictactoe or gomoku or sudoku or ultimatetictactoe)");
		}
		else if (x == prompt_code::null_ptr) {
			cout << "Pointed to null_ptr";
		}
		return x;
	}

	try {
		shared_ptr<gamebase> game = gamebase::instance();
		try {
			return (*game).play();
		}
		catch (int x) {
			if (x == prompt_code::open_file_fail)
			{
				cout << "Failed to open file to write" << endl;
			}
		}
	}
	catch (int x) {
		if (x == prompt_code::null_ptr) {
			cout << "Pointed to null_ptr";
		}
	}

}
