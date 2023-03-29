#pragma once
#include "Row.h"
#include <fstream>

const int maxRows = 50;

class MarkdownParser {
private:
    Row rows[maxRows];
    int alignments[maxColls] = { 0 };
    int numOfRows = 0;

    Row getRow(std::ifstream& stream) const;
    int getMaxCollLength() const;
    int getCollIndex(const char* collName) const;
    int getRowIndexByCollAndValue(int collIndex, const char* value) const;
    void printRow(int index, int collLen) const;
    void saveRow(int index, int collLen, std::ofstream& file) const;

public:
    void loadTable(const char* fileName);

    void printTable() const;

    void changeCollName(const char* collName, const char* newCollName);

    void addRow(char row[][maxCharsInField + 1]);

    void addRow(Row row);

    void changeValue(int rowNum, const char* collName, const char* newValue);

    void changeValue(const char* collName, const char* valueToChange, const char* newValue);

    void printRowsWhere(const char* collName, const char* value) const;

    void saveTable(const char* fileName) const;

    //the below 2 methods are needed for the cli. better option would be to make it a friend class but i dont know if its allowed
    int getNumOfCols() const;

    int getNumOfRows() const;

};