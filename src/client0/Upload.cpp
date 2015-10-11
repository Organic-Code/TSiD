#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>

#define NB_BYTE_PER_PACKET 8192


unsigned int getFileLength( std::string const& filename ) {						//Retrieving file size in bytes

	std::ifstream file( filename.c_str(), std::ios::binary | std::ios::in );
	if( !file.fail() ){
		file.seekg( 0, std::ios::end );
		return file.tellg();
	}
	return 0;
}


bool startUpload( std::ifstream& infile, unsigned int& file_size, sf::TcpSocket& server ) {		//Starts an upload (opening file and telling the server its name and size)
													//Also retrieves server's answer (upload accepted or denied)
	std::string filename;
	
	std::cout << "File name : ";
	std::cin >> filename;

	file_size = getFileLength( filename ) ;
	infile.open(filename.c_str(), std::ios::binary | std::ios::in);

	if( file_size == 0 || infile.fail() ) {
		std::cout << "There was a problem reading the file" << std::endl;
		return false;
	}

	sf::Packet packet;

	packet << Upload << filename << file_size << NB_BYTE_PER_PACKET;
	server.send(packet);
	packet.clear();

	int server_state;

	server.receive(packet);
	if( packet.getDataSize() > sizeof( int ) || !(packet >> server_state) ){
		std::cout << "There was an error retrieving server state" << std::endl;
		return false;
	}
	return interpretServerAns( static_cast<char>(server_state) );
}


bool sendData( sf::TcpSocket& server ){									// Sends a file to the server

	std::ifstream input_file;
	unsigned int file_size;

	if( !startUpload( input_file, file_size, server) ){				//Preparing to upload
		std::cout << "Could not send the file" << std::endl;
		return false;
	}

	unsigned int loop_number=file_size/NB_BYTE_PER_PACKET;
	char input_data_array[NB_BYTE_PER_PACKET];
	sf::Packet spacket;
	spacket.clear();

	std::cout << "Upload is starting" << std::endl << "\e[?25l";
	for( unsigned int i(0) ; i<loop_number ; ++i ){					//Reading an sending the file

		input_file.read( input_data_array, NB_BYTE_PER_PACKET);
		for( unsigned int j(0) ; j<NB_BYTE_PER_PACKET ; ++j)
			spacket << static_cast<sf::Int8>(input_data_array[j]);

		if( server.send(spacket) == sf::Socket::Disconnected ){

			std::cout << "Lost connection with server !" << std::endl << "\e[?25h";
			return false;
		}

		spacket.clear();

		std::cout << "\r[" << static_cast<short>(100*i/loop_number) << "%] - File being transfered ( " << i << "/" << loop_number+(file_size>0) << " )";
	}

	file_size -= loop_number * NB_BYTE_PER_PACKET;
	if( file_size > 0){								//There is some more to be transferred

		char* file_tail = new char[file_size];
		input_file.read( file_tail, file_size);
		for( unsigned int j(0) ; j< file_size ; ++j)
			spacket << file_tail[j];

		if( server.send(spacket) == sf::Socket::Disconnected ){
			std::cout << "Too bad. You almost done it but you were disconnected by server :(" << std::endl;
			delete file_tail;
			return false;
		}

		std::cout << "\r[100%] - File being transfered ( " << loop_number+1 << "/" << loop_number+1 << " )";

		delete file_tail;

	}

	std::cout << std::endl << "Transfer terminated successfully" << std::endl << "\e[?25h";

	return true;
}
