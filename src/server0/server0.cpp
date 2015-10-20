#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
#include <cstring>
#include "../../include/common/SCommand.hpp"
#include "../../include/server0/upload.hpp"
#include "../../include/server0/download.hpp"
#include "../../include/server0/listFiles.hpp"


bool getClient( sf::TcpSocket& socket ){

	unsigned short port;

	std::cout << "Port to use : ";
	std:: cin >> port;

	sf::TcpListener listener;

	if( listener.listen( port ) != sf::Socket::Done )
	{
		return false;
	}

	std::cout << "Listening." << std::endl;

	if( listener.accept( socket ) != sf::Socket::Done )
	{
		return false;
	}

	std::cout << "Client accepted" << std::endl;
	return true;
}

bool connectUser(sf::TcpSocket& client, sf::Packet& cpacket, std::string& client_id){
	
	std::string client_pass, client_real_pass;
	client.receive(cpacket);
	cpacket >> client_id >> client_pass;
	cpacket.clear();

	std::ifstream user_file( ("UsersData/" + client_id).c_str(), std::ios::in );

	if( user_file.fail() ){

		cpacket << BadID;
		client.send( cpacket );
		cpacket.clear();
		return false;

	}//else
	
	std::getline( user_file, client_real_pass );

	if( client_real_pass == client_pass ){

		cpacket << GoodID;
		client.send( cpacket );
		cpacket.clear();
		return true;
	}//else

	cpacket << BadID;
	client.send( cpacket );
	cpacket.clear();
	return false;
}


int main(){

	sf::TcpSocket client;
	if( !getClient( client ) ){

		std::cout << "Didn't get any client" << std::endl;
		return 1;

	}

	sf::Packet cpacket;
	std::string client_id;
	if ( !connectUser( client, cpacket, client_id ) ){

		std::cout << client_id << " tried to connect (IP: " << client.getRemoteAddress() << ")" << std::endl;
		client.disconnect();
		return 2;
	}

	int client_command;
	do{
		client_command = 0;
		cpacket.clear();
		client.receive( cpacket );
		cpacket >> client_command;

		switch( static_cast<char>(client_command) ){

			case Upload :

				std::cout << "Client sent an upload request" << std::endl;
				if( !retrieveData( client, cpacket ) ){
					std::cout << "Failed to retrieve client data" << std::endl;
				}
				else{
					std::cout << "Client data retrieved successfully" << std::endl;
				}
				break;

			case Download :

				std::cout << "Client sent a download request" << std::endl;
				if( !sendData( client, cpacket ) ){
					std::cout << "Failed to send datas to client" << std::endl;
				}
				else{
					std::cout << "Datas sent to client successfully" << std::endl;
				}
				break;

			case Ls :

				std::cout << "Listing files for client" << std::endl;
				if( !listFiles( client, cpacket ) ){
					std::cout << "Failed to send ls result to client" << std::endl;
				}
				else{
					std::cout << "Successfully listed files to client" << std::endl;
				}
				break;

			case Disconnect :

				std::cout << "Client disconnected" << std::endl;
				break;

			default:
				std::cout << "Client sent an invalid command" << std::endl;
				cpacket.clear();
				cpacket << UnknownIssue ;
				client.send( cpacket );
				return 2;

		}
	}while( static_cast<char>(client_command) != Disconnect );
	client.disconnect();
	return 0;
}
