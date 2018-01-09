//Source.cpp: Contains the definitions to usage messages and converting string to lowercase
//Authors: F Huang 

#include "stdafx.h"
#include "Helper.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int usage_message(char * program_name_input, char * usage_info)
{
	int wrong_num_args = 3; //NW: This should technical be an enum value.
	cout << "usage: " << program_name_input << " " << usage_info; //prints out file name and usage info
	return wrong_num_args; // used same return value as in Lab 0 because wrong args is still the error but just added new info
}

string to_lowercase(string content)
{
	for (size_t i = 0; i < content.size();++i)
	{
		content[i] = tolower(content[i]);
	}
	return content;
}