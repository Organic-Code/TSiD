#include "../../include/client0/serverAnsInterpret.hpp"

bool interpretServerAns( signed char sanswer){								//Sends message to console telling the problem // success
	
	switch( sanswer ){

	case NotAuthorized :
		std::cout << "Message from server : action prohibited" << std::endl;
		return false;

	case TooBig:
		std::cout << "Message from server : file too heavy" << std::endl;
		return false;
	
	case AlreadyExist:
		std::cout << "Message from server : this file already exists" << std::endl;
		return false;

	case VoidFileName:
		std::cout << "Message from server : void file name" << std::endl;
		return false;

	case FileDoesNotExist:
		std::cout << "Message from server : the file does not exist" << std::endl;
		return false;

	case ServerFailure:
		std::cout << "Message from server : an error has occured" << std::endl;
		return false;
	
	case UnknownIssue:
		std::cout << "Message from server : what happened ?!?" << std::endl;
		return false;

	case BadID:
		std::cout << "Message from server : bad password or user name" << std::endl;
		return false;

	case ServerReady:					//ServerReady or GoodID
		std::cout << "Server ready" << std::endl;
		return true;
	
	default:
		std::cout << "Unexpected answer from server" << std::endl;
		return false;
	}
}
