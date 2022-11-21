#ifndef INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
#define INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_

#include <Wt/WCompositeWidget.h>

#include <vector>

class GameField: public Wt::WCompositeWidget {
 public:
    GameField(size_t rows, size_t columns);

    Wt::Signal<size_t> &cellPressed();

    virtual ~GameField();
 private:
    Wt::WTable *field_;
    std::vector<Wt::WPushButton *> cellButtons_;
    std::vector<Wt::Signals::connection> connections_;

    Signal<size_t> cellPressed_;

    void processButtonPushed(Wt::WPushButton *button);
};


#endif //INC_2022_2_YOUNG_DEVS_GAMEFIELD_H_
