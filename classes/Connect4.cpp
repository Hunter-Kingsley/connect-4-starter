#include "Connect4.h"

Connect4::Connect4() 
{
    _grid = new Grid(7, 6);
}

Connect4::~Connect4()
{
    delete _grid;
}

void Connect4::setUpBoard()
{
    setNumberOfPlayers(2);
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;
    _grid->initializeSquares(80, "square.png");

    Player* blackPlayer = getPlayerAt(RED_PLAYER);
    Player* whitePlayer = getPlayerAt(YELLOW_PLAYER);

    if (gameHasAI()) {
        setAIPlayer(AI_PLAYER);
    }

    startGame();
}

// Scan the board with a 4x4 window to check all possible combinations
Player* Connect4::checkForWinner()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            Player* winner = CheckWindow(_grid->getSquare(i, j));
            if (winner) return winner;
        }
    }

    return nullptr;
}

// Use a 4x4 window to scan the board for possible winning combonations
Player* Connect4::CheckWindow(ChessSquare* square)
{
    int x = square->getColumn();
    int y = square->getRow();
    Player* W = nullptr;

    // top left checks
    W = CheckRow(square);
    if (W) {return W;}
    W = CheckCol(square);
    if (W) {return W;}
    W = CheckDiag(square, 1);
    if (W) {return W;}

    // bottom left checks
    W = CheckRow(_grid->getSquare(x, y+3));
    if (W) {return W;}
    W = CheckDiag(_grid->getSquare(x, y+3), -1);
    if (W) {return W;}

    // top right check
    W = CheckCol(_grid->getSquare(x+3, y));
    if (W) {return W;}

    return W;
}

Player* Connect4::CheckRow(ChessSquare* square)
{
    if (square->bit() == nullptr) {return nullptr;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; i < 4; i++)
    {
        if (_grid->getSquare(x+i, y) == nullptr || _grid->getSquare(x+i, y)->bit() == nullptr)
        {
            return nullptr;
        }

        if (_grid->getSquare(x+i, y)->bit()->getOwner() != squareOwner)
        {
            return nullptr;
        }
    }

    return squareOwner;
}
Player* Connect4::CheckCol(ChessSquare* square)
{
    if (square->bit() == nullptr) {return nullptr;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; i < 4; i++)
    {
        if (_grid->getSquare(x, y+i) == nullptr || _grid->getSquare(x, y+i)->bit() == nullptr)
        {
            return nullptr;
        }

        if (_grid->getSquare(x, y+i)->bit()->getOwner() != squareOwner)
        {
            return nullptr;
        }
    }

    return squareOwner;
}
Player* Connect4::CheckDiag(ChessSquare* square, int direction)
{
    // upwards should be 1 or -1 for the function to work

    if (square->bit() == nullptr) {return nullptr;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; std::abs(i) < 4; i++)
    {
        if (_grid->getSquare(x+i, y+(i*direction)) == nullptr || _grid->getSquare(x+i, y+(i*direction))->bit() == nullptr)
        {
            return nullptr;
        }

        if (_grid->getSquare(x+i, y+(i*direction))->bit()->getOwner() != squareOwner)
        {
            return nullptr;
        }
    }

    return squareOwner;
}

bool Connect4::checkForDraw()
{
    for (int i = 0; i < 7; i++) 
    {
        if (!_grid->getSquare(i, 0)->bit()) 
        {
            return false;
        }
    }

    return true;
}

std::string Connect4::initialStateString()
{
    return std::string (42, '0');
}

std::string Connect4::stateString()
{
    std::string s(42, '0');
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        Bit *bit = square->bit();
        if (!bit) {
            s[y * 7 + x] = '0';
        } else if (bit->getOwner() == getPlayerAt(RED_PLAYER)) {
            s[y * 7 + x] = '1';
        } else {
            s[y * 7 + x] = '2';
        }
    });
    return s;
}

void Connect4::setStateString(const std::string &s)
{

}

bool Connect4::actionForEmptyHolder(BitHolder &holder)
{
    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int x = square->getColumn();
    int y = square->getRow();
    Player* currentPlayer = getCurrentPlayer();
    
    if (_grid->getSquare(x, 0)->bit()) return false;

    //std::cout << x << " " << y << std::endl;

    Bit* newPiece = createPiece(currentPlayer);
    ChessSquare* newSquare = findLowestPossibleSquare(square);
    newSquare->setBit(newPiece);
    newPiece->setPosition(_grid->getSquare(x, 0)->getPosition());
    newPiece->moveTo(newSquare->getPosition());

    endTurn();
    return true;
}

bool Connect4::canBitMoveFrom(Bit &bit, BitHolder &src)
{
    return false;
}

bool Connect4::canBitMoveFromTo(Bit &bit, BitHolder &src, BitHolder &dst)
{
    return false;
}

void Connect4::stopGame()
{
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
}

void Connect4::updateAI() 
{

}

Bit * Connect4::createPiece(Player* player)
{
    Bit* bit = new Bit();
    bit->LoadTextureFromFile(player == getPlayerAt(RED_PLAYER) ? "red.png" : "yellow.png");
    bit->setOwner(player);
    return bit;
}

ChessSquare* Connect4::findLowestPossibleSquare(ChessSquare* square) 
{
    for (int i = _grid->getHeight() -1; i >= 0; i--) 
    {
        if (!_grid->getSquare(square->getColumn(), i)->bit())
        {
            return _grid->getSquare(square->getColumn(), i);
        }
    }
    return nullptr;
}

bool isAIBoardFull(const std::string& state) 
{
    return state.find('0') == std::string::npos;
}

int evaluateAIBoard(const std::string& state)
{

}

int Connect4::negamax(std::string& state, int depth, int playerColor)
{
    int score = evaluateAIBoard(state);

    // Check if AI wins, human wins, or draw
    if(depth == 4) { 
        // A winning state is a loss for the player whose turn it is.
        // The previous player made the winning move.
        return -score; 
    }

    if(isAIBoardFull(state)) {
        return 0; // Draw
    }

    int bestVal = -1000; // Min value
    // for (int y = 0; y < 6; y++) {
    //     for (int x = 0; x < 7; x++) {
    //         // Check if cell is empty
    //         if (state[y * 7 + x] == '0') {
    //             // Make the move
    //             state[y * 7 + x] = playerColor == HUMAN_PLAYER ? '1' : '2'; // Set the cell to the current player's color
    //             bestVal = std::max(bestVal, -negamax(state, depth + 1, -playerColor));
    //             // Undo the move for backtracking
    //             state[y * 7 + x] = '0';
    //         }
    //     }
    // }

    for (int i = 0; i < 7; i++)
    {
        ChessSquare* topSquare = findLowestPossibleSquare(_grid->getSquare(i, 0));
        if (topSquare == nullptr) {continue;}

        state[topSquare->getRow() * 7 + topSquare->getColumn()] = playerColor == RED_PLAYER ? '0' : '1';
        bestVal = std::max(bestVal, -negamax(state, depth + 1, 1- playerColor));
        state[topSquare->getRow() * 7 + topSquare->getColumn()] = '0';
    }

    return bestVal;
}