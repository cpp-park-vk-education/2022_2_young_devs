#include "GameField.h"

#include <iostream>
#include <nlohmann/json.hpp>

size_t convertToContinous_(size_t index)
{
    size_t block_no = index / 9;
    size_t inner_block_no = index % 9;
    size_t i = (block_no / 3 * 3) + inner_block_no / 3;
    size_t j = (block_no % 3 * 3) + inner_block_no % 3;
    return i * 9 + j;
}

std::vector<bool> GetDisabledButtons_(ReportAction report) {
    if (report.steps.empty()) {
        return std::vector<bool>(81, true);
    }
    size_t index = report.steps.back().index;
    size_t block = index % 9;
    std::vector<bool> indices(81);
    for (size_t i = 0; i < 81; ++i) {
        if (i / 9 != block) {
            indices[convertToContinous_(i)] = false;
        }
        else {
            indices[convertToContinous_(i)] = true;
        }
    }
    return indices;
}

boost::asio::thread_pool pool(4);

static size_t IdRoom = 166;

GameField::GameField(size_t rows, size_t columns, bool isEnemyBot, size_t roomID, size_t playerID)
    : gameInf_(GameInf()), gameProgress_(GameProgress()), roomID_(roomID),
    Wt::WContainerWidget(), playerOrder_(true), isEnemyBot_(isEnemyBot), playerID_(playerID) {
    setContentAlignment(Wt::AlignmentFlag::Center);

    gameStatus_ = addWidget(std::make_unique<Wt::WText>());

    addWidget(std::make_unique<Wt::WBreak>());

    newGameButton_ = addWidget(std::make_unique<Wt::WPushButton>("new game"));
    newGameButton_->clicked().connect(std::bind(&GameField::processNewGameButton, this));
    newGameButton_->hide();


    /*
    bool isExist = gameInf_.stoppedGameExist(8);
    //TODO
    if (rows != 3) {
        if (isExist) {
            restoreButton_ = addWidget(
                    std::make_unique<Wt::WPushButton>("restore"));
            restoreButton_->clicked().connect(
                    std::bind(&GameField::processRestoreButton, this));
        }
    }
     */

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
    /*
    if (isEnemyBot) {
        rollbackButton_ = addWidget(std::make_unique<Wt::WPushButton>("rollback"));
        rollbackButton_->clicked().connect(std::bind(&GameField::processRollbackButton, this));

        addWidget(std::make_unique<Wt::WBreak>());

        saveButton_ = addWidget(std::make_unique<Wt::WPushButton>("save"));
        saveButton_->clicked().connect(std::bind(&GameField::processSaveButton, this));
    }
     */
    /*
    if (rows == 9) {
        T_GameField *field = new ST_Field;
        T_GameLogic *logic = new ST_Logic;
        T_Output *output = new T_WtOutput(cellButtons_, rollbackButton_,
                                          gameStatus_, newGameButton_, gameInf_);
        T_Bot *bot = new ST_Bot;
        player_1 = {.id = 0, .isBot = false, .cell = TypeCell::X};
        if (!isEnemyBot) {
            player_2 = {.id = 1, .isBot = false, .cell = TypeCell::O};
            room = new T_Room(IdRoom, player_1, player_2, field, logic, output,
                              nullptr, TypeGame::ST);
        } else {
            player_2 = {.isBot = true, .cell = TypeCell::O};
            room = new T_Room(IdRoom, player_1, player_2, field, logic, output,
                              bot, TypeGame::ST);
        }
        // GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot);
        //room = new T_Room(IdRoom, player_1, player_2, field, logic, output,
        //                  nullptr, TypeGame::ST);

        if (!isExist) {
            gameInf_.addGame(8, 20, TypeGame::ST);
        }
    } else {
        T_GameField *field = new OT_Field;
        T_GameLogic *logic = new OT_Logic;
        T_Output *output = new T_WtOutput(cellButtons_, rollbackButton_,
                                          gameStatus_, newGameButton_, gameInf_);
        T_Bot *bot = new OT_Bot;
        player_1 = {.id = 0, .isBot = false, .cell = TypeCell::X};
        player_2 = {.id = 1, .isBot = false, .cell = TypeCell::O};
        // GameRoom *room 		= new T_Room(player_1, player_2, field, logic, output, bot);
        room = new T_Room(IdRoom, player_1, player_2, field, logic, output,
                          nullptr, TypeGame::OT);
        if (!isExist) {
            gameInf_.addGame(4, 20, TypeGame::OT);
        }
    }
    */

    client_ = new Wt::Http::Client();

    client_->setMaxRedirects(10);
    client_->done().connect(this, &GameField::requestDone);
    //std::thread t(&GameField::poll, this);
    //t.join();
}

void GameField::poll() {
    while (true) {
        client_->get("127.0.0.1/pull?room_id=" + std::to_string(roomID_) +
        "&player_id=" + std::to_string(playerID_));
    }
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


    if (isEnemyBot_) {
        std::string message = "{\"player_id\": " + std::to_string(playerID_) + ","
                               "\"command\": \"s\","
                               "\"value\": " + std::to_string(numberOfCell) + "}";

        Wt::Http::Message postMessage;
        postMessage.addBodyText(message);
        client_->post("127.0.0.1:2000/bot/room/" + std::to_string(roomID_),
                      postMessage);
    } else {

    }


    /*
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
     */

    playerOrder_ = !playerOrder_;
    //boost::asio::post(pool, task);
    //task();
}

// TODO
void GameField::processNewGameButton() {

}
/*
// TODO
void GameField::processRestoreButton() {
    restoreButton_->hide();

    std::vector<StepInfo> stepsFromDB = gameProgress_.getMoves(IdRoom).steps;

    room->Initialize(stepsFromDB);

    std::string cellValue(1, '\0');
    for (auto &step: stepsFromDB) {
        cellValue = (step.cell == 1 ? "X" : "O");
        cellButtons_[step.index]->setText(cellValue);
    }

    size_t i = 0;
    std::vector<bool> indices = GetDisabledButtons_({.steps = stepsFromDB});
    for (const auto &index: indices) {
        if (index) {
            cellButtons_[i]->enable();
        } else {
            cellButtons_[i]->disable();
        }
        i++;
    }
}

void GameField::processRollbackButton() {
    T_RollbackTask task(room, player_1, 2);
    //boost::asio::post(pool, task);
    task();
}


// TODO
void GameField::processSaveButton() {
    for (size_t i = 0; i < 10; i++) {
        std::cout << IdRoom << std::endl;
    }
    gameProgress_.addMoves(IdRoom, room->getSteps());
    gameInf_.updateGameStatus(IdRoom, "stopped");

    for (auto &cell: cellButtons_) {
        cell->disable();
    }
}

*/



//void GameField::processButtonPushed(const Wt::WKeyEvent &e,
//                                    Wt::WPushButton *button) {
//    if (Wt::WCompositeWidget::isHidden())
//        return;

    //if(e.key() == static_cast<Wt::Key>(button->text().toUTF8()[0]))
    //    processButton(button);
//}

void requestDone(Wt::AsioWrapper::error_code ec, const Wt::Http::Message &msg) {
    nlohmann::json bodyJSON = nlohmann::json::parse(msg.body());

    if (bodyJSON["is_valid"]) {
        for (size_t i = 0; i < 10; i++) {
            std::cout << bodyJSON["room_id"] << std::endl;
        }
    }
}

