#include "../INC/Server.hpp"

int Server::SplitJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string ChStr, PassStr, buff;
	std::istringstream iss(cmd);
	while(iss >> cmd)
		tmp.push_back(cmd);
	if (tmp.size() < 2) {token.clear(); return 0;}
	tmp.erase(tmp.begin());
	ChStr = tmp[0]; tmp.erase(tmp.begin());
	if (!tmp.empty()) {PassStr = tmp[0]; tmp.clear();}
	for (size_t i = 0; i < ChStr.size(); i++){
		if (ChStr[i] == ',')
				{token.push_back(std::make_pair(buff, "")); buff.clear();}
		else buff += ChStr[i];
	}
	token.push_back(std::make_pair(buff, ""));
	if (!PassStr.empty()){
		size_t j = 0; buff.clear();
		for (size_t i = 0; i < PassStr.size(); i++){
			if (PassStr[i] == ',')
				{token[j].second = buff; j++; buff.clear();}
			else buff += PassStr[i];
		}
		token[j].second = buff;
	}
	for (size_t i = 0; i < token.size(); i++)//erase the empty channel names
		{if (token[i].first.empty())token.erase(token.begin() + i--);}
	for (size_t i = 0; i < token.size(); i++){//ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
		if (*(token[i].first.begin()) != '#')
			{senderror(403, GetClient(fd)->GetNickName(), token[i].first, GetClient(fd)->GetFd(), " :No such channel\r\n"); token.erase(token.begin() + i--);}
		else
			token[i].first.erase(token[i].first.begin());
	}
	return 1;
}

int Server::SearchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].findClientByName(nickname))
			count++;
	}
	return count;
}

bool IsInvited(Client *cli, std::string ChName, int flag){
	if(cli->getInvitedUsers(ChName)){
		if (flag == 1)
			cli->RemoveChannelInvite(ChName);
		return true; // invitedChannel
	}
	return false;
}

void Server::ExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int j, int fd)
{
	if (this->channels[j].findClientByName(GetClient(fd)->GetNickName()))// if the client is already registered
		return;
	if (SearchForClients(GetClient(fd)->GetNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n"); return;}
	if (!this->channels[j].getPassword().empty() && this->channels[j].getPassword() != token[i].second){// ERR_BADCHANNELKEY (475) // if the password is incorrect
		if (!IsInvited(GetClient(fd), token[i].first, 0))
			{senderror(475, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n"); return;}
	}
	if (this->channels[j].getInviteOnlyMode()){// ERR_INVITEONLYCHAN (473) // if the channel is invit only
		if (!IsInvited(GetClient(fd), token[i].first, 1))
			{senderror(473, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n"); return;}
	}
	if (this->channels[j].getLimit() && this->channels[j].getClientsNumber() >= this->channels[j].getLimit())// ERR_CHANNELISFULL (471) // if the channel reached the limit of number of clients
		{senderror(471, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+l)\r\n"); return;}
	// add the client to the channel
	Client *cli = GetClient(fd);
	this->channels[j].addClient(*cli);
	if(channels[j].getTopicName().empty())
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAddress(),token[i].first) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickName(),channels[j].getName(),channels[j].getChannelUserList()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),channels[j].getName()),fd);
	else
		_sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAddress(),token[i].first) + \
			RPL_TOPICIS(GetClient(fd)->GetNickName(),channels[j].getName(),channels[j].getTopicName()) + \
			RPL_NAMREPLY(GetClient(fd)->GetNickName(),channels[j].getName(),channels[j].getChannelUserList()) + \
			RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),channels[j].getName()),fd);
    channels[j].broadcastMessage(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAddress(),token[i].first), fd);
	if (Flag_PrintInfoServer == 1)
		std::cout  << printBackground(LBLU,BWHI,"JOIN:") << BLU  << getClientNbrNick(fd) << " join channel #" << token[i].first << RESET << std::endl;
}


void Server::NotExistCh(std::vector<std::pair<std::string, std::string> >&token, int i, int fd)
{
	if (SearchForClients(GetClient(fd)->GetNickName()) >= 10)//ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
		{senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n"); return;}
	Channel newChannel;
	newChannel.setName(token[i].first);
	newChannel.addAdmin(*GetClient(fd));
	newChannel.setChannelCreationTimestamp();
	this->channels.push_back(newChannel);
	// notifiy thet the client joined the channel
    _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(),GetClient(fd)->getIpAddress(),newChannel.getName()) + \
        RPL_NAMREPLY(GetClient(fd)->GetNickName(),newChannel.getName(),newChannel.getChannelUserList()) + \
        RPL_ENDOFNAMES(GetClient(fd)->GetNickName(),newChannel.getName()),fd);
	if (Flag_PrintInfoServer == 1)
		std::cout << printBackground(LBLU,BWHI,"JOIN:") << BLU  << getClientNbrNick(fd) << " creat and join channel <#" << token[i].first << "> he is admin" << RESET << std::endl;
}


void Server::JOIN(std::string cmd, int fd)
{
	std::vector<std::pair<std::string, std::string> > token;
	if (!SplitJoin(token, cmd, fd))// ERR_NEEDMOREPARAMS (461) // if the channel name is empty
		{senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n"); return;}
	if (token.size() > 10) //ERR_TOOMANYTARGETS (407) // if more than 10 Channels
		{senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many channels\r\n"); return;}
	for (size_t i = 0; i < token.size(); i++){
		bool flag = false;
		for (size_t j = 0; j < this->channels.size(); j++){
			if (this->channels[j].getName() == token[i].first){
				ExistCh(token, i, j, fd);
				flag = true; break;
			}
		}
		if (!flag)
			NotExistCh(token, i, fd);
	}
}

