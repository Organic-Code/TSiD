#include "c_retrieveFileList.hpp"

bool retrieveFileList( sf::TcpSocket& server, std::string current_directory ){

	sf::Packet spacket;
	
	spacket << current_directory << Ls;
	server.send( spacket );
	spacket.clear();

	sf::Int32 file;
	unsigned int filename_length;

	server.receive( spacket );
	spacket >> file;
	spacket.clear();

	if( static_cast<char>(file) == ServerFailure ){
		std::cout << "ServerFailure" << std::endl;
		return false;
	}


	std::vector<std::string> directory_array;

	do{
		server.receive( spacket );
		
		directory_array.push_back( std::string("") );
		spacket >> filename_length;

		for( unsigned int i(0) ; i < filename_length ; ++i ){

			spacket >> file;
			directory_array.back() += static_cast<char>( file );
		}
		spacket.clear();

	}while( filename_length != 0 );

	std::sort( directory_array.begin(), directory_array.end() );
	std::cout << std::endl;

	setColors("light blue"); //for folders

	for( unsigned int i(0) ; i < directory_array.size() ; ++i )
		if( directory_array[i][directory_array[i].size()-1] == '/' && directory_array[i][0] != '.' )
			std::cout << directory_array[i] << std::endl;

	setColors("light green"); //for files

	for( unsigned int i(0) ; i < directory_array.size() ; ++i )
		if( directory_array[i][directory_array[i].size()-1] != '/' )
			std::cout << directory_array[i] << std::endl;

	std::cout << std::endl;
	setColors("reset");

	return true;
}