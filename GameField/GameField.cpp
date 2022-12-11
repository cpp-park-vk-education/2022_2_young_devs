#include "GameField.h"

GameField::GameField(size_t rows, size_t columns): Wt::WCompositeWidget(),
    playerOrder_(true) {
    field_ = new Wt::WTable();

    setImplementation(std::unique_ptr<Wt::WTable>(field_));

    field_->resize(rows, columns);

    size_t numberOfCells = rows * columns;
    for (size_t i = 0; i < numberOfCells; i++) {
        std::string c(1, '\0');
        Wt::WPushButton *cell = field_->elementAt(i / columns, i % columns)->addWidget(
                std::make_unique<Wt::WPushButton>(c)
                );

        cellButtons_.push_back(cell);
        cell->resize(Wt::WLength(30), Wt::WLength(30));

        cell->clicked().connect(std::bind(&GameField::processButton, this, cell));

        //connections_.push_back(Wt::WApplication::instance()->globalKeyPressed().connect(
        //        std::bind(&GameField::processButtonPushed, this, std::placeholders::_1, cell)
        //        ));
    }
}

GameField::~GameField() {
    for (auto &connection: connections_) {
        connection.disconnect();
    }
}

void GameField::processButton(Wt::WPushButton *button) {
    button->disable();
    if (playerOrder_) {
        button->setText("X");
    } else {
        button->setText("O");
    }
    playerOrder_ = not playerOrder_;

    size_t numberOfCell = 0;
    for (const auto &cell: cellButtons_) {
        if (cell == button) {
            cellPressed_.emit(numberOfCell++);
        }
    }
}

//void GameField::processButtonPushed(const Wt::WKeyEvent &e,
//                                    Wt::WPushButton *button) {
//    if (Wt::WCompositeWidget::isHidden())
//        return;

    //if(e.key() == static_cast<Wt::Key>(button->text().toUTF8()[0]))
    //    processButton(button);
//}
