#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Board.hpp"
using namespace std;

Board::Board()
{
    numRows = 3;
    numCols = 3;
    max = 0;
    target = 32;
    allocateMemory();
}


Board::Board(int m)
{
    Board();
    if (m >= 1)
    {
        numRows = m;
        numCols = m;
    }
    allocateMemory();
}

Board::Board(int m, int n)
{
    Board();
    if (m >= 1 && n >=1)
    {
        numRows = m;
        numCols = n;
    }
    allocateMemory();
}

Board::~Board()
{
    int row = 0;
    for(row=0; row<numRows; ++row)
    {
        free(panel[row]);
        panel[row] = NULL;
    }
    free(panel);
    panel = NULL;
}

void Board::setGoal(int goal)
{
    target = goal;
}

void Board::clear()
{
    // memset(panel, 0, sizeof(int)*numRows*numCols);
    int row, col;
    for(row=0; row<numRows; ++row)
    {
        for(col=0; col<numCols; ++col)
        {
            panel[row][col] = 0;
        }
    }
}

void Board::allocateMemory()
{
    int row;
    panel = (int **)calloc(numRows, sizeof(int *));
    if(!panel)
    {
        cout << "calloc for panel error" << endl;
        return;
    }
    for(row=0; row<numRows; ++row)
    {
        panel[row] = (int *)calloc(numCols, sizeof(int));
        if(!panel[row])
        {
            cout << "calloc for sub panel error" << endl;
            return;
        }
    }
}

void Board::print() const
{
    int row, col;
    string updown = "----";
    string leftright = "|";
    string corner = "+";
    string black = "    ";

    cout << corner;
    for(col=0; col<numCols; ++col)
    {
        cout << updown << corner;
    }
    cout << endl;

    for (row=0; row<numRows; ++row)
    {
        cout << leftright;
        for(col=0; col<numCols; ++col)
        {
            cout << black << leftright;
        }
        cout << endl;

        cout << corner;
        for(col=0; col<numCols; ++col)
        {
            cout << updown << corner;
        }
        cout << endl;
    }
}

int main(int argc, char *argv[])
{
    int m = 3, n = 3; 
    if(argc >= 2)
    {
        m = atoi(argv[1]);
        n = m;
    }
    if(argc >= 3)
    {
        n = atoi(argv[2]);
    }
    Board game = Board(m, n);
    game.print();
    return 0;
}