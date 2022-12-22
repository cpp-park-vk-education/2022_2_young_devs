#include "GameField.h"

#include <iostream>

boost::asio::thread_pool pool(4);

static size_t IdRoom = 152;

GameField::GameField(size_t rows, size_t columns, bool isEnemyBot, size_t roomID)
    : gameInf_(GameInf()), gameProgress_(GameProgress()), Wt::WContainerWidget(),
    playerOrder_(true) {
    setContentAlignment(Wt::AlignmentFlag::Center);

    gameStatus_ = addWidget(std::make_unique<Wt::WText>());

    addWidget(std::make_unique<Wt::WBreak>());

    newGameButton_ = addWidget(std::make_unique<Wt::WPushButton>("new game"));
    newGameButton_->clicked().connect(std::bind(&GameField::processNewGameButton, this));
    newGameButton_->hide();


    //TODO
    if (gameInf_.stoppedGameExist(9)) {
        restoreButton_ = addWidget(std::make_unique<Wt::WPushButton>("restore"));
        restoreButton_->clicked().connect(std::bind(&GameField::processRestoreButton, this));
    }

    table_ = addWidget(std::make_unique<Wt::WTable>());
    table_->resize(rows, columns);

    size_t numberOfCells = rows * columns;
    for (size_t i = 0; i < numberOfCells; i++) {
        std::string c(1, '\0');
        Wt::WPushButton *cell = table_->elementAt(i / columns, i % columns)->addWidget(
                std::make_unique<Wt::WPushButton>(c)
                );

        cellButtons_.push_back(cell);
        cell->resize(Wt::WLength(30), Wt::WLength(30));

        cell->clicked().connect(std::bind(&GameField::processTableButton, this, cell));

        //connections_.push_back(Wt::WApplication::instance()->globalKeyPressed().connect(
        //        std::bind(&GameField::processButtonPushed, this, std::placeholders::_1, cell)
        //        ));
    }

    if (isEnemyBot) {
        rollbackButton_ = addWidget(std::make_unique<Wt::WPushButton>("rollback"));
        rollbackButton_->clicked().connect(std::bind(&GameField::processRollbackButton, this));

        addWidget(std::make_unique<Wt::WBreak>());

        saveButton_ = addWidget(std::make_unique<Wt::WPushButton>("save"));
        saveButton_->clicked().connect(std::bind(&GameField::processSaveButton, this));
    }

    if (rows == 9) {
        T_GameField *field = new ST_Field;
        T_GameLogic *logic = new ST_Logic;
        T_Output *output = new T_WtOutput(cellButtons_, rollbackButton_,
                                          gameStatus_, newGameButton_, gameInf_);
        T_Bot *bot = new ST_Bot;
        player_1 = {.id = 0, .isBot = false, .cell = TypeCell::X};
        player_2 = {.id = 1, .isBot = false, .cell = TypeCell::O};
        // GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot);
        room = new T_Room(IdRoom++, player_1, player_2, field, logic, output,
                          nullptr, TypeGame::ST);
    } else {
        T_GameField *field = new OT_Field;
        T_GameLogic *logic = new OT_Logic;
        T_Output *output = new T_WtOutput(cellButtons_, rollbackButton_,
                                          gameStatus_, newGameButton_, gameInf_);
        T_Bot *bot = new OT_Bot;
        player_1 = {.id = 0, .isBot = false, .cell = TypeCell::X};
        player_2 = {.id = 1, .isBot = false, .cell = TypeCell::O};
        // GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot);
        room = new T_Room(IdRoom++, player_1, player_2, field, logic, output,
                          nullptr, TypeGame::OT);
    }

    //gameInf_.addGame(player_1.id, player_2.id, "active", typegame);
}

GameField::~GameField() {
    for (auto &connection: connections_) {
        connection.disconnect();
    }
}

void GameField::processTableButton(Wt::WPushButton *button) {
    auto convertToBlocks = [](size_t index){
        size_t i = index / 9;
        size_t j = index % 9;
        size_t start_index_in_block = ((i / 3) * 3 + (j / 3)) * 9;
        return start_index_in_block + (i % 3) * 3 + (j % 3);
    };

    size_t numberOfCell = 0;
    for (size_t i = 0; i < cellButtons_.size(); i++) {
        if (cellButtons_[i] == button) {

            // cellPressed_.emit(numberOfCell++);
            numberOfCell = i;
        }
    }


    // DODELAT
    if (cellButtons_.size() == 81) {
        if (playerOrder_) {
            T_StepTask task(room, player_1, convertToBlocks(numberOfCell));
            task();
        } else {
            T_StepTask task(room, player_2, convertToBlocks(numberOfCell));
            task();
        }
    } else {
        if (playerOrder_) {
            T_StepTask task(room, player_1, numberOfCell);
            task();
        } else {
            T_StepTask task(room, player_2, numberOfCell);
            task();
        }
    }

    playerOrder_ = !playerOrder_;
    //boost::asio::post(pool, task);
    //task();
}

// TODO
void GameField::processNewGameButton() {

}

// TODO
void GameField::processRestoreButton() {
    restoreButton_->hide();

    std::vector<StepInfo> stepsFromDB = gameProgress_.getMoves(12).steps;

    room->Initialize(stepsFromDB);

    std::string cellValue(1, '\0');
    for (auto &step: stepsFromDB) {
        cellValue = (step.cell == 1 ? "X" : "O");
        cellButtons_[step.index]->setText(cellValue);
    }
}

void GameField::processRollbackButton() {
    T_RollbackTask task(room, player_1, 2);
    //boost::asio::post(pool, task);
    task();
}


// TODO
void GameField::processSaveButton() {
    //gameProgress_.addMoves(id_game, vectorSteps);
}





//void GameField::processButtonPushed(const Wt::WKeyEvent &e,
//                                    Wt::WPushButton *button) {
//    if (Wt::WCompositeWidget::isHidden())
//        return;

    //if(e.key() == static_cast<Wt::Key>(button->text().toUTF8()[0]))
    //    processButton(button);
//}
