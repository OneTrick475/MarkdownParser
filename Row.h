#pragma once

const int maxColls = 10;
const int maxCharsInField = 20;

struct Row {
    char colls[maxColls][maxCharsInField+1]{'\0'};
    int numOfColls = 0;
};