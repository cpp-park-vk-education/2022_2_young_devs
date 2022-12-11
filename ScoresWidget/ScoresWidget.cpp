#include "ScoresWidget.h"

ScoresWidget::ScoresWidget(): Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);

    addWidget(std::make_unique<Wt::WText>("<h2>Best players</h2>"));

    table_ = addWidget(std::make_unique<Wt::WTable>());

    table_->setWidth(Wt::WLength("50%"));

    table_->elementAt(0, 0)->addWidget(
            std::make_unique<Wt::WText>("Rank"));

    table_->elementAt(0, 1)->addWidget(
            std::make_unique<Wt::WText>("Username"));

    table_->elementAt(0, 2)->addWidget(
            std::make_unique<Wt::WText>("Games"));

    table_->elementAt(0, 3)->addWidget(
            std::make_unique<Wt::WText>("Victories"));

    table_->elementAt(0, 4)->addWidget(
            std::make_unique<Wt::WText>("Win rate"));


    table_->elementAt(1, 0)->addWidget(std::make_unique<Wt::WText>("1"));
    table_->elementAt(1, 1)->addWidget(std::make_unique<Wt::WText>("Michael"));
    table_->elementAt(1, 2)->addWidget(std::make_unique<Wt::WText>("10"));
    table_->elementAt(1, 3)->addWidget(std::make_unique<Wt::WText>("10"));
    table_->elementAt(1, 4)->addWidget(std::make_unique<Wt::WText>("100"));

    table_->elementAt(2, 0)->addWidget(std::make_unique<Wt::WText>("2"));
    table_->elementAt(2, 1)->addWidget(std::make_unique<Wt::WText>("Maxim"));
    table_->elementAt(2, 2)->addWidget(std::make_unique<Wt::WText>("10"));
    table_->elementAt(2, 3)->addWidget(std::make_unique<Wt::WText>("8"));
    table_->elementAt(2, 4)->addWidget(std::make_unique<Wt::WText>("80"));

    table_->elementAt(3, 0)->addWidget(std::make_unique<Wt::WText>("3"));
    table_->elementAt(3, 1)->addWidget(std::make_unique<Wt::WText>("Dmitriy"));
    table_->elementAt(3, 2)->addWidget(std::make_unique<Wt::WText>("10"));
    table_->elementAt(3, 3)->addWidget(std::make_unique<Wt::WText>("0"));
    table_->elementAt(3, 4)->addWidget(std::make_unique<Wt::WText>("0"));
}
