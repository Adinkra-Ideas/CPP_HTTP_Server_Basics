/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: euyi <euyi@student.42wolfsburg.de>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 19:03:12 by euyi              #+#    #+#             */
/*   Updated: 2024/07/13 21:49:00 by euyi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "TcpServer.hpp"

int main() {
	http::TcpServer server("127.0.0.1", 8080) ;
	server.startListen() ;

	return 0;
}