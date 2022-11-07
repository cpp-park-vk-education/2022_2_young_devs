#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>

/*
    1. Если мы вводим индексы с консоли
        Игровой цикл ожидает ввода в методе MakeStep
    2. Если сервер инициирует ввод индексов (по начажатю пользователя на ячейку в графическом интерфейсе)
        Игра должна продолжаться по этому событию

    Событийное программирование?


    Игровой цикл крутится в отдельном потоке
    Когда необходим ввод пользователя, поток засыпает 

    Происходит ввод пользователя
    Функция обработчик будет поток игрового цикла


    в вк идеи
    + билдер для инициализации игры
        инпут, аутпут, состояние и др

*/

std::mutex              mutex;
std::condition_variable cv;
static ssize_t          indexGlobal  = -1;
static bool             needsInput   = false;
static bool             continueLoop = false;
static bool             endGame      = false;

enum CELL { X = 1, E = 0, O = -1 };

class IOutput;
class IInput;
class IGameLoop;
class IGameLogic;

struct ReportGame
{
    bool isDraw = false;
    ssize_t indexPlayer = -1;
};

class IGameField
{
public:
    virtual typename std::vector<int16_t>::iterator Begin() = 0;
    virtual typename std::vector<int16_t>::iterator End() = 0;
    virtual size_t Size() const = 0;

    virtual int16_t operator[](size_t index) const = 0;
    virtual int16_t& operator[](size_t index) = 0; 
};

class IGameLogic
{
public:
    virtual void ClearState() = 0;
    virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) = 0;
    virtual bool EndOfGame() = 0;
    virtual ReportGame GetReportGame() const = 0;
    virtual IGameField *GetField() const = 0;
};

class IInput
{
public:
    virtual uint16_t    InputIndex() = 0;
    virtual void        Undo() = 0;
};

class IOutput
{
public:
    virtual void    Output(IGameField *field) const = 0;
    virtual void    OutputMsg(std::string const &message) const = 0;
};

class IGameLoop
{
protected:
    size_t _countPlayers;
    virtual void InitializeGame() = 0;
    virtual void MakeStep(size_t indexPlayer) = 0;
    virtual bool EndOfGame() = 0;
    virtual void PrintWinner() = 0;
    virtual ~IGameLoop() = default;
public:
    IGameLoop(size_t countPlayers) : _countPlayers(countPlayers) {}
    void Play()
    {
        InitializeGame();
        size_t indexPlayer = 0;
        while (!EndOfGame())
        {
            MakeStep(indexPlayer);
            indexPlayer = (indexPlayer + 1) % _countPlayers;
        }
        PrintWinner();
    }
};

class InputConsole : public IInput
{
private:
    std::vector<uint16_t> _seqIndices;
public: 
    virtual uint16_t    InputIndex() override
    {
        std::cout << "Enter index of cell\n";
        uint16_t indexCell;
        std::cin >> indexCell;
        _seqIndices.push_back(indexCell);
        return indexCell;
    }
    virtual void        Undo() override
    {
        if (!_seqIndices.empty())
        {
            _seqIndices.pop_back();
        }
    }
};

class InputEvent : public IInput
{
private:
    std::vector<uint16_t> _seqIndices;
public: 
    virtual uint16_t    InputIndex() override
    {
        {
            std::unique_lock<std::mutex> lock(mutex);
            needsInput = true;
        }
        cv.notify_one();
        {
            std::unique_lock<std::mutex> lock(mutex);
            while(!continueLoop && indexGlobal == -1)
            {
                cv.wait(lock);
            }
        }
        continueLoop = false;
        uint16_t indexLocal = indexGlobal;
        indexGlobal = -1;
        _seqIndices.push_back(indexLocal);
        return indexLocal;
    }
    virtual void        Undo() override
    {
        if (!_seqIndices.empty())
        {
            _seqIndices.pop_back();
        }
    }
};

class OutputConsole : public IOutput
{
public:
    virtual void    Output(IGameField *field) const override
    {
        for (size_t i = 0; i < field->Size() * field->Size(); ++i)
        {
            std::cout << (*field)[i] << ' ';
            if (i % field->Size() == field->Size() - 1)
            {
                std::cout << '\n';
            }
        }
    }
    virtual void    OutputMsg(std::string const &message) const override
    {
        std::cout << message << '\n';
    }
};

class TicTacToeField : public IGameField
{
public:
    TicTacToeField(size_t n = 3) : _n(n), _field(n * n, E) {}
    virtual int16_t operator[](size_t index) const override 
    {
        assert(index < _field.size());
        return _field[index];
    }
    virtual int16_t& operator[](size_t index) override 
    {
        assert(index < _field.size());
        return _field[index];
    }

    virtual size_t Size() const override 
    {
        return _n;
    }
    virtual typename std::vector<int16_t>::iterator Begin() override
    {
        return _field.begin();
    }
    virtual typename std::vector<int16_t>::iterator End() override
    {
        return _field.end();
    }
private:
    size_t                     _n; 
    std::vector<int16_t>       _field;
};

class TicTacToeLogic : public IGameLogic
{
public:
    TicTacToeLogic(size_t n = 3, IGameField *field = nullptr) : _n(n), _sums(n * 2 + 2, 0)  
    {
        _field = (field == nullptr) ? new TicTacToeField(_n) : field;
    }
    virtual void ClearState() override
    {
        _winner = E;
        std::fill(_field->Begin(), _field->End(), E);
        std::fill(_sums.begin(), _sums.end(), 0);
    }
    virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) override 
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
    virtual bool EndOfGame() override 
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
    virtual ReportGame GetReportGame() const override
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
    virtual IGameField *GetField() const override
    {
        return _field;
    }
