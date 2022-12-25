#ifndef INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
#define INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_

#include <vector>
#include <Wt/WCompositeWidget.h>
#include <Wt/WTable.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <Wt/WStackedWidget.h>
#include <Wt/Http/Client.h>
#include <Wt/Http/Message.h>

#include <iostream>
#include <thread>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "tasks.h"

#include "game_logic.h"
#include "game_room.h"
#include "GameInf.h"
#include "GameProgress.h"

class GameField: public Wt::WContainerWidget {
 public:
    GameField(size_t rows, size_t columns, bool isEnemyBot, size_t roomID, size_t playerID);
    Wt::Signal<size_t> &cellPushed() {
        return cellPressed_;
    }

    virtual ~GameField();

 private:
    GameInf gameInf_;

    GameProgress gameProgress_;

    size_t roomID_;

    size_t playerID_;

    void poll();

    /*
    Player player_1;

    Player player_2;

    T_Room *room;
    */
    bool isEnemyBot_;

    Wt::Http::Client *client_;

    bool playerOrder_;

    Wt::WStackedWidget *field_;

    Wt::WText *gameStatus_;

    Wt::WTable *table_;

    std::vector<Wt::WPushButton *> cellButtons_;

    std::vector<Wt::Signals::connection> connections_;

    Wt::Signal<size_t> cellPressed_;

    void requestDone(Wt::AsioWrapper::error_code ec, const Wt::Http::Message &msg);

    void processNewGameButton();

    void processRestoreButton();

    void processRollbackButton();

    void processSaveButton();

    void processTableButton(Wt::WPushButton *button);

    Wt::WPushButton *newGameButton_;

    Wt::WPushButton *restoreButton_;

    Wt::WPushButton *rollbackButton_;

    Wt::WPushButton *saveButton_;

    //void processButtonPushed(const Wt::WKeyEvent &e, Wt::WPushButton *button);
};


#endif //INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
