#include "Server.h"
#include "../Config.h"

namespace Ledlib {

Server::Server(){}
Server::~Server(){}

bool Server::Init(){
	if(++initCounter > 1) return false;
	return true;
}

}
