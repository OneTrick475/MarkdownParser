#include "MdParserCli.h"
#include <iostream>

Row MdParserCli::getRow() const{
    printf("Enter %d values(each line is considered a value): ", mdp.getNumOfCols());

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // this is to allow spaces in the values
    Row row;
    for (int i = 0; i < mdp.getNumOfCols(); i++) {
        std::cin.getline(row.colls[row.numOfColls++], maxCharsInField + 1);
        if (std::cin.fail()) {
            cleanInputBuffer();
        }
    }

    return row;
}

void MdParserCli::printMenu() const {
    std::cout << "Chose an option by typing the corresponding number:\n1. Load new table\n2. Print table\n3. Save table\n4. Change column name\n"
        << "5. Add row\n6.Change Value\n7.Select rows by column and value\n8.exit\n";
}

void MdParserCli::getFile(char* file) const {
    std::cout << "Write the name of the file (no more than 100 characters): ";

    std::cin >> file;
}

int MdParserCli::getOption(int min, int max) const {
    int option;

    std::cin >> option;
    while (std::cin.fail() || option < min || option > max) {
        cleanInputBuffer();
        printf("Invalid. Enter a number between %d and %d\n", min, max);
        std::cin >> option;
    }
    return option;
}

void MdParserCli::cleanInputBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void MdParserCli::changeValue() {
    std::cout << "1. Change by row number and column name\n2. Change by column name and value\n";
    int newOption = getOption(1, 2);

    if (newOption == 1) {
        std::cout << "Write the row number: ";
        int rowNum = getOption(1, mdp.getNumOfRows() - 2);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char colName[maxCharsInField + 1];
        printf("Write the name of the column (no more than %d characters): ", maxCharsInField);
        std::cin.getline(colName, maxCharsInField);

        if (std::cin.fail()) {
            cleanInputBuffer();
        }

        char value[maxCharsInField + 1];
        printf("Write the new value (no more than %d characters): ", maxCharsInField);
        std::cin.getline(value, maxCharsInField + 1);

        if (std::cin.fail()) {
            cleanInputBuffer();
        }

        mdp.changeValue(rowNum, colName, value);
    }
    else {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        char colName[maxCharsInField + 1];
        printf("Write the name of the column (no more than %d characters): ", maxCharsInField);
        std::cin.getline(colName, maxCharsInField + 1);

        if (std::cin.fail()) {
            cleanInputBuffer();
        }

        char value[maxCharsInField + 1];
        printf("Write the value you want to change (no more than %d characters): ", maxCharsInField);
        std::cin.getline(value, maxCharsInField + 1);

        if (std::cin.fail()) {
            cleanInputBuffer();
        }

        char newValue[maxCharsInField + 1];
        printf("Write the new value (no more than %d characters): ", maxCharsInField);
        std::cin.getline(newValue, maxCharsInField + 1);

        if (std::cin.fail()) {
            cleanInputBuffer();
        }

        mdp.changeValue(colName, value, newValue);
    }
}

void MdParserCli::changeColName() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    char colName[maxCharsInField + 1];
    printf("Write the name of the column you want to change (no more than %d characters): ", maxCharsInField);
    std::cin.getline(colName, maxCharsInField + 1);

    if (std::cin.fail()) {
        cleanInputBuffer();
    }

    char colNewName[maxCharsInField + 1];
    printf("Write the new name of the column (no more than %d characters): ", maxCharsInField);
    std::cin.getline(colNewName, maxCharsInField + 1);

    if (std::cin.fail()) {
        cleanInputBuffer();
    }

    mdp.changeCollName(colName, colNewName);
}

void MdParserCli::printRowsWhere() const {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    char columnName[maxCharsInField + 1];
    printf("Write the name of the column (no more than %d characters): ", maxCharsInField);
    std::cin.getline(columnName, maxCharsInField + 1);

    if (std::cin.fail()) {
        cleanInputBuffer();
    }

    char value[maxCharsInField + 1];
    printf("Write the value (no more than %d characters): ", maxCharsInField);
    std::cin.getline(value, maxCharsInField + 1);

    if (std::cin.fail()) {
        cleanInputBuffer();
    }

    mdp.printRowsWhere(columnName, value);
}

void MdParserCli::run() {
    char file[100];

    getFile(file);

    mdp.loadTable(file);

    while (true) {
        printMenu();

        int option = getOption(1, 9);

        switch (option) {
        case 1:
            getFile(file);
            mdp.loadTable(file);
            break;
        case 2:
            mdp.printTable();
            break;
        case 3:
            getFile(file);
            mdp.saveTable(file);
            break;
        case 4:
            changeColName();
			break;
        case 5:
            mdp.addRow(getRow());
            break;
        
        case 6:
            changeValue();
            break;

        case 7:
            printRowsWhere();
            break;

        case 8:
            return;

        default:
            std::cout << "idk";
        }
        //this is the most damn boring part to write
    }
}
