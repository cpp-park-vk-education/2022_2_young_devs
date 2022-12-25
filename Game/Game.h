#ifndef INC_2022_2_YOUNG_DEVS_GAME_H_
#define INC_2022_2_YOUNG_DEVS_GAME_H_

#include <Wt/WContainerWidget.h>
#include <Wt/WComboBox.h>
#include <Wt/WText.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WAnchor.h>
#include <Wt/WApplication.h>
#include <Wt/WImage.h>

#include <Wt/Http/Client.h>
#include <Wt/Http/Message.h>
#include "Wt/AsioWrapper/asio.hpp"

#include <nlohmann/json.hpp>

#include "../GameWidget/GameWidget.h"
#include "../RulesWidget/RulesWidget.h"
#include "../ScoresWidget/ScoresWidget.h"

#include "User.h"

class Game: public Wt::WContainerWidget {
 public:
    Game(size_t roomID);

    void handleInternalPath(const std::string &internalPath);

 private:
    size_t roomID_;

    User *userDB_;

    Wt::WStackedWidget *mainStack_;

    Wt::WComboBox *gameType_;

    Wt::WComboBox *enemyType_;

    GameWidget *game_;

    ScoresWidget *scores_;

    RulesWidget *rules_;

    Wt::WAnchor *backToGameAnchor_;

    Wt::WAnchor *scoresAnchor_;

    Wt::WAnchor *rulesAnchor_;

    Wt::WContainerWidget *links_;

    Wt::Http::Client *client_;

    void showGame();

    void requestDone(Wt::AsioWrapper::error_code ec, const Wt::Http::Message &msg);

    void showScores();

    void doNothing() {

    }

    void showRules();
};


#endif //INC_2022_2_YOUNG_DEVS_GAME_H_
