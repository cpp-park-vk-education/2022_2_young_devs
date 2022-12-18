#ifndef INC_2022_2_YOUNG_DEVS_GAME_H_
#define INC_2022_2_YOUNG_DEVS_GAME_H_

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WAnchor.h>
#include <Wt/WApplication.h>
#include <Wt/WImage.h>

#include "../GameWidget/GameWidget.h"
#include "../RulesWidget/RulesWidget.h"
#include "../ScoresWidget/ScoresWidget.h"

#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>

#include "User.h"

class Game: public Wt::WContainerWidget {
 public:
    Game();

    void handleInternalPath(const std::string &internalPath);

 private:
    User *userDB_;

    boost::asio::thread_pool pool;

    Wt::WStackedWidget *mainStack_;

    GameWidget *game_;

    ScoresWidget *scores_;

    RulesWidget *rules_;

    Wt::WAnchor *backToGameAnchor_;

    Wt::WAnchor *scoresAnchor_;

    Wt::WAnchor *rulesAnchor_;

    Wt::WContainerWidget *links_;

    void showGame();

    void showScores();

    void showRules();
};


#endif //INC_2022_2_YOUNG_DEVS_GAME_H_
