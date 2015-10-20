#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
#include "../../include/client0/userInputInterpret.hpp"
#include "../../include/client0/setStdcinEcho.hpp"

bool sconnect( sf::TcpSocket& socket, std::string& user_id ) {								//Connect the client to the server

	unsigned short remote_port;
	std::string remote_address, user_pass;

	std::cout << "User ID : ";
	std::cin >> user_id;

	std::cout << "User passwd : ";
	setStdcinEcho( false );
	std::cin.ignore();
	std::getline(std::cin, user_pass);
	setStdcinEcho( true );


	std::cout << std::endl << "Remote address : ";
	std::cin >> remote_address;
	std::cout << "Remote port : ";
	std::cin >> remote_port;
	std::cout << "Connecting to the remote @ " << remote_address << ":" << remote_port << std::endl;

	if( socket.connect( remote_address, remote_port ) != sf::Socket::Done ){
		std::cout << "Connection wasn't successful" << std::endl;
		return false;
	}

	std::cout << "Connected to server" << std::endl;
	return true;
}

int main() {

	sf::TcpSocket socket;
	std::string user_id;

	if( !sconnect( socket, user_id ) )
		return 1;

	userInputInterpret( socket, user_id );

	sf::Packet bye;
	bye << Disconnect;
	socket.send( bye );

	socket.disconnect();

	return EXIT_SUCCESS;
}
