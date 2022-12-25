
#ifndef BOOST_ASIO_SERVER_ROUTER_H
#define BOOST_ASIO_SERVER_ROUTER_H

namespace http {
    namespace server3 {
template<typename Handler>
class Router {
public:
    void addHandler(std::string method, Handler handler) {
        m_Handlers.emplace(method, handler);
    }

    Response processRoute(const std::string &method, const Request &request) {
        std::string path(method.begin(), method.begin() + method.rfind('/'));
        auto handler_it = m_Handlers.find(path);
        if (handler_it != m_Handlers.end()) {
            return (handler_it->second)(request);
        }

        Response response;
        response.status_code = 404;
        response.status_message = "Not Found";
        response.http_version = "HTTP/1.0";
        response.body = "Not Found";
        return response;    
    }

private:
    std::map<std::string, Handler> m_Handlers;
};
    }
}

#endif //BOOST_ASIO_SERVER_ROUTER_H


// http://127.0.0.1/room/1?player_id=1&index=6
