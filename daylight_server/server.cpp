#include "server.h"

#include <iostream>
#include <string>
#include <boost\bind.hpp>

using boost::asio::ip::tcp;
std::string make_response_string(std::string aux);


AsyncDaytimeServer::AsyncDaytimeServer(boost::asio::io_context& io_context)
	:	context_(io_context),
		acceptor_(io_context, tcp::endpoint(tcp::v4(), 80)),//ese de ahi es el numero del puerto
		socket_(io_context)
{
}

AsyncDaytimeServer::~AsyncDaytimeServer()
{
}

void AsyncDaytimeServer::start()
{
	if (socket_.is_open())
	{
		socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
		socket_.close();
	}
	start_waiting_connection();
}

void AsyncDaytimeServer::start_waiting_connection()
{
	std::cout << "start_waiting_connection()" << std::endl;
	if (socket_.is_open())
	{
		std::cout << "Error: Can't accept new connection from an open socket" << std::endl;
		return;
	}
	acceptor_.async_accept(			//solo recibe socket que va a administrar la nueva conexion y el callback
		socket_, 
		boost::bind(
			&AsyncDaytimeServer::connection_received_cb, 
			this, 
			boost::asio::placeholders::error
			)
		);
}

void AsyncDaytimeServer::start_answering(std::string aux)
{
	std::cout << "start_answering()" << std::endl;
	msg = make_response_string(aux);
	boost::asio::async_write(//make_daytime_string()
		socket_,
		boost::asio::buffer(msg),
		boost::bind(
			&AsyncDaytimeServer::response_sent_cb,
			this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred
		)
	);
	socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
	socket_.close();
}


void AsyncDaytimeServer::connection_received_cb(const boost::system::error_code& error)//aca hacemos lo de buscar e interpretar el mensaje
{
	std::ofstream handler;//abro/creo si no esta/ el archivo para poner lo que reciba del server
	handler.open("handler.txt", std::ios::trunc);//borro lo que habia antes
	for (;;)//recibo lo del cliente y lo interpreto
	{
		boost::array<char, 128> buf;
		boost::system::error_code error;

		size_t len = socket_.read_some(boost::asio::buffer(buf), error);

		if (error == boost::asio::error::eof)
			break; // Connection closed cleanly by peer.
		else if (error)
			throw boost::system::system_error(error); // Some other error.

		std::cout.write(buf.data(), len);
		handler.write(buf.data(), len);//guardo en el archivo que se llama handler, para interpretar mas adelante
		handler.close();
		break;
	}
	using namespace std;
	string nombreArchivo = "handler.txt";
	ifstream archivo(nombreArchivo.c_str());
	string linea;
	string aux = "";
	// Obtener línea de archivo, y almacenar contenido en "linea"
	while (getline(archivo, linea)) {
		// Lo vamos imprimiendo
		bool fin = false;
		bool ini = false;
		for (int i = 0; fin == false; ++i) {//aislo el archivo a buscar
			if (linea[i] == '/'||ini==true) {//encontre el inicio
				if (ini == false) {
				}
				else {
					aux += linea[i];
				}
				ini = true;
			}
			if (ini == true) {
				if (linea[i] == ' ') {//encontre el final
					fin = true;
				}
			}
		}
		if (fin == true) {
			break;
		}
	}
	//en aux tengo el path a buscar
	

	std::cout << "connection_received_cb()" << std::endl;
	if (!error) {
		start_answering(aux);
		start_waiting_connection();
	}
	else {
		std::cout << error.message() << std::endl;
	}
}

void AsyncDaytimeServer::response_sent_cb(const boost::system::error_code& error,
								size_t bytes_sent)
{
	std::cout << "response_sent_cb()" << std::endl;
	if (!error) {
		std::cout << "Response sent. " << bytes_sent << " bytes." << std::endl;
	}

}


std::string make_response_string(std::string aux)//aca armamos el mensaje en aux tengo el path a buscar
{
#pragma warning(disable : 4996)
	std::cout << aux << std::endl;
	using namespace std; // For time_t, time and ctime;
	time_t now = time(0);
	return ctime(&now);
}

