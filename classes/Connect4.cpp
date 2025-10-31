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
    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    // top left checks
    if (CheckRow(square)) {return squareOwner;}
    if (CheckCol(square)) {return squareOwner;}
    if (CheckDiag(square, -1)) {return squareOwner;}

    // bottom left checks
    if (CheckRow(_grid->getSquare(x, y+3))) {return squareOwner;}
    if (CheckDiag(_grid->getSquare(x, y+3), 1)) {return squareOwner;}

    // top right check
    if (CheckCol(_grid->getSquare(x+3, y))) {return squareOwner;}

    return nullptr;
}

bool Connect4::CheckRow(ChessSquare* square)
{
    if (square->bit() == nullptr) {return false;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; i < 3; i++)
    {
        if (_grid->getSquare(x+1, y)->bit() == nullptr)
        {
            return false;
        }

        if (_grid->getSquare(x+i, y)->bit()->getOwner() != squareOwner)
        {
            return false;
        }
    }

    return true;
}
bool Connect4::CheckCol(ChessSquare* square)
{
    if (square->bit() == nullptr) {return false;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; i < 3; i++)
    {
        if (_grid->getSquare(x, y+i)->bit() == nullptr)
        {
            return false;
        }

        if (_grid->getSquare(x, y+i)->bit()->getOwner() != squareOwner)
        {
            return false;
        }
    }

    return true;
}
bool Connect4::CheckDiag(ChessSquare* square, int direction)
{
    // upwards should be 1 or -1 for the function to work

    if (square->bit() == nullptr) {return false;}

    Player* squareOwner = square->bit()->getOwner();
    int x = square->getColumn();
    int y = square->getRow();

    for (int i = 0; i < 3; i += direction)
    {
        if (_grid->getSquare(x+i, y+i)->bit() == nullptr)
        {
            return false;
        }

        if (_grid->getSquare(x+i, y+i)->bit()->getOwner() != squareOwner)
        {
            return false;
        }
    }

    return true;
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