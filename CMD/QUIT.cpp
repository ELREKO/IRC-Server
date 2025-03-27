#include "../INC/Server.hpp"

/**
 * @brief 
 * [ role ]
 * Find Param(2):"tofind" on the Param(1):"cmd"
 * When it's found, save string after "tofind" to the Param(3):"&str".
 *
 * 					[ Example ]
 * @param cmd 		quit :sorry, have to go
 * @param tofind 	quit
 * @param str		:cause it's too tired
 * 
 * [ Background ]
 *	for (; i<cmd.size(0; i++)
 *	= ; means initialize 'i' in cpp.
 */
void findAndExtractAfterWord(std::string &cmd, std::string &tofind, std::string &reason)
{
	size_t pos = 0;
	size_t start = 0;

	// 1. Start to find the word(= param(2))
	while (pos < cmd.size())
	{
		// 1-1. Skip blank(' ') in the front
		while (pos < cmd.size() && cmd[pos] == ' ')
			pos++;
		// 1-2. Set the start index, which word starts
		start = pos; // 4
		// 1-3. Goes up (pos)++, when it meets with blank(' ').
		while (pos < cmd.size() && cmd[pos] != ' ')
			pos++; // 11
		// 1-4. Compare word with Param(2), if finds -> break.
		if  (cmd.substr(start, pos - start) == tofind)
			break;
	}
	// 2. Substring & save it to the Param(3).
	if (pos < cmd.size())
	{
		reason = cmd.substr(pos);
		reason.erase(0, reason.find_first_not_of(" "));
	}
	else
		reason.clear();
}

/**
 * @brief 
 * Check the reason, that user used 'quit' and print out with reason together.
 */
std::string	SplitQuit(std::string cmd)
{
	// convert type from string to stream.
	std::istringstream stm(cmd);						// ex) cmd == quit :sorry, have to go
	std::string reason, str;

	// only first word from 'stm' is saved in str.
	stm >> str;											// ex) str == quit
	findAndExtractAfterWord(cmd, str, reason);			// ex) reason == :sorry, have to go

	// If reason is empty, just print out "Quit".
	if (reason.empty())
		return std::string("Quit");

	// If message doesn't start with ':'
	if (reason[0] != ':'){
		for (size_t i = 0; i < reason.size(); i++){
			if (reason[i] == ' ')
				{reason.erase(reason.begin() + i, reason.end());break;}
		}
		reason.insert(reason.begin(), ':');
	}
	return reason;
}

void Server::QUIT(std::string cmd, int fd)
{
	std::string reason;
	reason = SplitQuit(cmd);
	for (size_t i = 0; i < channels.size(); i++)
	{
		if (channels[i].getClient(fd)){
			channels[i].removeClient(fd);
			if (channels[i].getClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else{
				std::string rpl = "CHANNEL <#" + channels[i].getName() +">: Client :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + " is leaving ([REASON] " + reason + ").\r\n";
				channels[i].broadcastMessage(rpl);
			}
		}
		else if (channels[i].getAdmin(fd)){
			bool flag = userIsTheOnlyAdmininChannels(channels[i],fd);
			channels[i].removeAdmin(fd);
			if (channels[i].getClientsNumber() == 0)
				channels.erase(channels.begin() + i);
			else{
				std::string rpl;
				if (!flag){
					rpl =  "CHANNEL <#" + channels[i].getName() +">: Admin :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + " is leaving ([REASON] " + reason + ").\r\n";
				}
				else{
					rpl = std::string(RED) + "CHANNEL <#" + channels[i].getName() +">: Last admin :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + \
						" is leaving ([REASON] " + reason + "). \nNo Admin in Channel #" + channels[i].getName() + \
						std::string(RESET) +  "\r\n";
				}
				channels[i].broadcastMessage(rpl);
			}
		}
	}
	if (Flag_PrintInfoServer == 1){
		std::cout << printBackground(LBLU,BWHI,"QUIT") << BLU <<  getClientNbrNick(fd) << " Leave use Command QUIT! [REASON] " << reason << RESET <<std::endl;	
		std::cout << RED << "Client <" << fd << "> Disconnected" << RESET << std::endl;
	}
	RmChannels(fd);
	RemoveClient(fd);
	RemoveFds(fd);
	close(fd);
}