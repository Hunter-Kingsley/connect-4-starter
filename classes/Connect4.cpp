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
    return nullptr;
}

bool Connect4::checkForDraw()
{
    return false;
}

std::string Connect4::initialStateString()
{
    return " ";
}

std::string Connect4::stateString()
{
    return " ";
}

void Connect4::setStateString(const std::string &s)
{

}

bool Connect4::actionForEmptyHolder(BitHolder &holder)
{
    if (holder.bit()) return false;

    ChessSquare* square = static_cast<ChessSquare*>(&holder);
    int x = square->getColumn();
    int y = square->getRow();
    Player* currentPlayer = getCurrentPlayer();

    //std::cout << x << " " << y << std::endl;

    if (!_grid->getSquare(x, 0)->bit()) {
        Bit* newPiece = createPiece(currentPlayer);
        ChessSquare* newSquare = findLowestPossibleSquare(square);
        newPiece->setPosition(newSquare->getPosition());
        newSquare->setBit(newPiece);
    }


    return false;
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
    return nullptr;
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