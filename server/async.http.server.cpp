
#include "async.http.server.h"
#include "game_room.h"
#include "room_builder.h"
#include "helpers_func.h"

#include <optional>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp>

static std::vector<GameRoom *> rooms;

std::optional<GameRoom *> _findRoomById(std::vector<GameRoom *> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item->id == id)
		{
			return item;
		}
	}
	return std::nullopt;
}

GameRoom *CreateRoomById(size_t room_id)
{
	GameRoom *room = GameRoomBuilder()\
		.withRoomId(room_id)\
		.withGame(TypeGame::ST)\
		.withPlayers(0)\
		.withOutput(new T_ServerOutput)\
		.build();
	return room;
}

struct StepRequest
{
    size_t room_id;
    char command;
    size_t player_id;
    int value;
};

std::string getParam(std::string const &query)
{
    return std::string(query.begin() + query.find('=') + 1, query.end());
}

// StepRequest parsePath(std::string const &path)
// {
//     // std::string path(path.begin(), path.begin() + path.rfind('/'));
//     size_t room_id = std::stoul(std::string(path.begin() + path.rfind('/') + 1, path.begin() + path.rfind('?')));
//     std::string params(path.begin() + path.find('?') + 1, path.end());

//     std::stringstream ss(params);
//     std::string item;
//     std::vector<std::string> elems;
//     while (std::getline(ss, item, '&')) 
//     {
//         elems.push_back(item);
//     }

//     char command = getParam(elems[0])[0];
//     size_t player_id = std::stoul(getParam(elems[1]));
//     int value = std::stod(getParam(elems[2]));

//     return { .room_id = room_id, .command = command, .player_id = player_id, .value = value };
// }

StepRequest parsePath(const Request &request)
{
    std::cout << "BODY = " << request.body << " END BODY " <<std::endl;
    nlohmann::json j = nlohmann::json::parse(request.body);

    size_t room_id = j["rood_id"];
    size_t command = j["command"];
    size_t player_id = j["player_id"];
    int value = j["value"];

    return { .room_id = room_id, .command = command, .player_id = player_id, .value = value };
}

Response handler_post_set_step_bot(const Request &request)
{
    // StepRequest stepReq = parsePath(request.path);
    if (request.method != "POST")
    {
        Response response;
        response.status_code = 404;
        response.status_message = "Not Found";
        response.http_version = "HTTP/1.0";
        response.body = "Not Found";
        return response; 
    }
    StepRequest stepReq = parsePath(request);

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.http_version = "HTTP/1.1";
    response.body = request.body;
    std::cout << "response ==== " << request.body << std::endl; 
    return response;

    auto optional_room 	= _findRoomById(rooms, stepReq.room_id);
    GameRoom *room;
    if (!optional_room)
    {
        room = CreateRoomById(stepReq.room_id);
        rooms.push_back(room);
    }
    else
    {
        room = *optional_room;
    }
    // use data from request

    Player player = { .id = stepReq.player_id, .cell = TypeCell::X };

    TypeAction typeAction = stepReq.command == 's' ? TypeAction::Step : TypeAction::Rollback;  

    ReportAction report = room->DoAction(player, typeAction, { .value = stepReq.value });

    if (report.isValid)
    {
        if (!report.result.isEnd)
        {
            report = room->DoAction({ .isBot = true, .cell = TypeCell::O}, TypeAction::Step);
        }
    }

    std::string body = strJson(report);

    // Response response;
    // response.status_code = 200;
    // response.status_message = "OK";
    // response.http_version = "HTTP/1.1";
    // response.body = body;
    // return response;
}

Response handler_post_set_step_multi(const Request &request)
{

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.http_version = "HTTP/1.0";
    response.body = "{\"userId\": 1, \"nickname\" : \"Vinograduss\" }";
    return response;
}

Response handler_get_pull(const Request &request)
{

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.http_version = "HTTP/1.0";
    response.body = "{\"userId\": 2, \"nickname\" : \"Vinograduss\" }";
    return response;
}

Response handler_get_get_room(const Request &request)
{

    Response response;
    response.status_code = 200;
    response.status_message = "OK";
    response.http_version = "HTTP/1.0";
    response.body = "{\"userId\": 3, \"nickname\" : \"Vinograduss\" }";
    return response;
}


namespace http {
    namespace server3 {

        server::server(const std::string& address, const std::string& port,
                       std::size_t thread_pool_size)
                : thread_pool_size_(thread_pool_size),
                  signals_(io_context_),
                  acceptor_(io_context_),
                  new_connection_()
        {
            // Register to handle the signals that indicate when the server should exit.
            signals_.add(SIGINT);   // остановка процесса с терминала
            signals_.add(SIGTERM);  // сигнал от kill
            signals_.async_wait(boost::bind(&server::handle_stop, this));

            request_router.addHandler("/bot/room", handler_post_set_step_bot);
            request_router.addHandler("/multiplayer/room", handler_post_set_step_multi);
            request_router.addHandler("/pull", handler_get_pull);
            request_router.addHandler("/get_room", handler_get_get_room);

            // Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
            boost::asio::ip::tcp::resolver resolver(io_context_);
            boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(address, port).begin();
            acceptor_.open(endpoint.protocol());
            acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
            acceptor_.bind(endpoint);
            acceptor_.listen();

            start_accept();
        }

        void server::run()
        {
            // Create a pool of threads to run all of the io_contexts.
            std::vector<boost::shared_ptr<std::thread> > threads;
            for (std::size_t i = 0; i < thread_pool_size_; ++i)
            {
                boost::shared_ptr<std::thread> thread(new std::thread(
                        boost::bind(&boost::asio::io_context::run, &io_context_)));
                threads.push_back(thread);
            }

            // Wait for all threads in the pool to exit.
            for (std::size_t i = 0; i < threads.size(); ++i)
                threads[i]->join();
        }

        void server::start_accept()
        {
            new_connection_.reset(new Connection(io_context_, request_router));
            acceptor_.async_accept(new_connection_->socket(),
                                   boost::bind(&server::handle_accept, this,
                                               boost::asio::placeholders::error));
        }

        void server::handle_accept(const boost::system::error_code& e)
        {
            if (!e)
            {
                new_connection_->start();
            }

            start_accept();
        }

        void server::handle_stop()
        {
            io_context_.stop();
        }

    } // namespace server3
} // namespace http

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/lexical_cast.hpp>

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if (argc != 4)
        {
            std::cerr << "Usage: http_server <address> <port> <threads>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80 1 \n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80 1 \n";
            return 1;
        }

        // Initialise the server.
        std::size_t num_threads = boost::lexical_cast<std::size_t>(argv[3]);
        http::server3::server s(argv[1], argv[2], num_threads);

        // Run the server until stopped.
        s.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}