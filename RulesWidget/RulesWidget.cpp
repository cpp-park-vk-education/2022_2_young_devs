#include "RulesWidget.h"

RulesWidget::RulesWidget(): Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);
    //addWidget(std::make_unique<Wt::WText>(Wt::WString::tr("rules")));
    addWidget(std::make_unique<Wt::WText>("there should be rules"));
}

