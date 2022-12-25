#include "connection.h"
#include "game_room.h"
#include "room_builder.h"
#include "helpers_func.h"


#include <optional>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/beast/http/write.hpp>
#include <iostream>

#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

static size_t ID = 0;
static size_t PLAYER_ID = 0;

struct Room
{
    size_t id;
    T_Room *room = nullptr;
    std::optional<Player> player_1 = std::nullopt;
    std::optional<Player> player_2 = std::nullopt;
    bool notify = false;
    ReportAction lastReport;
};

// static std::vector<GameRoom *> rooms;
static std::vector<Room> rooms;

http::response<http::string_body> getRespOk(http::request<http::string_body> const &request)
{
    http::response<http::string_body> res{http::status::bad_request, request.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.result(http::status::ok);
    return res;
}

http::response<http::string_body> getRespError(http::request<http::string_body> const &request)
{
    http::response<http::string_body> res{http::status::bad_request, request.version()};
    res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    res.set(http::field::content_type, "application/json");
    res.result(http::status::not_found);
    res.body() = " Not found";  
    return res;
}

// localhost/bot/room/23?player_id=5    -> 23
// localhost/bot/room/23                -> 23
size_t getRoomIdFromURL(std::string const &url)
{
    return std::stoul(std::string(url.begin() + url.rfind('/') + 1, url.find('?') == std::string::npos ? url.end() : url.begin() + url.find('?')));
}

std::vector<std::string> getVecParams(std::string const &url)
{
    std::string params(url.begin() + url.find('?') + 1, url.end());

    std::stringstream ss(params);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, '&')) 
    {
        elems.push_back(item);
    }
    return elems;
}

Room &GetRoomStructById(std::vector<Room> &items, size_t id)
{
    for (auto &&item : items)
	{
		if (item.id == id)
		{
			return item;
		}
	}
	throw std::runtime_error("exception GetRoomStructById...");
}


// std::optional<GameRoom *> findRoomById(std::vector<GameRoom *> &items, size_t id)
std::optional<T_Room *> findRoomById(std::vector<Room> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item.id == id)
		{
			return item.room;
		}
	}
	return std::nullopt;
}

std::optional<Room> findRoomStructById(std::vector<Room> &items, size_t id)
{
	for (auto &&item : items)
	{
		if (item.id == id)
		{
			return item;
		}
	}
	return std::nullopt;
}

bool UpdateLastReport(std::vector<Room> &items, size_t id, ReportAction const &report)
{
    for (auto &&item : items)
	{
		if (item.id == id)
		{
            item.lastReport = report;
            item.notify = false;
			return true;
		}
	}
    return false;
}


T_Room *CreateRoomWithBot(size_t room_id, size_t player_id)
{
	T_Room *room = GameRoomBuilder()\
		.withRoomId(room_id)\
		.withGame(TypeGame::ST)\
		.withPlayers(player_id)\
		.withOutput(new T_ServerOutput)\
		.build();
	return room;
}

size_t CreateEmptyRoom()
{
    rooms.push_back({ .id = ++ID});
    return ID;
}

struct StepRequest
{
    // size_t room_id;
    char command;
    size_t player_id;
    int value;
};

StepRequest parsePostBody(std::string const &body)
{
    std::cout << "BODY = " << body << " END BODY " <<std::endl;
    nlohmann::json j = nlohmann::json::parse(body);

    // size_t room_id              = j["room_id"];
    std::string str_command     = j["command"];
    char command                = str_command[0];
    size_t player_id            = j["player_id"];
    int value                   = j["value"];

    puts("1");

    return { /* .room_id = room_id, */.command = command, .player_id = player_id, .value = value };
}

http::response<http::string_body> handlerSetStepBot(http::request<http::string_body> const &request)
{
    
    http::response<http::string_body> res = getRespOk(request);

    size_t room_id = getRoomIdFromURL(std::string(request.target()));

    StepRequest stepReq = parsePostBody(request.body());

    // auto optional_room 	= findRoomById(rooms, stepReq.room_id);
    auto optional_room 	= findRoomById(rooms, room_id);

    puts("2");
    
    if (!optional_room)
    {
        res.body() = "{ \"is_valid\" : false, \"code_error\": 20, \"message_error\" : \"Game room does not exist\"}";
        return res;
    }
    puts("3");
    T_Room *room = *optional_room;

    puts("4");
    Player player = { .id = stepReq.player_id, .cell = TypeCell::X };

    std::cout << "command = " << stepReq.command << std::endl;
    std::cout << "stepReq.value = " << stepReq.value << std::endl;

    TypeAction typeAction = stepReq.command == 's' ? TypeAction::Step : TypeAction::Rollback;  

    puts("5");
    ReportAction report = room->DoAction(player, typeAction, { .value = stepReq.value });

    puts("6");
    if (report.isValid)
    {
        if (!report.result.isEnd)
        {
            puts("6.5");
            report = room->DoAction({ .isBot = true, .cell = TypeCell::O}, TypeAction::Step);
        }
    }
    puts("7");

    std::string body = strJson(report);

    puts("8");
    res.body() = body;
    return res;
}

