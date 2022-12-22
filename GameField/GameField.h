#ifndef INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
#define INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_

#include <vector>
#include <Wt/WCompositeWidget.h>
#include <Wt/WTable.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <Wt/WStackedWidget.h>

#include <iostream>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "tasks.h"

#include "game_logic.h"
#include "game_room.h"

class GameField: public Wt::WContainerWidget {
 public:
    GameField(size_t rows, size_t columns, bool isEnemyBot, size_t roomID);
    Wt::Signal<size_t> &cellPushed() {
        return cellPressed_;
    }

    virtual ~GameField();

 private:
    Player player_1;

    Player player_2;

    GameRoom *room;

    bool playerOrder_;

    Wt::WStackedWidget *field_;

    Wt::WText *gameStatus_;

    Wt::WTable *table_;

    std::vector<Wt::WPushButton *> cellButtons_;

    std::vector<Wt::Signals::connection> connections_;

    Wt::Signal<size_t> cellPressed_;

    void processRollbackButton();

    void processTableButton(Wt::WPushButton *button);

    Wt::WPushButton *rollbackButton_;

    //void processButtonPushed(const Wt::WKeyEvent &e, Wt::WPushButton *button);
};


#endif //INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
