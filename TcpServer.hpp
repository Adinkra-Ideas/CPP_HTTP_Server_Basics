/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TcpServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 19:02:59 by euyi              #+#    #+#             */
/*   Updated: 2023/02/18 19:22:48 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include <iostream>
# include <stdexcept>
# include <sstream>

# include <unistd.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <arpa/inet.h>		// definition of struct	sockaddr_in

# define BUFFER_SIZE 30720
# define MAX_QUEUE 20

namespace http {

	void log(const std::string &message);

	void exitWithError(const std::string &errorMessage);

	class TcpServer {
	public:
		TcpServer( const std::string &ip_addr, const int &port );
		~TcpServer( void );

		void	startListen( void );

	private:
		TcpServer( void );

		std::string	_ip_addr;					// For storing un-validated preferred IP to establish server upon
		int			_port;						// For storing un-validated preferred PORT to establish server upon
		int			_inbound_socket;			// For storing FD for the server inbound socket address
		int			_outbound_socket;			// For storing FD for each outbound socket address of clients

		struct	sockaddr_in		_socketAddress;		// object for storing IP and Port in network byte order
		unsigned int			_socketAddressLen;	// For storing the sizeof(_socketAddress)
		std::string				_serverMessage;		// string for storing the full HTTP Response

		void	startServer( void );
		void	acceptConnection( int &new_socket );

		void	sendResponse();

		std::string	buildResponse();

	};

} // namespace http

#endif
