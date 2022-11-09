#include "game_logic.h"
#include <algorithm>

TicTacToeLogic::TicTacToeLogic(size_t n, IGameField *field) : _n(n), _sums(n * 2 + 2, 0)
{
    _field = (field == nullptr) ? new TicTacToeField(_n) : field;
}
// копирует данные, указателем не владеет
// virtual void setState(IGameField *field = nullptr) override
void TicTacToeLogic::SetNewGame(bool isNewGame)
{
    if (isNewGame)
    {
        _field->Clear();
        _winner = E;
        std::fill(_sums.begin(), _sums.end(), 0);
    }
}
bool TicTacToeLogic::MakeStep(uint16_t indexCell, size_t indexPlayer)
{
    CELL cell = indexPlayer % 2 == 0 ? X : O;
    if (indexCell >= (_n * _n) || (*_field)[indexCell] != E)
    {
        return false;
    }
    (*_field)[indexCell] = (int16_t)cell;
    CommitState(indexCell, cell);
    return true;
}
bool TicTacToeLogic::EndOfGame()
{
    for (size_t i = 0; i < _sums.size(); ++i)
    {
        if (_sums[i] == _n)
        {
            _winner = X;
            return true;
        }
        if (_sums[i] == -_n)
        {
            _winner = O;
            return true;
        }
    }
    auto it = std::find(_field->Begin(), _field->End(), E);
    if (it == _field->End())
    {
        _winner = E;
        return true;
    }
    return false;
}
ReportGame TicTacToeLogic::GetReportGame() const
{
    ReportGame report;
    if (_winner == E)
    {
        report.isDraw = true;
    }
    else
    {
        report.isDraw = false;
        report.indexPlayer = _winner == X ? 0 : 1;
    }
    return report;
}
IGameField *TicTacToeLogic::GetField() const
{
    return _field;
}

void TicTacToeLogic::CommitState(uint16_t indexCell, CELL cell)
{
    _sums[indexCell / _n] += cell;
    _sums[indexCell % _n + _n] += cell;
    if (indexCell % _n == indexCell / _n)
    {
        _sums[_n * 2] += cell;
    }
    if (indexCell / _n == (_n - 1) - indexCell % _n)
    {
        _sums[_n * 2 + 1] += cell;
    }
}

GameTestLogic::GameTestLogic(size_t n, IGameField *field)
{
    _field = (field == nullptr) ? new GameTestField(n) : field;
}
void GameTestLogic::SetNewGame(bool isNewGame)
{
    _field->Clear();
}
bool GameTestLogic::MakeStep(uint16_t indexCell, size_t indexPlayer)
{
    (void)indexCell;
    (void)indexPlayer;
    return true;
}
bool GameTestLogic::EndOfGame()
{
    static size_t cnt = 0;
    if (cnt == 5)
    {
        return true;
    }
    cnt++;
    return false;
}
ReportGame GameTestLogic::GetReportGame() const
{
    ReportGame report = {.isDraw = false, .indexPlayer = 1};
    return report;
}

IGameField *GameTestLogic::GetField() const
{
    return _field;
}
