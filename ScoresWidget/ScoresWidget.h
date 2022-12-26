#ifndef INC_2022_2_YOUNG_DEVS_SCORESWIDGET_H_
#define INC_2022_2_YOUNG_DEVS_SCORESWIDGET_H_

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WTable.h>

#include <vector>

#include "User.h"

class ScoresWidget: public Wt::WContainerWidget {
public:
    ScoresWidget(User *userDB);

private:
    User *userDB_;

    Wt::WTable *table_;
};


#endif //INC_2022_2_YOUNG_DEVS_SCORESWIDGET_H_
