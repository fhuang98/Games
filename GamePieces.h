//GamePieces.h: Contains the declarations for the game_piece constructor
//Authors: F Huang 
#pragma once
#include <string>
using namespace std;

struct game_piece {
	game_piece::game_piece(string display); // can display string
	game_piece::game_piece(string display, bool mutation);
	string piece_display;
	bool mutate;
};