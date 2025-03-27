#include "../INC/Channel.hpp"

// Basic
/** Basic(1) Default constructor
 * @brief
 * 	Initialize every member variables of 'Channel' object
 * 	to default values, when a new object is created.
 * 
 * [ Memo ]
 * 	this->key = 0;
 * 	this->topic = 0;
 */
Channel::Channel(){

	this->name = "";
	this->creationTime = "";
	this->limit = 0;
	this->inviteOnlyMode = 0;

	this->topicName = "";
	this->isTopicRestricted = false;

	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for(int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i],false));
}

/* Copy constructor */
Channel::Channel(Channel const &src){*this = src;}

/* Destructor */
Channel::~Channel(){}


/**
 * @brief Copy assignment operator
 * 
 * @param src copy source
 * @return Channel& 
 */
Channel &Channel::operator=(Channel const &src){
	if (this != &src){

		this->name = src.name;
		this->password = src.password;
		this->creationTime = src.creationTime;
		this->limit = src.limit;
		this->inviteOnlyMode = src.inviteOnlyMode;

		this->topicName = src.topicName;
		this->isTopicRestricted = src.isTopicRestricted;
		

		this->admins = src.admins;
		this->clients = src.clients;
		this->modes = src.modes;
	}
	return *this;
}

int Channel::getClientsNumber(){return this->clients.size() + this->admins.size();}
std::string Channel::getName(){return this->name;}
std::string Channel::getPassword(){return this->password;}
std::string Channel::getChannelUserList(){
	std::string userList;
	for(size_t i = 0; i < admins.size(); i++){
		userList += "@" + admins[i].GetNickName();
		if((i + 1) < admins.size())
			userList += " ";
	}
	if(clients.size())
		userList += " ";
	for(size_t i = 0; i < clients.size(); i++){
		userList += clients[i].GetNickName();
		if((i + 1) < clients.size())
			userList += " ";
	}
	return userList;
}

std::string Channel::getTopicName(){return this->topicName;}
std::string Channel::getTime(){return this->topicUpdatedTime;}
bool Channel::getTopicRestriction() const{return this->isTopicRestricted;}
int Channel::getLimit(){return this->limit;}
int Channel::getInviteOnlyMode(){return this->inviteOnlyMode;}

/**
 * @brief 
 * [Mode Example]
 *	i - invite 
 *  k - set key
 *  l - limit
 *  
 */
std::string Channel::getModes(){
	std::string listMode;

	// 
	for(size_t i = 0; i < modes.size(); i++){
		if(modes[i].first != 'o' && modes[i].second)
			listMode.push_back(modes[i].first);
	}
	if(!listMode.empty())
		listMode.insert(listMode.begin(),'+');
	return listMode;
}

std::string Channel::getChannelCreationTimestamp(){return creationTime;}
bool Channel::getModeStatusAtIndex(size_t index){return modes[index].second;}

/**
 * @brief
 * Finds the Client object corresponding to the 'fd' and returns its address.
 */
Client *Channel::getClient(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

/** 
 * @brief 
 * Finds the Admin object corresponding to the 'fd' and returns its address.
 */
Client *Channel::getAdmin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetFd() == fd)
			return &(*it);
	}
	return NULL;
}

/** 
 * @brief 
 *	Searches for a client (by nickname) in both the clients and admins vectors
 *	and returns the pointer to the matching client.
 */
Client* Channel::findClientByName(std::string name)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetNickName() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetNickName() == name)
			return &(*it);
	}
	return NULL;
}

/** 
 * @brief 
 * Checks if the client with the given nickname is,
 * in either the clients or admins list and returns "true" if found.
 */
bool Channel::clientInChannel(std::string &nick){
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].GetNickName() == nick)
			return true;
	}
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].GetNickName() == nick)
			return true;
	}
	return false;
}
void Channel::setName(std::string name){this->name = name;}

/**
 * @brief 
 * 	1. Check the current time and save it 'long int' type.
 * 	2. Convert currentime variable's type from "long int" to "std::string" byusing std::ostringstream.
 * 	3. Save "string type" to [v] creationTime.
 */
void Channel::setChannelCreationTimestamp(){
	std::time_t currentTime = std::time(NULL);	// Check time & Save
	std::ostringstream oss;
	oss << currentTime;							// Convert type from "long int" to "String"
	this->creationTime = std::string(oss.str());
}


