#include "../INC/Client.hpp"

Client::Client()
{
	this->fd = -1;

	this->logedin = false;
	this->registered = false;

	this->nickname = "";
	this->username = "";
	this->buffer = "";
	this->ipAddress = "";
}

Client::Client(std::string nickname, std::string username, int fd) :fd(fd), nickname(nickname), username(username){}
Client::Client(Client const &src){*this = src;}
Client::~Client(){}
Client &Client::operator=(Client const &src){
	if (this != &src){
		this->fd = src.fd;

		this->logedin = src.logedin;
		this->registered = src.registered;

		this->nickname = src.nickname;
		this->username = src.username;
		this->buffer = src.buffer;
		this->ipAddress = src.ipAddress;

		this->invitedChannel = src.invitedChannel;
	}
	return *this;
}

int Client::GetFd(){return this->fd;}
bool Client::GetLogedIn(){return this->logedin;}
bool Client::getRegistered(){return registered;}
std::string Client::GetNickName(){return this->nickname;}
std::string Client::GetUserName(){return this->username;}
std::string Client::getBuffer(){return buffer;}
std::string Client::getIpAddress(){return ipAddress;}

std::string Client::getHostname(){
	std::string hostname = this->GetNickName() + "!" + this->GetUserName();
	return hostname;
}

bool Client::getInvitedUsers(std::string &ChannelName){
	for (size_t i = 0; i < this->invitedChannel.size(); i++){
		if (this->invitedChannel[i] == ChannelName)
			return true;
	}
	return false;
}

void Client::SetFd(int fd){this->fd = fd;}
void Client::setLogedin(bool value){this->logedin = value;}
void Client::setRegistered(bool value){registered = value;}
void Client::SetNickname(std::string& nickName){this->nickname = nickName;}
void Client::SetUsername(std::string& username){this->username = username;}
void Client::setBuffer(std::string received){buffer += received;}
void Client::setIpAddress(std::string ipAddress){this->ipAddress = ipAddress;}
void Client::clearBuffer(){buffer.clear();}

 /**
  * @brief 
  * Add a channel to the list of variable(= invitedChannel).
  */
void Client::AddChannelInvite(std::string &channelName){
	invitedChannel.push_back(channelName);
}

 /**
  * @brief
  * Remove a channel to the list of variable(= invitedChannel).
  */
void Client::RemoveChannelInvite(std::string &channelName){
	for (size_t i = 0; i < this->invitedChannel.size(); i++){
		if (this->invitedChannel[i] == channelName)
			{this->invitedChannel.erase(this->invitedChannel.begin() + i); return;}
	}
}

/** 
 * @brief 
 * Check the status of user's name.
 */
void Client::printAllName(){
	std::cout << RED << "nickname: " << WHI << this->GetNickName() << std::endl;
	std::cout << RED << "username: " << WHI << this->GetUserName() << std::endl;
	std::cout << RED << "Hostname: " << WHI << this->getHostname() << std::endl;
}