#ifndef INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
#define INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_

#include <vector>
#include <Wt/WCompositeWidget.h>
#include <Wt/WTable.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>

#include <iostream>

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "tasks.h"

#include "game_logic.h"
#include "game_room.h"

class GameField: public Wt::WCompositeWidget {
 public:
    GameField(size_t rows = 3, size_t columns = 3);
    Wt::Signal<size_t> &cellPushed() {
        return cellPressed_;
    }

    virtual ~GameField();

 private:
    Player player_1;

    Player player_2;

    GameRoom *room;

    bool playerOrder_;

    Wt::WTable *field_;

    std::vector<Wt::WPushButton *> cellButtons_;

    std::vector<Wt::Signals::connection> connections_;

    Wt::Signal<size_t> cellPressed_;

    void processButton(Wt::WPushButton *button);

    Wt::WPushButton *rollbackButton_;

    //void processButtonPushed(const Wt::WKeyEvent &e, Wt::WPushButton *button);
};


#endif //INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
