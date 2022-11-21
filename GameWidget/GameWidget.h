#ifndef INC_2022_2_YOUNG_DEVS_GAMEWIDGET_H_
#define INC_2022_2_YOUNG_DEVS_GAMEWIDGET_H_

#include <Wt/WContainerWidget.h>

class GameField;
class PlayersPanel;

class GameWidget: public Wt::WContainerWidget {
 public:
    GameWidget(const std::string &gameName);

 private:
    Wt::WText *title_;

    PlayersPanel *panel_;

    Wt::WText *statusText_

    GameField *field_;

    void regiserCell();
};


#endif //INC_2022_2_YOUNG_DEVS_GAMEWIDGET_H_

