#include <iostream>
#include "MarkdownParser.h"
#include "Row.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#pragma warning(disable : 4996)

void MarkdownParser::loadTable(const char* fileName) {
    if (fileName == nullptr) {
        std::cout << "invalid input";
        return;
    }

    numOfRows = 0;

    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cout << "Error!" << std::endl;
        return;
    }

    while (!file.eof()) {
        rows[numOfRows++] = getRow(file);
    }

    for(int i = 0; i < rows[0].numOfColls; i++) {
        if (rows[1].colls[i][0] == ':' && rows[1].colls[i][strlen(rows[1].colls[i]) - 1] == ':')
            alignments[i] = 2;
        else if (rows[1].colls[i][strlen(rows[1].colls[i]) - 1] == ':')
            alignments[i] = 1;
        //left align (0) is the default state so its unnecessary to check for it
    }

    file.close();
}

Row MarkdownParser::getRow(std::ifstream& file) const {
    Row row;

    //this formula works if we assume that the number of whitespaces per value wont be more than 20
    const int buffSize = maxColls * maxCharsInField * 2 + maxColls + 2;
    char buff[buffSize];

    file.getline(buff, buffSize);

    //used string stream to split into words and ignore white spaces
    std::stringstream ss(buff);

    char value[21] = {'\0'};

    ss.get(); //remove the first '|'

    while(ss >> value) {
        if (strcmp("|", value) != 0) {
            if(strlen(row.colls[row.numOfColls]) > 0) {
                char newValue[21] = " ";
                strcat(newValue, value);
                strcat(row.colls[row.numOfColls], newValue);
            }
            else
				strcat(row.colls[row.numOfColls], value);
        }
        else
            row.numOfColls++;
    }

    return row;
}

//func used to get len of longest value in table, the width of each column when printing is based on this
//possible optimization is to store the value in a field and update it every time the table is updated
//tho normally the table would be printed only once between table updates so it seemed unnecessary
int MarkdownParser::getMaxCollLength() const {
    int max = 0;

    for (int i = 0; i < numOfRows; i++)
        if(i != 1)
			for (int j = 0; j < rows[i].numOfColls; j++)
				max = strlen(rows[i].colls[j]) > max ? strlen(rows[i].colls[j]) : max;
    
    return max;
}

void MarkdownParser::saveRow(int index, int collLen, std::ofstream& file) const {
    file << "| ";

    if (index == 1) {
        for (int j = 0; j < rows[index].numOfColls; j++) {
            char align = alignments[j] == 0 || alignments[j] == 2 ? ':' : '-';

            file << std::setfill('-') << align;

            align = alignments[j] == 1 || alignments[j] == 2 ? ':' : '-';

            file << std::setw(collLen - 1) << align << " | ";
        }
        file << std::setfill(' ');
        return;
    }
    for (int j = 0; j < rows[index].numOfColls; j++) {
        int leftW = 0, rightW = collLen - strlen(rows[index].colls[j]) + 3;

        if (alignments[j] == 2) {
            rightW = (collLen - strlen(rows[index].colls[j])) / 2 + 3;
            leftW = rightW + strlen(rows[index].colls[j]) - 3;
            rightW = strlen(rows[index].colls[j]) % 2 == 0 ? rightW : rightW + 1;
        }
        else if (alignments[j] == 1) {
            leftW = collLen;
            rightW = 0;
        }
        file << std::setw(leftW) << rows[index].colls[j] << std::setw(rightW) << " | ";
    }
}

void MarkdownParser::printRow(int index, int collLen) const {
    std::cout << "| ";
    if (index == 1) {
        for (int j = 0; j < rows[index].numOfColls; j++)
            std::cout << std::setfill('-') << std::setw(collLen + 3) << " | ";

        std::cout << std::endl << std::setfill(' ');
        return;
    }
    for (int j = 0; j < rows[index].numOfColls; j++) {
        int leftW = 0, rightW = collLen - strlen(rows[index].colls[j]) + 3;

        if (alignments[j] == 2) {
            rightW = (collLen - strlen(rows[index].colls[j])) / 2 + 3;
            leftW = rightW + strlen(rows[index].colls[j]) - 3;
            rightW = strlen(rows[index].colls[j]) % 2 == 0 ? rightW : rightW + 1;
        }
        else if (alignments[j] == 1) {
            leftW = collLen;
            rightW = 0;
        }

        std::cout << std::setw(leftW) << rows[index].colls[j] << std::setw(rightW) << " | ";
    }
    std::cout << std::endl;
}

//the width of each cell is based on the longest value in the table
void MarkdownParser::printTable() const {
    int collLen = getMaxCollLength();

    //to improve centering, the len of each column will be an even number
    collLen = collLen % 2 == 0 ? collLen : collLen + 1;

	for(int i = 0; i < numOfRows; i++)
        printRow(i, collLen);
	
    std::cout << std::endl;
}

