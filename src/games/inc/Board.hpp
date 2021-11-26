#ifndef BOARD_H
#define BOARD_H


class Board
{
private:
    // two dimensional array with numRows rows and numCols columns
    int **panel;    
    int numRows;
    int numCols;
    int target; // the goal
    int max;    // the current max in all cell of pannel
public:
    Board();    // construct a 3x3 pannel
    Board(int m);   // construct a m x m pannel
    Board(int m, int n);
    ~Board();   // destructor
    
    void setGoal(int goal);     // set goal of the game
    void allocateMemory();
    void clear();           // set each cell of the panel to be zero
    void print() const;

    void selectRandomCell(int& row, int& col);
    void pressUp();
    void pressDown();
    void pressLeft();
    void pressRight();
    void start();
    bool noAdjacentSameValue() const;
        // if there is mo two adjacent cells
        // share same value, return true,
        // otherwise, return false.
};

#endif