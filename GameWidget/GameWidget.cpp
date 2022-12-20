#include "GameWidget.h"

GameWidget::GameWidget(size_t rows, size_t columns, bool isEnemyBot): Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);

    statusText_ = addWidget(std::make_unique<Wt::WText>());

    field_ = addWidget(std::make_unique<GameField>(rows, columns, isEnemyBot));
    field_->cellPushed().connect(this, &GameWidget::registerCell);

    addWidget(std::make_unique<Wt::WBreak>());

}

void GameWidget::registerCell() {

}