private:
    size_t                  _n; 
    IGameField              *_field;
    std::vector<int8_t>     _sums;
    CELL            _winner = E;
    void CommitState(uint16_t indexCell, CELL cell)
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
};

class TicTacToeLoop : public IGameLoop
{
public:
    TicTacToeLoop(IInput *input = nullptr, IOutput *output = nullptr, IGameLogic *gameLogic = nullptr) : IGameLoop(2) 
    {
        _input =        (input == nullptr)      ?    new InputConsole       : input;
        _output =       (output == nullptr)     ?    new OutputConsole      : output;
        _gameLogic =    (gameLogic == nullptr)  ?    new TicTacToeLogic(3)  : gameLogic;
    }
    virtual ~TicTacToeLoop()
    {
        if (_input)
        {
            delete _input;
        }
    }
protected:
    virtual void InitializeGame() override
    {
        _gameLogic->ClearState();
    }
    virtual void MakeStep(size_t indexPlayer) override
    {
        _output->Output(_gameLogic->GetField());
        uint16_t indexCell = _input->InputIndex();
        while (!_gameLogic->MakeStep(indexCell, indexPlayer))
        {
            _input->Undo();
            indexCell = _input->InputIndex();
        }
    }
    virtual bool EndOfGame() override
    {
        return _gameLogic->EndOfGame();
    }   
    virtual void PrintWinner() override
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
private:
    // разные интерфейсы одного объекта
    IGameLogic *        _gameLogic;
    IInput  *           _input;
    IOutput *           _output;
};

// тестовые заглушки

class GameTestField : public IGameField
{
public:
    GameTestField(size_t n = 3) : _n(n), _testField(n * n, 0) 
    {
        for (size_t i = 0; i < _testField.size(); ++i)
        {
            _testField[i] = std::rand() % 10;
        }
    }
    virtual typename std::vector<int16_t>::iterator Begin() override
    {
        return _testField.begin();
    }

    virtual typename std::vector<int16_t>::iterator End() override
    {
        return _testField.end();
    }
    
    virtual size_t Size() const override
    {
        return 3;
    }
    
    virtual int16_t operator[](size_t index) const override
    {
        return _testField[index];
    }
    
    virtual int16_t& operator[](size_t index) override
    {
        return _testField[index];
    }
private:
    size_t _n;
    std::vector<int16_t> _testField;
};

class GameTestLogic : public IGameLogic
{
public:
    GameTestLogic(size_t n = 3, IGameField *field = nullptr)
    {
        _field = (field == nullptr) ? new GameTestField(n) : field;
    }
    virtual void ClearState() override
    {
        (void)0;
    }
    virtual bool MakeStep(uint16_t indexCell, size_t indexPlayer) override
    {
        (void)indexCell;
        (void)indexPlayer;
        return true;
    }
    virtual bool EndOfGame() override
    {
        static size_t cnt = 0;
        if (cnt == 5)
        {
            return true;
        }
        cnt++;
        return false;
    }
    virtual ReportGame GetReportGame() const override
    {
        ReportGame report = { .isDraw = false, .indexPlayer = 1 };
        return report;
    }

    virtual IGameField *GetField() const override
    {
        return _field;
    }
private:
    IGameField              *_field;
};


template <typename TypeGameLoop>
class GameLoopBuilder
{
public:
    IGameLoop *build()
    {
        return new TypeGameLoop(_input, _output, _gameLogic);
    }

    GameLoopBuilder &withGameLogic(IGameLogic *gameLogic)
    {
        _gameLogic = gameLogic;
        return *this;
    }

    GameLoopBuilder &withInput(IInput *input)
    {
        _input = input;
        return *this;
    }

    GameLoopBuilder &withOutput(IOutput *output)
    {
        _output = output;
        return *this;
    }

private:
    IGameLogic * _gameLogic = nullptr;
    IInput  *    _input = nullptr;
    IOutput *    _output = nullptr;
};

void UserEvents()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex);
        while (!needsInput && !endGame)
        {
            cv.wait(lock);
        }
        if (endGame)
        {
            break;
            lock.unlock();
        }
        std::cout << "Enter index of cell\n";
        uint16_t indexLocal;
        std::cin >> indexLocal;
        needsInput = false;
        indexGlobal = indexLocal;
        continueLoop = true;
        cv.notify_one();
    }
}

void Go()
{
    std::thread user(UserEvents);

    // *********** [ NETEST ] ***********
    // IGameLoop* ticTacToe = GameLoopBuilder<TicTacToeLoop>()\
    //         .withGameLogic(new TicTacToeLogic)\
    //         .withInput(new InputEvent)\
    //         .withOutput(new OutputConsole)\
    //         .build();

    // *********** [ TEST ] *********** 
    IGameLoop* ticTacToe = GameLoopBuilder<TicTacToeLoop>()\
            .withGameLogic(new GameTestLogic)\
            .withInput(new InputEvent)\
            .withOutput(new OutputConsole)\
            .build();

    ticTacToe->Play();

    std::cout << "Waiting user...\n";
    user.join();
    std::cout << "User completed!\n";
}

int main()
{
    Go();
    return 0;
}
