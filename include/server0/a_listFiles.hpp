#ifndef A_LISTFILES_HPP_INCLUDED
#define A_LISTFILES_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <dirent.h>
#include <iostream>
#include "../common/SCommand.hpp"
#include "tprint.hpp"
#include "../common/isFolder.hpp"
#include "Client.hpp"

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#else
	#include <sys/types.h>
#endif

/**
 * @brief Answer to the client command 'ls'
 * @details If the path is /Public send the the Private folder and the list of the files in path,
 * else just send a list of the files in path
 * 
 * @param client The Client object wich contain the path to list files
 * @return 0 for fail, 1 for success
 */
bool a_listFiles( Client& client );

#endif
