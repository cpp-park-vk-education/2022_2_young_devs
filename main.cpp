#include <iostream>
#include <Wt/WServer.h>
#include <Wt/WApplication.h>

// SDELAT STATIC
#include "Game/Game.h"

#include "User.h"
#include "GameProgress.h"
#include "GameInf.h"

std::unique_ptr<Wt::WApplication> createWidget(const Wt::WEnvironment& env) {
    auto app = std::make_unique<Wt::WApplication>(env);

    app->setTitle("Tic Tac");

    app->messageResourceBundle().use(app->appRoot() + "strings");

    app->root()->addWidget(std::make_unique<Game>());
    return app;
}

int main(int argc, char **argv) {
    Wt::WServer server(argc, argv, WTHTTP_CONFIGURATION);

    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

    server.addEntryPoint(Wt::EntryPointType::Application, createWidget);

    if (server.start()) {
        int sig = Wt::WServer::waitForShutdown();
        std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
        server.stop();
    }
}
