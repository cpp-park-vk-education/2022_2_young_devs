#include "game_loop.h"

void IGameLoop::Play(bool isNewGame)
{
    InitializeGame(isNewGame);
    size_t indexPlayer = 0;
    while (!EndOfGame())
    {
        MakeStep(indexPlayer);
        indexPlayer = (indexPlayer + 1) % _countPlayers;
    }
    PrintWinner();
}

TicTacToeLoop::TicTacToeLoop(IInput *input, IOutput *output, IGameLogic *gameLogic) : IGameLoop(2)
{
    _input = (input == nullptr) ? new InputConsole : input;
    _output = (output == nullptr) ? new OutputConsole : output;
    _gameLogic = (gameLogic == nullptr) ? new TicTacToeLogic(3) : gameLogic;
}

TicTacToeLoop::~TicTacToeLoop()
{
    // TODO
}

void TicTacToeLoop::InitializeGame(bool isNewGame)
{
    stopGame = false;
    _gameLogic->SetNewGame(isNewGame);
}

void TicTacToeLoop::MakeStep(size_t indexPlayer)
{
    _output->Output(_gameLogic->GetField());
    uint16_t indexCell = _input->InputIndex();
    if (indexCell == 911)
    {
        stopGame = true;
        return;
    }
    while (!_gameLogic->MakeStep(indexCell, indexPlayer))
    {
        _input->Undo();
        indexCell = _input->InputIndex();
    }
}

bool TicTacToeLoop::EndOfGame()
{
    if (stopGame)
    {
        return true;
    }
    return _gameLogic->EndOfGame();
}

void TicTacToeLoop::PrintWinner()
{
    _output->Output(_gameLogic->GetField());
    ReportGame report = _gameLogic->GetReportGame();
    if (report.isDraw)
    {
        _output->OutputMsg("The game ended in a draw\n");
    }
    else
    {
        _output->OutputMsg(std::string("Winner is ") + (report.indexPlayer == 0 ? "X\n" : "O\n"));
    }
}
