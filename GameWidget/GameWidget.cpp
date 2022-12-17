#include "GameWidget.h"

GameWidget::GameWidget(size_t rows, size_t columns): Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);

    //title_ = addWidget(std::make_unique<Wt::WText>("123"));

    statusText_ = addWidget(std::make_unique<Wt::WText>());

    field_ = addWidget(std::make_unique<GameField>(rows, columns)); // digits
    field_->cellPushed().connect(this, &GameWidget::registerCell);

    addWidget(std::make_unique<Wt::WBreak>());

}

void GameWidget::registerCell() {

}

