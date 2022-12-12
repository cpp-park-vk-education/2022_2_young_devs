#ifndef INC_2022_2_YOUNG_DEVS_PLAYERSPANEL_H_
#define INC_2022_2_YOUNG_DEVS_PLAYERSPANEL_H_

#include <Wt/WContainerWidget.h>

class Player;

class PlayersPanel: Wt::WContainerWidget {
public:
    PlayersPanel(Player &user, Player &enemy);

private:
    Player *user_;

    Player *enemy_;
};


#endif //INC_2022_2_YOUNG_DEVS_PLAYERSPANEL_H_

