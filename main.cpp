#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

#include "compressor.h"
#include "game_loop.h"

extern std::mutex mutex;
extern std::condition_variable cv;

extern ssize_t indexGlobal;
extern bool needsInput;
extern bool continueLoop;
extern bool endGame;

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
*/

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

    IGameLogic *gameLogic = new TicTacToeLogic;
    {
        IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
                                   .withGameLogic(gameLogic)
                                   .withInput(new InputEvent)
                                   .withOutput(new OutputConsole)
                                   .build();

        ticTacToe->Play();
    }

    std::cout << "*********[  Game ended   ]*********\n\n";
    std::cout << "*********[ Continue game ]*********\n\n";

    ManagerTicTacToeField manager;
    std::string strField = manager.CompressField(gameLogic->GetField());
    std::cout << "Field of last game: " << strField << '\n';
    delete gameLogic;

    // заполняем поле через строку
    IGameField *field = manager.DecompressField(strField);

    {
        IGameLogic *gameLogic = new TicTacToeLogic(3, field);

        IGameLoop *ticTacToe = GameLoopBuilder<TicTacToeLoop>()
                                   .withGameLogic(gameLogic)
                                   .withInput(new InputEvent)
                                   .withOutput(new OutputConsole)
                                   .build();

        // начинаем игру с прошлого состояния
        ticTacToe->Play(false);

        delete gameLogic;
    }

    delete field;

    std::cout << "Waiting user...\n";
    user.join();
    std::cout << "User completed!\n";
}

int main()
{
    Go();
    return 0;
}
