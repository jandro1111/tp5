//
// client.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include<fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;


int main(int argc, char* argv[])//https://github.com/jandro1111/tp5 usar esto de ejemplo
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Usage: cliente <host/path/filename>" << std::endl;
            return 1;
        }
        //despues sortear el argc
        std::string host = "";
    int port = 80;
    std::string message = "GET ";
    bool barra = false;
    for (int i = 0; argv[1][i] != NULL; ++i) {
        if ((argv[1][i] == '/')&& barra==false ) {
            barra = true;
        }
        else {
            if (!barra) {
                host += argv[1][i];
            }
            else {
                message += argv[1][i];
            }
        }
    }
    message += " HTTP/1.1";
    message += 13;
    message += 10;
    message += "Host: ";
    message += host;
    message += 13;
    message += 10;
    std::cout << message << std::endl;
        //
        boost::asio::io_service ios;
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
        boost::asio::ip::tcp::socket socket(ios);
        socket.connect(endpoint);
        boost::array<char, 128> buf;
        std::copy(message.begin(), message.end(), buf.begin());
        boost::system::error_code error;
        socket.write_some(boost::asio::buffer(buf, message.size()), error);

        std::ofstream prueba;//abro/creo si no esta/ el archivo para poner lo que reciba del server
        prueba.open("prueba.txt", std::ios::trunc);//borro lo que habia antes

        for (;;)
        {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
            prueba.write(buf.data(), len);//guardo en el archivo
        }
        prueba.close();
        socket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    
    
    return 0;
}
