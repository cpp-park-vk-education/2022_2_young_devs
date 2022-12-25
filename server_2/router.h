#pragma once


namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>


inline std::string getPath(std::string const &pathParams)
{
    std::string path = pathParams;
    if (pathParams.find("bot/room") != std::string::npos || pathParams.find("multiplayer/room") != std::string::npos)
    {
        path = std::string(pathParams.begin(), pathParams.begin() + pathParams.rfind('/'));
    }
    else
    {
        if (pathParams.find('?') != std::string::npos)
        {
            path = std::string(pathParams.begin(), pathParams.begin() + pathParams.find('?'));
        }
    }
    return path;
}

namespace server3 {
    template<typename Handler>
    class Router {
    public:
        void addHandler(std::string method, Handler handler) {
            m_Handlers.emplace(method, handler);
        }

        http::response<http::string_body> processRoute(http::request<http::string_body> const &request) 
        {
            std::string path = getPath(std::string(request.target()));
            std::cout << "PATH = " << path << std::endl;

            auto handler_it = m_Handlers.find(path);
            if (handler_it != m_Handlers.end()) {
                return (handler_it->second)(request);
            }

            http::response<http::string_body> res{http::status::bad_request, request.version()};

            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/json");
            res.result(http::status::not_found);
            res.body() = " Not found";  
            return res;
        }

    private:
        std::map<std::string, Handler> m_Handlers;
    };
}
