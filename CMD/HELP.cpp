#include "../INC/Server.hpp"
/**
 * @brief Print a all Command what to client
 * 
 * @param fd filediscripter from client
 */
void Server::HELP(int fd)
{
	_sendResponse(	MSG_MSG("HELP COMMANDS: ") + \
					MSG_MSG("-->PASS <Password>") + \
					MSG_MSG("   Use for register to server") + \
					MSG_MSG("-->NICK <nickname>") + \
					MSG_MSG("   set a nickname for the chat") + \
					MSG_MSG("-->USER <username> <hostname> <servername> <realname>") + \
					MSG_MSG("	  Exampler \"USER max 0 * max_mustermann\" Use for login") + \
					MSG_MSG("-->PRIVMSG <channelname/nickname> <message>") + \
					MSG_MSG("   Use to chat to channel or nick") + \
					MSG_MSG("-->JOIN #<channelname>") + \
					MSG_MSG("	  Use for join or creat a chanel") + \
					MSG_MSG("-->PART #<channelname>") + \
					MSG_MSG("	  Use to logout from channel") + \
					MSG_MSG("-->MODE #<channelname> <option>") + \
					MSG_MSG("	  Only for admins") + \
					MSG_MSG("	  <option>") + \
					MSG_MSG("	     +/-k set/remove key (password)") + \
					MSG_MSG("	     	Exampel: \"MODE #<channelname> +k <channelpassword>\"") + \
					MSG_MSG("	     +/-i set/remove invite-only channel") + \
					MSG_MSG("	     	Exampel: \"MODE #<channelname> +i\"") + \
					MSG_MSG("	     +/-t set/remove restrictions of the TOPIC command to channel") + \
					MSG_MSG("	     	Exampel: \"MODE #<channelname> +t\"") + \
					MSG_MSG("	     +/-o set/remove channel opererator to user") + \
					MSG_MSG("	     	Exampel: \"MODE #<channelname> +0 <username>\"") + \
					MSG_MSG("	     +/-l set/remove user limits to channel") + \
					MSG_MSG("	     	Exampel: \"MODE #<channelname> +l <int nummber>\"") + \
					MSG_MSG("-->TOPIC <topic>") + \
					MSG_MSG("	  set a channel topic") + \
					MSG_MSG("-->WHOIAM") + \
					MSG_MSG("	  Print information about yourself") + \
					MSG_MSG("-->CHANNELS") + \
					MSG_MSG("	  Prints inforamtion about all channels") + \
					MSG_MSG("-->CLIENTS") + \
					MSG_MSG("	  Prints Nick from logedin clients") + \
					MSG_MSG("-->HELP") + \
					MSG_MSG("	  Print all useable commands"),fd);

}

/**
 * @brief Send the Personal information to the client about his self
 * 
 * @param fd filediscripter from client
 */
void Server::WHOIAM(int fd)
{
	_sendResponse(	MSG_MSG("  --> Nick: " + (GetClient(fd)->GetNickName())) + \
					MSG_MSG("  --> User: " + (GetClient(fd)->GetUserName())),fd);
	if (GetClient(fd)->GetLogedIn())
		_sendResponse(MSG_MSG("  --> Login: YES"),fd);
	else
		_sendResponse(MSG_MSG("  --> Login: NO"),fd);
}

/**
 * @brief Print all Channels information to the Client 
 * 
 * @param fd filediscripter from client
 */
void Server::CHANNELS(int fd)
{
	if (channels.empty()) 
		{_sendResponse(MSG_MSG("   --> No Channels"), fd);return;}

	std::string nickname = GetClient(fd)->GetNickName();
	std::string channelname;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) 
	{
		channelname = it->getName();
		std::vector<Client> admins = it->getAllAdmins();
		std::vector<Client> channelclients = it->getAllClients();
		if (!it->getUserInChannelAdmin(nickname) && !it->getUserInChannelClient(nickname))
			{_sendResponse(MSG_MSG("   You are not in channel: #" + channelname),fd);}
		if(it->getUserInChannelAdmin(nickname))
		{	
			_sendResponse(MSG_MSG("   -->You are admin in channel: #" + channelname + "\n      other Admins"),fd);
			for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
			{
				if (nickname != it->GetNickName())
					{_sendResponse(MSG_MSG(std::string(BLU) + "  --> Nick: " + it->GetNickName() + std::string(RESET)),fd);}
			}
			_sendResponse(MSG_MSG("   -->Clients in channel: #" + channelname),fd);
			for (std::vector<Client>::iterator it = channelclients.begin(); it != channelclients.end(); ++it)
				{_sendResponse(MSG_MSG(std::string(BLU) + "     --> Nick: " + it->GetNickName() + std::string(RESET)),fd);}	

		}
		else if(it->getUserInChannelClient(nickname))
		{
			_sendResponse(MSG_MSG("   -->You are client in channel: #" + channelname + "\n      other Clients" + std::string(RESET)),fd);
			for (std::vector<Client>::iterator it = channelclients.begin(); it != channelclients.end(); ++it)
			{
				if (nickname != it->GetNickName())
					{_sendResponse(MSG_MSG(std::string(BLU) + "     --> Nick: " + it->GetNickName() + std::string(RESET)),fd);}
			}
			_sendResponse(MSG_MSG("   -->Admins in channel: #" + channelname),fd);
			for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
				{_sendResponse(MSG_MSG(std::string(BLU) + "     --> Nick: " + it->GetNickName() + std::string(RESET)),fd);}			
		}		
	}
}

void Server::CLIENTS(int fd)
{
	for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it){
		if (fd != it->GetFd()){
			if (it->GetLogedIn())
				{_sendResponse(MSG_MSG("  -->Nick: " + it->GetNickName()),fd);}
		}
	}
}