#include "GameWidget.h"

GameWidget::GameWidget(size_t rows, size_t columns, bool isEnemyBot, size_t roomID)
    : Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);

    statusText_ = addWidget(std::make_unique<Wt::WText>());

    field_ = addWidget(std::make_unique<GameField>(rows, columns, isEnemyBot, roomID));
    field_->cellPushed().connect(this, &GameWidget::registerCell);

    addWidget(std::make_unique<Wt::WBreak>());

}

void GameWidget::registerCell() {

}

