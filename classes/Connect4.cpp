#include "Connect4.h"
#include <limits>
#include <vector>

Connect4::Connect4(bool A) 
{
    _grid = new Grid(7, 6);

    isPlayerFirst = A;
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
        if (!isPlayerFirst) 
        {
            setAIPlayer(RED_PLAYER);
        } else 
        {
            setAIPlayer(YELLOW_PLAYER);
        }
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

// Window is 4x4
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
    std::string s = stateString();

    int curColor = (getCurrentPlayer() == getPlayerAt(RED_PLAYER)) ? RED_PLAYER : YELLOW_PLAYER;

    auto findLowestRowInState = [&](const std::string& st, int col) -> int {
        for (int r = 5; r >= 0; --r) {
            if (st[r * 7 + col] == '0') return r;
        }
        return -1;
    };

    int bestVal = std::numeric_limits<int>::min();
    int bestCol = -1;

    for (int col = 0; col < 7; ++col) {
        int row = findLowestRowInState(s, col);
        if (row == -1) continue;
        // Make the move for the correct player
        s[row * 7 + col] = (curColor == RED_PLAYER) ? '1' : '2';
        int val = -negamax(s, 1, 1 - curColor);
        // undo
        s[row * 7 + col] = '0';

        if (val > bestVal) {
            bestVal = val;
            bestCol = col;
        }
    }

    if (bestCol >= 0) {
        // play the move for real
        ChessSquare* target = findLowestPossibleSquare(_grid->getSquare(bestCol, 0));
        if (target) {
            Bit* newPiece = createPiece(getCurrentPlayer());
            target->setBit(newPiece);
            newPiece->setPosition(_grid->getSquare(bestCol, 0)->getPosition());
            newPiece->moveTo(target->getPosition());
            endTurn();
        }
    }
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

bool C4isAIBoardFull(const std::string& state) 
{
    return state.find('0') == std::string::npos;
}

int findLowestRowInState(const std::string& s, int col) {
    for (int r = 5; r >= 0; --r) {
        if (s[r * 7 + col] == '0') return r;
    }
    return -1;
};


int Connect4::C4evaluateAIBoard(const std::string& state)
{
    const int height = 6;

    auto scoreWindow = [&](const std::vector<char>& window) {
        int redCount = 0;
        int yellowCount = 0;
        int emptyCount = 0;
        for (char c : window) {
            if (c == '1') ++redCount;
            else if (c == '2') ++yellowCount;
            else ++emptyCount;
        }

        if (redCount == 4) return 100000;
        if (yellowCount == 4) return -100000;

        int score = 0;
        if (redCount == 3 && emptyCount == 1) score += 100;
        if (redCount == 2 && emptyCount == 2) score += 10;

        if (yellowCount == 3 && emptyCount == 1) score -= 100;
        if (yellowCount == 2 && emptyCount == 2) score -= 10;

        return score;
    };

    int total = 0;

    // Horizontal windows
    for (int y = 0; y < _gameOptions.rowY; ++y) {
        for (int x = 0; x <= _gameOptions.rowX - 4; ++x) {
            std::vector<char> window;
            for (int i = 0; i < 4; ++i) window.push_back(state[y * _gameOptions.rowX + (x + i)]);
            total += scoreWindow(window);
        }
    }

    // Vertical windows
    for (int x = 0; x < _gameOptions.rowX; ++x) {
        for (int y = 0; y <= _gameOptions.rowY - 4; ++y) {
            std::vector<char> window;
            for (int i = 0; i < 4; ++i) window.push_back(state[(y + i) * _gameOptions.rowX + x]);
            total += scoreWindow(window);
        }
    }

    // Up Left Diagonals
    for (int x = 0; x <= _gameOptions.rowX - 4; ++x) {
        for (int y = 3; y < _gameOptions.rowY; ++y) {
            std::vector<char> window;
            for (int i = 0; i < 4; ++i) window.push_back(state[(y - i) * _gameOptions.rowX + (x + i)]);
            total += scoreWindow(window);
        }
    }

    // Down Right Diagonals
    for (int x = 0; x <= _gameOptions.rowX - 4; ++x) {
        for (int y = 0; y <= _gameOptions.rowY - 4; ++y) {
            std::vector<char> window;
            for (int i = 0; i < 4; ++i) window.push_back(state[(y + i) * _gameOptions.rowX + (x + i)]);
            total += scoreWindow(window);
        }
    }

    return total;
}

int Connect4::negamax(std::string& state, int depth, int playerColor)
{
    const int startAlpha = -9999999999999999;
    const int startBeta = 9999999999999999;

    return negamaxAB(state, depth, playerColor, startAlpha, startBeta);
}

int Connect4::negamaxAB(std::string& nodeState, int ply, int plyPlayerColor, int alpha, int beta)
{
    const int MAX_DEPTH = 5;

    int eval = C4evaluateAIBoard(nodeState);
    if (ply >= MAX_DEPTH || C4isAIBoardFull(nodeState)) {
        return (plyPlayerColor == RED_PLAYER) ? eval : -eval;
    }

    int best = std::numeric_limits<int>::min() + 1;

    // For each column
    for (int col = 0; col < 7; ++col) {
        int row = findLowestRowInState(nodeState, col);
        if (row == -1) continue;

        // make move
        nodeState[row * 7 + col] = (plyPlayerColor == RED_PLAYER) ? '1' : '2';

        int val = -negamaxAB(nodeState, ply + 1, 1 - plyPlayerColor, -beta, -alpha);

        // undo
        nodeState[row * 7 + col] = '0';

        if (val > best) best = val;
        if (best > alpha) alpha = best;
        if (alpha >= beta) break;
    }

    // Draw Check
    if (best == std::numeric_limits<int>::min() + 1) return 0;
    return best;
}