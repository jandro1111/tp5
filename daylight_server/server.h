#pragma once

/*
	Asynchronous TCP Daytime Server
	Based on:
	https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/tutorial/tutdaytime3/src.html

	Example use:
		boost::asio::io_context io_context;
		AsyncDaytimeServer s(io_context);
		s.start();
		io_context.run();

	Tested with client from:
	https://www.boost.org/doc/libs/1_72_0/doc/html/boost_asio/tutorial/tutdaytime1/src.html

*/


#include <iostream>
#include<fstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include<curl/curl.h>
#include<string>
using boost::asio::ip::tcp;
class AsyncDaytimeServer
{
public:
	AsyncDaytimeServer(boost::asio::io_context& context);
	~AsyncDaytimeServer();

	void start();

private:
	void start_waiting_connection();
	void start_answering(std::string);
	void connection_received_cb(const boost::system::error_code& error);
	void response_sent_cb(const boost::system::error_code& error, size_t bytes_sent);

	std::string msg;	

	boost::asio::io_context& context_;
	boost::asio::ip::tcp::socket socket_;
	boost::asio::ip::tcp::acceptor acceptor_;
};