http::response<http::string_body> handlerSetStepMulti(http::request<http::string_body> const &request)
{
    size_t room_id = getRoomIdFromURL(std::string(request.target()));

    http::response<http::string_body> res = getRespOk(request);

    StepRequest stepReq = parsePostBody(request.body());

    // auto optional_room_struct = findRoomStructById(rooms, room_id); 

    Room &roomStruct = GetRoomStructById(rooms, room_id);

    // if (!optional_room_struct)
    // {
    //     res.body() = "{ \"is_valid\" : false, \"code_error\": 20, \"message_error\" : \"Game room does not exist\"}";
    //     return res;
        // // room = CreateRoomById(stepReq.room_id);
        // // rooms.push_back(room);
    // }
    // Room roomStruct = *optional_room_struct;
    // T_Room *room = roomStruct.room;
    T_Room *room = roomStruct.room;

    Player player = { .id = stepReq.player_id, .cell = TypeCell::X };

    TypeAction typeAction = stepReq.command == 's' ? TypeAction::Step : TypeAction::Rollback;  

    ReportAction report = room->DoAction(player, typeAction, { .value = stepReq.value });

    if (report.isValid)
    {
        roomStruct.lastReport = report;
        roomStruct.notify = false;
        // UpdateLastReport(rooms, roomStruct.id, report); //  + notify = false
    }

    std::string body = strJson(report);

    res.body() = body;
    return res;
}

std::string getParam(std::string const &query)
{
    return std::string(query.begin() + query.find('=') + 1, query.end());
}

/*
    Запрос: localhost/pull?room_id=x&player_id=y
    Ответ: { good : true | false,
             [report]
            }
*/

http::response<http::string_body> handlerPull(http::request<http::string_body> const &request)
{
    http::response<http::string_body> res = getRespOk(request);
    
    std::vector<std::string> elems = getVecParams(std::string(request.target()));

    // std::string params(request.target().begin() + request.target().find('?') + 1, request.target().end());

    // std::stringstream ss(params);
    // std::string item;
    // std::vector<std::string> elems;
    // while (std::getline(ss, item, '&')) 
    // {
    //     elems.push_back(item);
    // }

    size_t room_id      = std::stoul(getParam(elems[0]));
    size_t player_id    = std::stoul(getParam(elems[1]));

    auto optional_room_struct = findRoomStructById(rooms, room_id);

    if (!optional_room_struct)
    {
        res.body() = "{ \"good\" : false, \"code_error\": 30, \"message_error\" : \"Game room structure does not exist\"}";
        return res;
    }

    Room roomStruct = *optional_room_struct;
    if (roomStruct.room && !roomStruct.notify && (roomStruct.player_1->id == player_id || roomStruct.player_2->id == player_id))
    {
        res.body() = strJson(roomStruct.lastReport);
        return res;
    }

    return res;
}

/*
    Запрос: localhost/get_room
    Ответ: { room_id : x }
*/

http::response<http::string_body> handlerGetRoom(http::request<http::string_body> const &request)
{
    http::response<http::string_body> res = getRespOk(request);
    
    std::vector<std::string> elems = getVecParams(std::string(request.target()));

    // std::cout << " elems[0] === " << elems[0] << std::endl;
    bool withBot = (getParam(elems[0]) == "true" ? true : false);

    size_t room_id = CreateEmptyRoom();
    Room &roomStruct = GetRoomStructById(rooms, room_id);
    roomStruct.player_1 = std::optional<Player>({ .id = PLAYER_ID++ });
    
    if (withBot)
    {
        puts("withBot");
        roomStruct.room = CreateRoomWithBot(room_id, roomStruct.player_1->id);
        roomStruct.player_2 = std::optional<Player>({ .isBot = true });
    }

    res.body() = "{ \"room_id\" : " + std::to_string(room_id) + ", player_id : " + std::to_string(roomStruct.player_1->id) + "}";
    return res;
}

namespace server3 {

        Connection::Connection(tcp::socket&& socket)
                : stream_(std::move(socket))
        {
            requestRouter_.addHandler("/bot/room", handlerSetStepBot);
            requestRouter_.addHandler("/bot/multiplayer", handlerSetStepMulti);
            requestRouter_.addHandler("/pull", handlerPull);
            requestRouter_.addHandler("/get_room", handlerGetRoom);
            requestRouter_.addHandler("/join", handlerGetRoom);
        }


        void Connection::start()
        {
            net::dispatch(stream_.get_executor(),
                          beast::bind_front_handler(&Connection::do_read,
                                             shared_from_this()));

        }

        void Connection::do_read()
        {
            request_ = {};

            // Read a request
            http::async_read(stream_, buffer_, request_,
                             beast::bind_front_handler(
                                     &Connection::handle_read,
                                     shared_from_this()));
        }

        void Connection::handle_read(beast::error_code e,
                                     std::size_t bytes_transferred)
        {
            if( e == http::error::end_of_stream)
                return do_close();

            if (!e)
            {
                http::response<http::string_body> res = requestRouter_.processRoute(request_);

                res.prepare_payload();

                auto sp = std::make_shared<http::message<false, http::string_body>>(std::move(res));
                res_ = sp;

                http::async_write(stream_,
                        *sp,
                        beast::bind_front_handler(
                                &Connection::handle_write,
                                shared_from_this(),
                                sp->need_eof()));
            }
        }

        void Connection::handle_write(bool close,
                                      beast::error_code e,
                                      std::size_t bytes_transferred)
        {
            if(close)
            {
                // This means we should close the connection, usually because
                // the response indicated the "Connection: close" semantic.
                return do_close();
            }

            if (!e)
            {
                do_read();
            }
        }

        void Connection::do_close()
        {
            beast::error_code ec;
            stream_.socket().shutdown(tcp::socket::shutdown_send, ec);

        }

    } // namespace server3

