#include "s_a_listFiles.hpp"

bool a_listFiles(Client& client){
	
	//checking allowance and sending what's up about this
	
	DIR* directory = opendir(client.path.c_str());
	struct dirent* redfile = NULL;
	client.packet.clear();

	std::cout << "\t-Directory: " << client.path << std::endl;

	if( directory == NULL ){
		client.packet << VoidDirectory;
		client.socket.send( client.packet );
		tprint();
		std::cout << client.name() << " -> Could not open the directory" << std::endl;
		return false;
	}//else
	
	client.packet << ServerReady;
	client.socket.send( client.packet );
	client.packet.clear();
	tprint();
	std::cout << client.name() << " -> Server ready to list" << std::endl;

	if(client.path == "./Public/"){
		client.packet << 8 
		<< static_cast<sf::Int32>('P')
		<< static_cast<sf::Int32>('r')
		<< static_cast<sf::Int32>('i')
		<< static_cast<sf::Int32>('v')
		<< static_cast<sf::Int32>('a')
		<< static_cast<sf::Int32>('t')
		<< static_cast<sf::Int32>('e')
		<< static_cast<sf::Int32>('/');
		client.socket.send( client.packet );
		client.packet.clear();
	}

	while( (redfile = readdir( directory )) != NULL ){

		std::string tmp( redfile->d_name );
    	std::string info_path = "./FilesData" + client.path.substr(1, std::string::npos); //add "./FilesData" at the begening
    	info_path += tmp; //add the filename at the end
    	info_path = info_path.insert(info_path.find_last_of("/") + 1,"."); //insert '.' before the filename
    	std::string info_date = " ";
    	std::string info_user = " ";

    	if( isFolder(client.path + tmp) ){
			tmp += "/";
		}

		if (tmp != "../" && tmp != "./"){
			
			std::ifstream file (info_path.c_str(), std::ios::binary | std::ios::in );

		    if( file.fail() ){
		        
		        file.close();
		        std::cout << "\t-File: " << info_path << std::endl;
		        std::cout << "\t-";
		        setColors("light red");
		        std::cout << "Error reading the file" << std::endl;
		        setColors("reset");
		    }
		    else{

		    	bool filename_printed(false);
		    	if(std::getline(file, info_date) != file){

		    		std::cout << "\t-File: " << info_path << std::endl;
		    		filename_printed = true;
			        std::cout << "\t-";
			        setColors("light yellow");
			        std::cout << "Error reading the date" << std::endl;
			        setColors("reset");
		    	}

		    	if(std::getline(file, info_user) != file){
		    		
		    		if(!filename_printed)
		    			std::cout << "\t-File: " << info_path << std::endl;

			        std::cout << "\t-";
			        setColors("light yellow");
			        std::cout << "Error reading the username" << std::endl;
			        setColors("reset");
		    	}
		    	file.close();
		    }
		}

	    tmp = tmp + "|" + info_date + "|" + info_user;
		
		client.packet << static_cast<unsigned int>( tmp.length() );

		for( unsigned short i(0) ; i < tmp.length() ; ++i ){
			client.packet << static_cast<sf::Int32>( tmp[i] );
		}

		client.socket.send( client.packet );
		client.packet.clear();

	}

	client.packet << 0 << EndOfStream;
	client.socket.send( client.packet );
	client.packet.clear(); 
	tprint();
	std::cout << client.name() << " -> file listed" << std::endl;

	return true;
}