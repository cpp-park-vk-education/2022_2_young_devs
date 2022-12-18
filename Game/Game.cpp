#include "Game.h"

Game::Game(): game_(0), scores_(0), rules_(0),
              userDB_(new User()), Wt::WContainerWidget() {
   setContentAlignment(Wt::AlignmentFlag::Center);
    std::unique_ptr<Wt::WText> title(
            std::make_unique<Wt::WText>("<h1>Strategic Tic Tac Toe</h1>"));

    addWidget(std::move(title));

    mainStack_ = new Wt::WStackedWidget();

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
        game_ = mainStack_->addWidget(std::make_unique<GameWidget>(9, 9));
    }

    mainStack_->setCurrentWidget(game_);
}

void Game::showScores() {
    if (!scores_) {
        scores_ = mainStack_->addWidget(std::make_unique<ScoresWidget>(userDB_));
    }

    mainStack_->setCurrentWidget(scores_);
}

void Game::showRules() {
    if (!rules_) {
        rules_ = mainStack_->addWidget(std::make_unique<RulesWidget>());
    }

    mainStack_->setCurrentWidget(rules_);
}