void Channel::setTime(std::string time){this->topicUpdatedTime = time;}
void Channel::setTopicName(std::string topicName){this->topicName = topicName;}
void Channel::setLimit(int limit){this->limit = limit;}
void Channel::setPassword(std::string password){this->password = password;}
void Channel::setTopicRestriction(bool value){this->isTopicRestricted = value;}
void Channel::setInviteOnlyMode(int inviteOnlyMode){this->inviteOnlyMode = inviteOnlyMode;}
void Channel::setModeStatusAtIndex(size_t index, bool mode){modes[index].second = mode;}
void Channel::addClient(Client newClient){clients.push_back(newClient);}
void Channel::addAdmin(Client newClient){admins.push_back(newClient);}

void Channel::removeClient(int fd){
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (it->GetFd() == fd)
			{clients.erase(it); break;}
	}
}

void Channel::removeAdmin(int fd){
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it){
		if (it->GetFd() == fd)
			{admins.erase(it); break;}
	}
}

bool Channel::upgradeClientToAdmin(std::string& nick){
	size_t i = 0;
	for(; i < clients.size(); i++){
		if(clients[i].GetNickName() == nick)
			break;
	}
	if(i < clients.size()){
		admins.push_back(clients[i]);
		clients.erase(i + clients.begin());
		return true;
	}
	return false;
}

bool Channel::downgradeAdminToClient(std::string& nick){
	size_t i = 0;
	for(; i < admins.size(); i++){
		if(admins[i].GetNickName() == nick)
			break;
	}
	if(i < admins.size()){
		clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;

}

/** Method(7)
 * @brief 
 * 1. Send message to all admins one by one with for loop.
 * 2. Send message to all cliens one by one with for loop.
*/
void Channel::broadcastMessage(std::string message)
{
	for(size_t i = 0; i < admins.size(); i++)
		if(send(admins[i].GetFd(), message.c_str(), message.size(),0) == -1)
			std::cerr << "send() function faild" << std::endl;
	for(size_t i = 0; i < clients.size(); i++)
		if(send(clients[i].GetFd(), message.c_str(), message.size(),0) == -1)
			std::cerr << "send() function faild" << std::endl;
}

/**
 * @brief
 * Send the message to everyone except the sender 
 */
void Channel::broadcastMessage(std::string message, int fd)
{
	for(size_t i = 0; i < admins.size(); i++){
		if(admins[i].GetFd() != fd)
			if(send(admins[i].GetFd(), message.c_str(), message.size(),0) == -1)
				std::cerr << "send() function faild" << std::endl;
	}
	for(size_t i = 0; i < clients.size(); i++){
		if(clients[i].GetFd() != fd)
			if(send(clients[i].GetFd(), message.c_str(), message.size(),0) == -1)
				std::cerr << "send() function faild" << std::endl;
	}
}




///////////////////// Print for Debugging, logs //////////////////
void Channel::PrintClientsServer(std::string color) {
	std::cout	<< "   Actual Users in Channel |" << this->getName() << "|" << std::endl;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        std::cout 	<< color
					<< "     --> FD |"  << it->GetFd() << "| -- "  
					<< "Nickname |" << it->GetNickName() << "| -- "
					<< "Username |" << it->GetUserName() << "|"
					<< RESET << std::endl; 
    }
}

void Channel::PrintAdminsServer(std::string color) {
	std::cout	<< "   Actual Admins in Channel |" << this->getName() << "|" << std::endl;
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
        std::cout 	<< color
					<< "     --> FD |"  << it->GetFd() << "| -- "  
					<< "Nickname |" << it->GetNickName() << "| -- "
					<< "Username |" << it->GetUserName() << "|"
					<< RESET << std::endl; 
    }
}

std::vector<Client> Channel::getAllAdmins() {
	std::vector<Client> ret_cli;
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it) {
		ret_cli.push_back(*it);
    }
	return ret_cli;
}

std::vector<Client> Channel::getAllClients(){
	std::vector<Client> ret_cli;
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
		ret_cli.push_back(*it);
    }
	return ret_cli;
}

bool Channel::getUserInChannelAdmin(std::string username)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it) 
	{
		if (username == it->GetNickName())
			{return true;}
	}
	return false;
}
bool Channel::getUserInChannelClient(std::string username)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) 
	{
		if (username == it->GetNickName())
			{return true;}
	}
	return false;
}