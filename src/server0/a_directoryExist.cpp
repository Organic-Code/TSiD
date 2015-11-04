#include "../../include/server0/a_directoryExist.hpp"

void a_directoryExist(Client& client){

	DIR* directory = opendir(client.path.c_str());
	client.packet.clear();
	std::cout << "\t-file asked: " << client.path << std::endl;
	
	if( directory == NULL ){

		client.packet << VoidDirectory;
		client.socket.send( client.packet );
		client.packet.clear();
		tprint();
		std::cout << client.name() << " -> file doesn't exist" << std::endl;
	}
	
	else{
		client.packet << Exist;
		client.socket.send( client.packet );
		client.packet.clear();
		tprint();
		std::cout << client.name() << " -> file exist" << std::endl;
	}
}