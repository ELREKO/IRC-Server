#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Server.hpp"
#include "Channel.hpp"

class Client
{
private:
	int fd;

	bool logedin;
	bool registered;

	std::string nickname;
	std::string username;
	std::string buffer;
	std::string ipAddress;

	std::vector<std::string> invitedChannel;

public:
	// Basic
	Client();
	Client(std::string nickname, std::string username, int fd);
	Client(Client const &src);
	~Client();
	Client &operator=(Client const &src);

	// Get
	int GetFd();
	bool GetLogedIn();
	bool getRegistered();
	std::string GetNickName();
	std::string GetUserName();
	std::string getBuffer();
	std::string getIpAddress();
	std::string getHostname();
	bool getInvitedUsers(std::string &channelName);

	// Set
	void SetFd(int fd);
	void setLogedin(bool value);
	void setRegistered(bool value);
	void SetNickname(std::string& nickName);
	void SetUsername(std::string& username);
	void setBuffer(std::string received);
	void setIpAddress(std::string ipAddress);

	// Method
	void clearBuffer();
	void AddChannelInvite(std::string &channelName);
	void RemoveChannelInvite(std::string &channelName);
	void printAllName();
};

#endif