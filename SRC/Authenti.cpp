#include "../INC/Server.hpp"

/* 
*   PASS COMMAND
*/

void Server::client_authen(int fd, std::string cmd)
{
	Client *cli = GetClient(fd);
	cmd = cmd.substr(4);
	size_t pos = cmd.find_first_not_of("\t\v ");
	if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	if(pos == std::string::npos || cmd.empty()) 
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if(!cli->getRegistered())
	{
		std::string pass = cmd;
		if(pass == password)
		{
			cli->setRegistered(true);
			if (Flag_PrintInfoServer == 1)
				{std::cout << printBackground (LBLU,BYEL,"PASS:") << BLU << " Client <" << cli->GetFd() << "> accepted pass" << RESET  << std::endl;}
			if (Flag_PrintInfoClient == 1)
				{	
					_sendResponse(MSG_SERVERACCEPT(getPort_str(), "password"), fd);
					_sendResponse(MSG_CLIENTNEEDNICK("*"), fd);
				}
		}
		else
            _sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
        _sendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->GetNickName()), fd);
}


/* 
*    NICK COMMAND
*/

bool Server::is_validNickname(std::string& nickname)
{
		
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!std::isalnum(nickname[i]) && nickname[i] != '_')
			return false;
	}
	return true;
}


bool Server::nickNameInUse(std::string& nickname)
{
	for (size_t i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i].GetNickName() == nickname)
			return true;
	}
	return false;
}


void Server::set_nickname(std::string cmd, int fd)
{
	std::string inuse;
	cmd = cmd.substr(4);
	size_t pos = cmd.find_first_not_of("\t\v ");
	if(pos < cmd.size())
	{
		cmd = cmd.substr(pos);
		if(cmd[0] == ':')
			cmd.erase(cmd.begin());
	}
	Client *cli = GetClient(fd);
	if (cmd == cli->GetNickName())
		{_sendResponse(MSG_CLIENTSAMENICK(std::string(cmd)), fd); return;}
	if(pos == std::string::npos || cmd.empty()){
		_sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd); 
		return;
	}

	if (nickNameInUse(cmd) && cli->GetNickName() != cmd){
		std::string inuse = "*";
		if(cli->GetNickName().empty())
			{cli->SetNickname(inuse);}
		_sendResponse(ERR_NICKINUSE(std::string(cmd)), fd); 
		return;
	}
	if(!is_validNickname(cmd)) {
		_sendResponse(ERR_ERRONEUSNICK(std::string(cmd)), fd);
		return;
	}
	else
	{
		if(cli && cli->getRegistered())
		{
			std::string oldnick = cli->GetNickName();
			cli->SetNickname(cmd);
			if (Flag_PrintInfoServer == 1 && oldnick.empty())
				{std::cout << printBackground (LBLU,BYEL,"NICK:") << BLU << " Client <" << cli->GetFd() << "> set nick: " << cli->GetNickName() << RESET  << std::endl;}
			if (Flag_PrintInfoClient == 1 && oldnick.empty())
				{_sendResponse(MSG_SERVERACCEPT(getPort_str(),"nick"), fd);}
			for(size_t i = 0; i < channels.size(); i++){
				Client *cl = channels[i].findClientByName(oldnick);
				if(cl)
					cl->SetNickname(cmd);
			}

			if(!oldnick.empty() && oldnick != cmd)
			{
				if (Flag_PrintInfoClient == 1)
					{_sendResponse(MSG_SERVERACCEPT(getPort_str(),"nick"), fd);}

				if (Flag_PrintInfoServer == 1)
					{std::cout << printBackground (LBLU,BYEL,"NICK:") << BLU << " Client <" << cli->GetFd() << "> changed nick from: " << oldnick << " to: " <<cli->GetNickName() << RESET  << std::endl;}

				if(oldnick == "*" && !cli->GetUserName().empty())
				{
					cli->setLogedin(true);
					_sendResponse(RPL_CONNECTED(cli->GetNickName()), fd);
					_sendResponse(MSG_CLIENTNICKCHANGE(cli->GetNickName(),cmd), fd);
				}
				else
					{_sendResponse(MSG_CLIENTNICKCHANGE(oldnick,cmd), fd);}
			}
			
		}
		else if (cli && !cli->getRegistered())
			_sendResponse(ERR_NOTREGISTERED(cmd), fd);

		if (Flag_PrintInfoClient == 1 && cli->GetUserName().empty())
		{_sendResponse(MSG_CLIENTNEEDUSER("*"), fd);}
	}
	checkAutenticationFinich(cli);
}

/* 
    USER COMMAND
*/

void	Server::set_username(std::string& cmd, int fd)
{
	std::vector<std::string> splited_cmd = split_cmd(cmd);

	Client *cli = GetClient(fd); 
	if((cli && splited_cmd.size() < 5))
		{_sendResponse(ERR_NOTENOUGHPARAM(cli->GetNickName()), fd); return; }
	if(!cli  || !cli->getRegistered())
		_sendResponse(ERR_NOTREGISTERED(std::string("*")), fd);
	else if (cli && !cli->GetUserName().empty())
		{_sendResponse(ERR_ALREADYREGISTERED(cli->GetNickName()), fd); return;}
	else
	{
		cli->SetUsername(splited_cmd[1]);	
		if (Flag_PrintInfoClient == 1)
		{
			_sendResponse(MSG_SERVERACCEPT(getPort_str(),"user"), fd);
			if (cli->GetNickName().empty() || cli->GetNickName() == "*")
				{_sendResponse(MSG_CLIENTNEEDNICK("*"), fd);}
		}
		if (Flag_PrintInfoServer == 1)
			{{std::cout << printBackground (LBLU,BYEL,"USER:") << BLU << " Client <" << cli->GetFd() << "> set username: " << cli->GetUserName() << RESET  << std::endl;}}
	}
	checkAutenticationFinich(cli);
}

void Server::checkAutenticationFinich(Client *cli)
{
	if(cli && cli->getRegistered() && !cli->GetUserName().empty() && !cli->GetNickName().empty() && cli->GetNickName() != "*"  && !cli->GetLogedIn())
	{
		cli->setLogedin(true);
		_sendResponse(RPL_CONNECTED(cli->GetNickName()), cli->GetFd());
		if (Flag_PrintInfoServer == 1)
		{
		std::cout  << printBackground (LBLU,BYEL,"USER LOGIN:") << BLU
						 	<< " Client <" << cli->GetFd() << "> finisched login " 
							<< "Nick: |" << cli->GetNickName() 
							<< "| Username: |" << cli->GetUserName() << "|" 
							<< RESET  << std::endl;
		}
	}

		
}
