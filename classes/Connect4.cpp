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

Player* Connect4::checkForWinner()
{
    Player* winningPlayer = nullptr;

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            winningPlayer = CheckWindow(_grid->getSquare(i, j));
        }
    }

    return winningPlayer;
}

Player* Connect4::CheckWindow(ChessSquare* square)
{

    int x = square->getColumn();
    int y = square->getRow();

    // top left checks
    if (CheckRow(square)) {return CheckRow(square);}
    if (CheckCol(square)) {return CheckCol(square);}
    if (CheckDiag(square, -1)) {return CheckDiag(square, -1);}

    // bottom left checks
    if (CheckRow(_grid->getSquare(x, y+3))) {return CheckRow(_grid->getSquare(x, y+3));}
    if (CheckDiag(_grid->getSquare(x, y+3), 1)) {return CheckDiag(_grid->getSquare(x, y+3), 1);}

    // top right check
    if (CheckCol(_grid->getSquare(x+3, y))) {return CheckCol(_grid->getSquare(x+3, y));}



    return nullptr;
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
        if (_grid->getSquare(x, y+i) == nullptr || _grid->getSquare(x+i, y)->bit() == nullptr)
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

    for (int i = 0; std::abs(i) < 4; i += direction)
    {
        if (_grid->getSquare(x+i, y+i) == nullptr || _grid->getSquare(x+i, y)->bit() == nullptr)
        {
            return nullptr;
        }

        if (_grid->getSquare(x+i, y+i)->bit()->getOwner() != squareOwner)
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

Player* Connect4::ownerAt(int index) const
{
    auto square = _grid->getSquare(index % 3, index / 3);
    if (!square || !square->bit()) {
        return nullptr;
    }
    return square->bit()->getOwner();
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

int Connect4::negamax(std::string& state, int depth, int playerColor)
{
    return 0;
}