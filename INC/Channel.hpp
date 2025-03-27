#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Client;
class Channel
{
private:
	// Channel-related variables
	std::string name;							// 채널 이름
	std::string password;						// 채널 비밀번호
	std::string creationTime;					// 채널 생성 시간
	int limit;									// 채널 최대 사용자 수
	int inviteOnlyMode;							// 초대 전용 모드 (1: ON, 0: OFF)

	// Topic-related variables
	std::string topicName;						// 현재 설정된 토픽(주제)
	std::string topicUpdatedTime;				// 토픽 마지막 변경 시간
	bool isTopicRestricted;						// 토픽 변경 제한 여부 (true: 관리자만 변경 가능)

	// User Managemet variables
	// Why use 'vector' structure?
	// 		= Easy to manage.
	// 		1. Dynamic Resizing: [f] push_back
	// 		2. Modify in the middle: erase(), insert()
	//		3. Allocate memory contiguously like arrays, so it use CPU cache efficiently.
	std::vector<Client> admins;					// 채널 관리자 목록
	std::vector<Client> clients;				// 채널 참여자 목록
	std::vector<std::pair<char, bool> > modes;	// 채널 모드 설정 목록

	// Why do we need below variables -> Comment those out.
	// int key;
	// int topic;

public:
	// Basic: 4ea
	Channel();	
	Channel(Channel const &src);
	~Channel();
	Channel &operator=(Channel const &src);





	// Get: 16ea
	// <1. Channel-related functions>: 7ea
	int getClientsNumber();						// INVITE	JOIN	KICK			PART	QUIT
	std::string getName();						// 			JOIN			MODE	PART
	std::string getPassword();					//			JOIN			MODE
	std::string getChannelUserList();			//			JOIN
	std::string getTopicName();					//			JOIN									TOPIC
	std::string getTime();						//													TOPIC
	bool getTopicRestriction() const;			// 													TOPIC	

	// <2. Setting & Status of Channel>: 5ea
	int getLimit();								// INVITE	JOIN
	int getInviteOnlyMode();					// INVITE	JOIN
	std::string getModes();						//							MODE
	std::string getChannelCreationTimestamp();	//							MODE
	bool getModeStatusAtIndex(size_t index);	// 							MODE

	// <3. Client-related functions>: 4ea
	Client *getClient(int fd);					// INVITE			KICK	MODE	PART	QUIT	TOPIC
	Client *getAdmin(int fd);					// INVITE			KICK	MODE	PART	QUIT	TOPIC
	Client *findClientByName(std::string name);	// INVITE	JOIN	KICK			PART					PRIVMSG		AUTHENTI
	bool clientInChannel(std::string &nick);	// 							MODE
	bool getUserInChannelAdmin(std::string username);
	bool getUserInChannelClient(std::string username);
	// <4. ETC>: 2ea
	// int GetKey();	-> NOT USED
	// int GetTopic();	-> NOT USED	// Server class also has same name function.





	// Set: 9ea
	// <1. #JOIN-related functions>: 2ea
	void setName(std::string name);
	void setChannelCreationTimestamp();

	// <2. #TOPIC-related functions>: 2ea
	void setTime(std::string time);
	void setTopicName(std::string topicName);
	
	// <3. #MODE-related functions>: 5ea
	void setLimit(int limit);
	void setPassword(std::string password);
	void setTopicRestriction(bool value);
	void setInviteOnlyMode(int inviteOnlyMode);
	void setModeStatusAtIndex(size_t index, bool mode);

	// <4. ETC>
	// void Setkey(int key);		-> NOT USED
	// void SetTopic(int topic);	-> NOT USED





	// Method: 8ea
	// <1. User(Admin & Client) Management>: 6ea
	void addClient(Client newClient);
	void addAdmin(Client newClient);
	void removeClient(int fd);
	void removeAdmin(int fd);
	bool upgradeClientToAdmin(std::string& nick);
	bool downgradeAdminToClient(std::string& nick);

	// <2. SendMessageToAll>: 2ea
	void broadcastMessage(std::string message);
	void broadcastMessage(std::string message, int fd);

	// Debugging 
	void PrintClientsServer(std::string color);
	void PrintAdminsServer(std::string color);
	void PrintClientsClient(std::string color, std::string nickname);
	void PrintAdminsClient(std::string color, std::string nickname);


	std::vector<Client> getAllAdmins();
	std::vector<Client> getAllClients();
};

#endif
