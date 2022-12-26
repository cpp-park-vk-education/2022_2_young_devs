#include "GameField.h"

#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <functional>

using namespace std::chrono_literals;

size_t convertToContinous_(size_t index)
{
    size_t block_no = index / 9;
    size_t inner_block_no = index % 9;
    size_t i = (block_no / 3 * 3) + inner_block_no / 3;
    size_t j = (block_no % 3 * 3) + inner_block_no % 3;
    return i * 9 + j;
}

std::vector<bool> GetDisabledButtons_(size_t i) {
    size_t index = i;
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

void GameField::sendPool() {
    client_->get("http://127.0.0.1:2000/pull?room_id=" +
                 std::to_string(roomID_) +
                 "&player_id=" + std::to_string(playerID_));
}

void GameField::pool2(std::string &in_) {
    while (true) {
        Wt::WServer::instance()->post(in_, std::bind(&GameField::sendPool, this));
        std::this_thread::sleep_for(300ms);
    }
}

GameField::GameField(size_t rows, size_t columns, bool isEnemyBot, size_t roomID, size_t playerID, std::string hash)
    : gameInf_(GameInf()), gameProgress_(GameProgress()), roomID_(roomID), i_(Wt::WApplication::instance()),
    Wt::WContainerWidget(), playerOrder_(true), isEnemyBot_(isEnemyBot), playerID_(playerID),
    hash_(hash) {
    Wt::WApplication::instance()->enableUpdates(true);

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

    if (isEnemyBot) {
        rollbackButton_ = addWidget(std::make_unique<Wt::WPushButton>("rollback"));
        rollbackButton_->clicked().connect(std::bind(&GameField::processRollbackButton, this));

        addWidget(std::make_unique<Wt::WBreak>());

        //saveButton_ = addWidget(std::make_unique<Wt::WPushButton>("save"));
        //saveButton_->clicked().connect(std::bind(&GameField::processSaveButton, this));
    } else {
        if (hash_ != "") {
            getInviteLink_ = addWidget(
                    std::make_unique<Wt::WPushButton>("invite link"));
            addWidget(std::make_unique<Wt::WBreak>());
            inviteLink_ = addWidget(std::make_unique<Wt::WText>());
            getInviteLink_->clicked().connect(
                    std::bind(&GameField::processGetInviteLink, this));
        }


        t_ = std::thread(std::bind(&GameField::pool2, this,
                                   Wt::WApplication::instance()->sessionId()));
    }


    client_ = new Wt::Http::Client();

    client_->setMaxRedirects(10);
    client_->done().connect(this, &GameField::requestDone);
}

GameField::~GameField() {
    for (auto &connection: connections_) {
        connection.disconnect();
    }

    t_.join();
}

void GameField::processGetInviteLink() {
    inviteLink_->setText(hash_);
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
                               "\"value\": " + std::to_string(convertToBlocks(numberOfCell)) + "}";

        Wt::Http::Message postMessage;
        postMessage.addBodyText(message);
        client_->post("http://127.0.0.1:2000/bot/room/" + std::to_string(roomID_),
                      postMessage);
    } else {
        std::string message = "{\"player_id\": " + std::to_string(playerID_) + ","
                               "\"command\": \"s\","
                               "\"value\": " + std::to_string(convertToBlocks(numberOfCell)) + "}";

        Wt::Http::Message postMessage;
        postMessage.addBodyText(message);
        client_->post("http://127.0.0.1:2000/multiplayer/room/" + std::to_string(roomID_),
                      postMessage);
    }
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
 */

void GameField::processRollbackButton() {
    std::string message = "{\"player_id\": " + std::to_string(playerID_) + ","
                           "\"command\": \"r\","
                           "\"value\": " + std::to_string(2) + "}";

    Wt::Http::Message postMessage;
    postMessage.addBodyText(message);
    client_->post("http://127.0.0.1:2000/bot/room/" + std::to_string(roomID_),
                  postMessage);
}


void GameField::processSaveButton() {
    client_->get("http://127.0.0.1:2000/bot/save/room/" + std::to_string(roomID_));
}

std::mutex mutex;

void GameField::requestDone(Wt::AsioWrapper::error_code ec, const Wt::Http::Message &msg) {
    std::lock_guard locker(mutex);
    if (!ec) {
        puts(msg.body().data());
        nlohmann::json bodyJSON = nlohmann::json::parse(msg.body());

        if (bodyJSON.contains("good")) {
            return;
        }

        //TODO
        if (bodyJSON.contains("save")) {


            //gameProgress_.addMoves(roomID_, STEPS);
            //gameInf_.updateGameStatus(roomID_, "stopped");

            return;
        }

        size_t isValid = bodyJSON["is_valid"];
        size_t isEnd = bodyJSON["is_end"];
        size_t draw = bodyJSON["draw"];
        int winnerID = bodyJSON["winner"];

        std::vector<int> cells = bodyJSON["field"];

        size_t lastIndex;
        if (!bodyJSON["steps"].empty()) {
            lastIndex = bodyJSON["steps"].back()["index"];
        } else {
            lastIndex = 100;
        }

        std::vector<bool> disabledCells(81, true);
        if (lastIndex != 100) {
            disabledCells = GetDisabledButtons_(lastIndex);
        }

        std::string value('\0', 1);
        if (isValid) {
            for (auto &cell: cellButtons_) {
                cell->enable();
            }
            puts("ENABLE");

            if (isEnd) {
                if (draw) {
                    gameStatus_->setText("Draw!");
                } else if (winnerID == playerID_) {
                    gameStatus_->setText("You won!");
                } else {
                    gameStatus_->setText("You lost!");
                }

                for (auto &cell: cellButtons_) {
                    cell->disable();
                }

            } else {
                puts("NOT IS END");
                for (size_t i = 0; i < cells.size(); i++) {
                    if (cells[i] == -1) {
                        value = "O";
                    } else if (cells[i] == 1) {
                        value = "X";
                    } else {
                        value = std::string('\0', 1);
                    }

                    puts("CONVERTTOCONTINIOUS");
                    cellButtons_[convertToContinous_(i)]->setText(value);
                    Wt::WApplication::instance()->triggerUpdate();
                }

                puts("DISABLE");
                for (size_t j = 0; j < cellButtons_.size(); j++) {
                    if (!disabledCells[j]) {
                        cellButtons_[j]->disable();
                    }
                }
            }
        }
    } else {
        std::cout << ec;
    }
}

