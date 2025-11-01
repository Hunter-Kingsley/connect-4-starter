#pragma once
#include "Game.h"

class Connect4 : public Game 
{
public:
    Connect4();
    ~Connect4();

    // set up the board
    void        setUpBoard() override;

    Player*     checkForWinner() override;
    bool        checkForDraw() override;
    std::string initialStateString() override;
    std::string stateString() override;
    void        setStateString(const std::string &s) override;
    bool        actionForEmptyHolder(BitHolder &holder) override;
    bool        canBitMoveFrom(Bit &bit, BitHolder &src) override;
    bool        canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst) override;
    void        stopGame() override;

	void        updateAI() override;
    bool        gameHasAI() override { return false; }
    Grid*       getGrid() override { return _grid; }
private:
    // Player constants
    static const int RED_PLAYER = 0;
    static const int YELLOW_PLAYER = 1;
    Bit *       createPiece(Player* player);
    ChessSquare* findLowestPossibleSquare(ChessSquare* square);
    Player*     CheckWindow(ChessSquare* square);
    Player*        CheckRow(ChessSquare* square);
    Player*        CheckCol(ChessSquare* square);
    Player*        CheckDiag(ChessSquare* square, int direction);
    int         negamax(std::string& state, int depth, int playerColor);

    Grid*       _grid;
};
