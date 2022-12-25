#include "Game.h"

#include <typeinfo>

Game::Game(size_t roomID): game_(0), scores_(0), rules_(0),
              userDB_(new User()), Wt::WContainerWidget() {
    setContentAlignment(Wt::AlignmentFlag::Center);

    std::unique_ptr<Wt::WText> title(
            std::make_unique<Wt::WText>("<h1>Strategic Tic Tac Toe</h1>"));

    addWidget(std::move(title));

    mainStack_ = new Wt::WStackedWidget();

    std::unique_ptr<Wt::WContainerWidget> comboBoxes = std::make_unique<Wt::WContainerWidget>();

    joinLink_ = mainStack_->addWidget(std::make_unique<Wt::WLineEdit>());
    joinLink_->setPlaceholderText("invite link");

    gameType_ = comboBoxes->addWidget(std::make_unique<Wt::WComboBox>());
    gameType_->addItem("strategic");
    gameType_->addItem("classic");

    comboBoxes->addWidget(std::make_unique<Wt::WBreak>());

    enemyType_ = comboBoxes->addWidget(std::make_unique<Wt::WComboBox>());
    enemyType_->addItem("bot");
    enemyType_->addItem("player");

    mainStack_->addWidget(std::move(comboBoxes));

    addWidget(std::unique_ptr<Wt::WContainerWidget>(mainStack_));

    links_ = new Wt::WContainerWidget();
    //links_->hide();
    addWidget(std::unique_ptr<Wt::WContainerWidget>(links_));

    backToGameAnchor_ = links_->addWidget(
            std::make_unique<Wt::WAnchor>("/play", "Game"));
    backToGameAnchor_->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/play"));

    links_->addWidget(std::make_unique<Wt::WBreak>());

    scoresAnchor_ = links_->addWidget(
            std::make_unique<Wt::WAnchor>("/scores", "Scores"));
    scoresAnchor_->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/scores"));

    links_->addWidget(std::make_unique<Wt::WBreak>());

    rulesAnchor_ = links_->addWidget(std::make_unique<Wt::WAnchor>("/rules", "Rules"));
    rulesAnchor_->setLink(Wt::WLink(Wt::LinkType::InternalPath, "/rules"));

    Wt::WApplication::instance()->internalPathChanged().connect(
            this, &Game::handleInternalPath);

    client_ = new Wt::Http::Client();

    client_->setMaxRedirects(10);
    client_->done().connect(this, &Game::requestDone);
}

void Game::handleInternalPath(const std::string &internalPath) {
    std::cerr << internalPath;
    if (internalPath == "/play") {
        showGame();
    } else if (internalPath == "/rules") {
        showRules();
    } else if(internalPath == "/scores") {
        showScores();
    } else {
        Wt::WApplication::instance()->setInternalPath("/rules", true);
    }
}

void Game::showGame() {
    if (!game_) {
        bool isEnemyBot = enemyType_->currentIndex() == 0;

        if (joinLink_->text() == "") {
            if (isEnemyBot) {
                client_->get("http://127.0.0.1:2000/get_room?with_bot=true");
                Wt::WApplication::instance()->enableUpdates(true);
            } else {
                client_->get("http://127.0.0.1:2000/get_room?with_bot=false");
                Wt::WApplication::instance()->enableUpdates(true);
            }
        } else {
            client_->get("http://127.0.0.1:2000/" + joinLink_->text().toUTF8());
            Wt::WApplication::instance()->enableUpdates(true);
        }
    }
}

void Game::showScores() {
    if (!scores_) {
        scores_ = mainStack_->addWidget(std::make_unique<ScoresWidget>(userDB_));
    }

    for (size_t i = 0; i < 10; i++) {

    }

    mainStack_->setCurrentWidget(scores_);
}

void Game::showRules() {
    if (!rules_) {
        rules_ = mainStack_->addWidget(std::make_unique<RulesWidget>());
    }

    mainStack_->setCurrentWidget(rules_);
}

void Game::requestDone(Wt::AsioWrapper::error_code ec, const Wt::Http::Message &msg) {
    if (!ec) {
        std::string bodyStr = msg.body();

        nlohmann::json bodyJSON = nlohmann::json::parse(bodyStr);


        bool isEnemyBot = enemyType_->currentIndex() == 0;
        size_t tableSize = (gameType_->currentIndex() == 0 ? 9 : 3);
        if (joinLink_->text() != "") {
            roomID_ = bodyJSON["room_id"];
        } else {
            roomID_ = joinLink_->text().toUTF8().back() - '0';
        }

        size_t playerID = bodyJSON["player_id"];

        game_ = mainStack_->addWidget(std::make_unique<GameWidget>(tableSize, tableSize,
                                                                   isEnemyBot, roomID_, playerID));

        Wt::WApplication::instance()->triggerUpdate();
        mainStack_->setCurrentWidget(game_);
        Wt::WApplication::instance()->enableUpdates(false);
    } else {
        std::cout << ec.message() << std::endl;
    }
}
