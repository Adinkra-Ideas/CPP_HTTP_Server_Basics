/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 19:02:38 by euyi              #+#    #+#             */
/*   Updated: 2023/02/18 19:20:21 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


# include "TcpServer.hpp"

namespace http {
	void log(const std::string &message)
    {
        std::cout << message << std::endl;
    }

	void exitWithError(const std::string &errorMessage)
    {
        log("ERROR: " + errorMessage);
        exit(1);
    }

	TcpServer::TcpServer( void ) {}

	// **************************************************					
	// Instantiates the Socket Address object			*
	// this->_socketAddress with the received			*
	// ip_addr and port params.							*
	// Then it calls this->startServer()				*
	// **************************************************
	TcpServer::TcpServer( const std::string &ip_addr, const int &port ) 
						: _ip_addr(ip_addr), _port(port), _inbound_socket(), _outbound_socket(),
						_socketAddress(), _socketAddressLen(sizeof(_socketAddress)),
						_serverMessage(buildResponse()) {
		_socketAddress.sin_family = AF_INET;
		_socketAddress.sin_port = htons(_port);
		_socketAddress.sin_addr.s_addr = inet_addr(_ip_addr.data()) ;

		if ( _socketAddress.sin_addr.s_addr == static_cast<uint32_t>(-1) )		// meaning that inet_addr() couldn't interpret _ip_addr as a valid IP
			exitWithError("Cannot Interpret " + _ip_addr + " As a Valid IP");

		startServer();
	}

	TcpServer::~TcpServer( void ) {
		close(_inbound_socket);

	}

	// ******************************************************
	// Use socket() API/function to create a new socket		*
	// of AF_INET Domain, stores the FD for this new socket	*
	// in this->_inbound_socket, bind() the FD to socket	*
	// address previously stored in this->_socketAddress,	*
	// to make the socket usable by listen()				*
	// Exits the program on failure							*
	// ******************************************************
	void	TcpServer::startServer() {
		_inbound_socket = socket(AF_INET, SOCK_STREAM, 0);

		if ( _inbound_socket < 0 )
			exitWithError("Cannot create socket");

		if ( bind(_inbound_socket, (sockaddr *)&_socketAddress, _socketAddressLen) < 0 )
			exitWithError("Cannot connect socket to address");
		
		std::ostringstream ss;
		ss << "\n*** SOCKET FD: " << _inbound_socket
			<< " is now initialized to SOCKET ADDRESS " 
			<< inet_ntoa(_socketAddress.sin_addr)
			<< ":" << ntohs(_socketAddress.sin_port) 
			<< " ***\n";
		log(ss.str());
	}

	// ******************************************************
	// start listening for incoming client requests on our 	*
	// socket address. 										*
	// For each incoming client requests on our socket		*
	// address, call acceptConnection() to setup outgoing	*
	// socket address on this->_outbound_socket.			*
	// For each this->_outbound_socket successfully			*
	// created, use this->sendResponse() to push			*
	// our webpage into the stream.							*
	// ******************************************************
	void	TcpServer::startListen( void ) {
		if ( listen(_inbound_socket, MAX_QUEUE) )
			exitWithError("Socket listen failed");

		std::ostringstream ss;
		ss << "\n*** Listening on SOCKET ADDRESS: "
			<< inet_ntoa(_socketAddress.sin_addr)
			<< " PORT: " << ntohs(_socketAddress.sin_port) 
			<< " ***\n\n";

		log(ss.str());

		while (true)
		{
			log("====== Waiting for a new connection ======\n\n\n");
			acceptConnection(_outbound_socket);

			sendResponse();

			close(_outbound_socket);
		}


	}

	// **********************************************************
	// For each incoming client request allowed through the		*
	// queue by listen(), setup new outgoing socket address		*
	// on this->_outbound_socket and test if the new outgoing	*
	// socket address is working.								*
	// Exit the program if not working							*
	// **********************************************************
	void TcpServer::acceptConnection(int &new_socket) {
		new_socket = accept(_inbound_socket, (sockaddr *)&_socketAddress, &_socketAddressLen);

		if (new_socket < 0)
		{
			std::ostringstream ss;
			ss << 
			"Server failed to accept incoming connection from ADDRESS: " 
			<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
			<< ntohs(_socketAddress.sin_port) ;
			exitWithError(ss.str());
		}

		// Now we test if the _outbound_socket created by accept() for this connection is working
		char buffer[BUFFER_SIZE + 1] = {0};
		int bytesReceived = read(_outbound_socket, buffer, BUFFER_SIZE);
			if (bytesReceived < 0)
				exitWithError("Failed to read bytes from client socket connection");
		
		// If it gets to this point, _outbound_socket is working
		// and can be used to send back packet data to client's browser
		std::ostringstream ss;
		ss << "New Outbound FD " << _outbound_socket << " Opened After Receiving Request from client "
			<< inet_ntoa(_socketAddress.sin_addr) << "; PORT: " 
			<< ntohs(_socketAddress.sin_port) << " ------\n\n";
		log(ss.str());
	}

	// **********************************************************
	// Simply fetch the appropriate http header response		*
	// using our packet data(web page) stored in 				*
	// this->_serverMessage and write() it into the socket FD	*
	// of this->_outbound_socket.								*
	// At this point, the clients browser will download and		*
	// display what got sent into this->_outbound_socket.		*
	// **********************************************************
	void	TcpServer::sendResponse() {
		long long bytesSent;

		bytesSent = write(_outbound_socket, _serverMessage.data(), _serverMessage.size());

		if (bytesSent >= 0 &&
			static_cast<long unsigned int>(bytesSent) == _serverMessage.size())
			log("------ Server Response sent to client ------\n\n");
		else
			log("Error sending response to client");
	}


	// simply return a static http response for testing purposes
	std::string	TcpServer::buildResponse()
	{
		std::string htmlFile = "<!DOCTYPE html><html lang=\"en\"><body><h1> HOME </h1><p> Hello from your Server :) </p></body></html>";

		std::ostringstream ss;
		ss << "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: " << htmlFile.size() << "\n\n"
			<< htmlFile;

		return ss.str();
	}

}	// namespace http