void MarkdownParser::changeCollName(const char* collName, const char* newCollName) {
    if(collName == nullptr || newCollName == nullptr || strlen(collName) > maxCharsInField || strlen(newCollName) > maxCharsInField) {
        std::cout << "invalid input";
        return;
    }

    for (int i = 0; i < rows[0].numOfColls; i++)
        if (strcmp(collName, rows[0].colls[i]) == 0) {
            strcpy(rows[0].colls[i], newCollName);
            return;
        }
    std::cout << "Column does not exist\n";
}

void MarkdownParser::addRow(char row[][maxCharsInField + 1]) {
    if (row == nullptr) {
        std::cout << "invalid input";
        return;
    }

    Row newRow;

    for(int i = 0; i < rows[0].numOfColls; i++) 
        strcpy(newRow.colls[i], row[i]);

    newRow.numOfColls = rows[0].numOfColls;
    rows[numOfRows++] = newRow;
}

void MarkdownParser::addRow(Row row) {
    if(row.numOfColls != rows[0].numOfColls) {
        std::cout << "Invalid row";
        return;
    }

    rows[numOfRows++] = row;
}


int MarkdownParser::getCollIndex(const char* collName) const {
    int collIndex = -1;

    for (int i = 0; i < rows[0].numOfColls; i++) {
        if (strcmp(rows[0].colls[i], collName) == 0) {
            collIndex = i;
            break;
        }
    }
    return collIndex;
}

int MarkdownParser::getRowIndexByCollAndValue(int collIndex, const char* value) const{
    //havent done input checks here and in some other private functions because they are only used
    //withing the class and the inputs are safe

    int rowIndex = -1;

    for (int i = 2; i < numOfRows; i++) {
        if (strcmp(rows[i].colls[collIndex], value) == 0) {
            rowIndex = i;
            break;
        }
    }
    return rowIndex;
}

void MarkdownParser::changeValue(int rowNum, const char* collName, const char* newValue) {
    if (collName == nullptr || newValue == nullptr || strlen(collName) > maxCharsInField || strlen(newValue) > maxCharsInField
        || rowNum < 1 || rowNum > numOfRows) {
        std::cout << "invalid input";
        return;
    }

    int collIndex = getCollIndex(collName);

    if(collIndex == -1) {
        std::cout << "Invalid column name" << std::endl;
        return;
    }

    strcpy(rows[rowNum + 1].colls[collIndex], newValue);
}

void MarkdownParser::changeValue(const char* collName, const char* valueToChange, const char* newValue) {
    if (collName == nullptr || newValue == nullptr || strlen(collName) > maxCharsInField || strlen(newValue) > maxCharsInField
        || valueToChange == nullptr || strlen(valueToChange) > maxCharsInField) {
        std::cout << "invalid input";
        return;
    }

    int collIndex = getCollIndex(collName);

    if (collIndex == -1) {
        std::cout << "Invalid column name" << std::endl;
        return;
    }

    int rowIndex = getRowIndexByCollAndValue(collIndex, valueToChange);

    if (rowIndex == -1) {
        std::cout << "Value does not exist" << std::endl;
        return;
    }

    strcpy(rows[rowIndex].colls[collIndex], newValue);
}

void MarkdownParser::printRowsWhere(const char* collName, const char* value) const{
    if (collName == nullptr || value == nullptr || strlen(collName) > maxCharsInField || strlen(value) > maxCharsInField) {
        std::cout << "invalid input";
        return;
    }

    int collLen = getMaxCollLength();

    collLen = collLen % 2 == 0 ? collLen : collLen + 1;

    int collIndex = getCollIndex(collName);

    if (collIndex == -1) {
        std::cout << "Invalid column name" << std::endl;
        return;
    }

    for (int i = 0; i < numOfRows; i++) {
        if (strcmp(rows[i].colls[collIndex], value) != 0 && i != 0 && i!=1)
            continue;

        printRow(i, collLen);
    }
    std::cout << std::endl;
}

void MarkdownParser::saveTable(const char* fileName) const {
    if (fileName == nullptr) {
        std::cout << "invalid input";
        return;
    }

    std::ofstream file(fileName, std::ios::trunc);  

    if (!file.is_open()) {
        std::cout << "Error!" << std::endl;
        return;
    }

    int collLen = getMaxCollLength();

    collLen = collLen % 2 == 0 ? collLen : collLen + 1;

    for (int i = 0; i < numOfRows; i++) {
        saveRow(i, collLen, file);

        if (i != numOfRows - 1)
			 file << '\n';
    }

    file.close();
}

int MarkdownParser::getNumOfCols() const {
    return rows[0].numOfColls;
}

int MarkdownParser::getNumOfRows() const {
    return numOfRows;
}