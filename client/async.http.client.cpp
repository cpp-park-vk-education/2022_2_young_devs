
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

#include "response.h"
#include "request.h"

using boost::asio::ip::tcp;

using namespace std::string_literals;


class Client
{
public:
    Client(boost::asio::io_context& io_context,
           const std::string& server, const std::string& port, const std::string& path)
            : resolver_(io_context),
              socket_(io_context)
    {
        request_.method = "GET";
        request_.path = path;
        request_.http_version = "HTTP/1.0";
        request_.host = server;


        // Start an asynchronous resolve to translate the server and service names
        // into a list of endpoints.
        resolver_.async_resolve(server, port,
                                boost::bind(&Client::handle_resolve, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::results));
    }

private:
    void handle_resolve(const boost::system::error_code& err,
                        const tcp::resolver::results_type& endpoints)
    {
        if (!err)
        {
            // Attempt a connection to each endpoint in the list until we
            // successfully establish a connection.
            boost::asio::async_connect(socket_, endpoints,
                                       boost::bind(&Client::handle_connect, this,
                                                   boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_connect(const boost::system::error_code& err)
    {
        if (!err)
        {
            std::string buffer = Request2String(request_);

            std::cout << "\n\nREQUEST\n\n" << buffer << "\n\nEND REQUEST\n\n";

            // The connection was successful. Send the request.
            boost::asio::async_write(socket_, boost::asio::buffer(buffer.data(), buffer.size()),
                                     boost::bind(&Client::handle_write_request, this,
                                                 boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_write_request(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Read the response status line. The response_ streambuf will
            // automatically grow to accommodate the entire line. The growth may be
            // limited by passing a maximum size to the streambuf constructor.
            boost::asio::async_read_until(socket_, response_buf_, "\r\n",
                                          boost::bind(&Client::handle_read_status_line, this,
                                                      boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err.message() << "\n";
        }
    }

    void handle_read_status_line(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Check that response is OK.
            std::istream response_stream(&response_buf_);
            response_stream >> response_.http_version;
            response_stream >> response_.status_code;

            std::cout << "\n\nRESPONSE_STATUS " << response_ << "\n\n END RESPONSE_STATUS\n\n";

            std::getline(response_stream, response_.status_message);

            if (response_.status_code != 200)
            {
                std::cout << "Response returned with status code ";
                std::cout << response_.status_code << "\n";
                return;
            }

            // Read the response headers, which are terminated by a blank line.
            boost::asio::async_read_until(socket_, response_buf_, "\r\n\r\n",
                                          boost::bind(&Client::handle_read_headers, this,
                                                      boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_headers(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Process the response headers.
            std::istream response_stream(&response_buf_);

            std::string header;
            std::cout << "\n\nHEADER\n\n\n";
            while (std::getline(response_stream, header) && header != "\r")
            {
                response_.headers.push_back(header);
                std::cout << "HEADER_ITEM = " << header << " "; 
            }

            std::cout << "\n\nEND HEADER\n\n"; 

            // Start reading remaining data until EOF.
            boost::asio::async_read(socket_, response_buf_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&Client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        }
        else
        {
            std::cout << "Error: " << err << "\n";
        }
    }

    void handle_read_content(const boost::system::error_code& err)
    {
        if (!err)
        {
            // Continue reading remaining data until EOF.
            boost::asio::async_read(socket_, response_buf_,
                                    boost::asio::transfer_at_least(1),
                                    boost::bind(&Client::handle_read_content, this,
                                                boost::asio::placeholders::error));
        }
        else if (err == boost::asio::error::eof)
        {
            std::istream response_stream(&response_buf_);
            response_.body = std::string(std::istreambuf_iterator<char>(response_stream), std::istreambuf_iterator<char>());
            // std::cout << response_;
            std::cout << "\n\nRESPONSE\n\n" << response_ << "\n\nEND RESPONSE\n\n";
        }
        else if (err != boost::asio::error::eof)
        {
            std::cout << "Error: " << err << "\n";
        }
    }

private:
    tcp::resolver resolver_;
    tcp::socket socket_;

    boost::asio::streambuf response_buf_;

    Request request_;
    Response response_;
};

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage: async_client <server> <port>\n";
            std::cout << "Example:\n";
            std::cout << "  async_client www.boost.org /LICENSE_1_0.txt\n";
            return 1;
        }
        
        size_t value;
        size_t player_id = 0;
        char command;
        std::cout << "Enter <command> <index>\n";
        while (std::cin >> command >> value)
        {
            switch(command)
            {
                case 's':
                {
                    boost::asio::io_context io_context;
                    std::stringstream args;
                    args << "/bot/room/0?command=" << command << "&player_id=" << player_id << "&value=" << value;
                    Client c(io_context, argv[1], argv[2], args.str());
                    io_context.run();
                    // T_StepTask task(room, player_1, value);
                    // boost::asio::post(pool, task);
                    break;
                }
                case 'r':
                {
                    boost::asio::io_context io_context;
                    std::stringstream args;
                    args << "/bot/room/0?command=" << command << "&player_id=" << player_id << "&value=" << value;
                    Client c(io_context, argv[1], argv[2], args.str());
                    io_context.run();
                    break;
                }
                default:
                {
                    std::cout << "Comamnd unknown\n";
                }
            }
        }

    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}