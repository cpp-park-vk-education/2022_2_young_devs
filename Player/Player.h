#ifndef INC_2022_2_YOUNG_DEVS_PLAYER_H_
#define INC_2022_2_YOUNG_DEVS_PLAYER_H_

#include <Wt/WContainerWidget.h>

class Player: Wt::WContainerWidget {
 public:
    Player(std::string &pathToAvatar, std::string &username);

 private:
    Wt::WImage *avatar_;

    Wt::WText *username_;
};


#endif //INC_2022_2_YOUNG_DEVS_PLAYER_H_

