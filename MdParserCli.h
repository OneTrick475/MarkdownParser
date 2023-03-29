#pragma once
#include "MarkdownParser.h"

class MdParserCli {
private:
	MarkdownParser mdp;

	void printMenu() const;
	void getFile(char* file) const;
	int getOption(int min, int max) const;
	Row getRow() const;
	void changeValue();
	void changeColName();
	void printRowsWhere() const;
	void cleanInputBuffer() const;

public:
	void run();

};