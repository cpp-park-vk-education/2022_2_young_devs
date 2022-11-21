#ifndef INC_2022_2_YOUNG_DEVS_GAME_H_
#define INC_2022_2_YOUNG_DEVS_GAME_H_

#include <Wt/WContainerWidget.h>

class GameWidget;

class Game: Wt::WContainerWidget {
 public:
    Game();

    void handleInternalPath(const std::string &internalPath);
 private:
    GameWidget *game_;

    Wt::WAnchor *back_;

    Wt::WAnchor *rules_;

    void showGame();

    void showRules();
};


#endif //INC_2022_2_YOUNG_DEVS_GAME_H_
