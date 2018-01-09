//GamePieces.cpp: Contains definitions for the game piece constructor
//Authors: F Huang (fehuang@wustl.edu)

#include "stdafx.h"
#include "GamePieces.h"
#include "Helper.h"
#include <string>
using namespace std;

game_piece::game_piece(string display)
	:piece_display(display) {}

game_piece::game_piece(string display, bool mutation)
	: piece_display(display), mutate(mutation) {}
