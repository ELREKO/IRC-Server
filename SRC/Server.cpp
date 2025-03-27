#include "../INC/Server.hpp"

bool Server::isBotfull = false;
Server::Server(){this->server_fdsocket = -1;}
Server::~Server(){}
Server::Server(Server const &src){*this = src;}
Server &Server::operator=(Server const &src){
	if (this != &src){
		this->port = src.port;
		this->server_fdsocket = src.server_fdsocket;
		this->password = src.password;
		this->clients = src.clients;
		this->channels = src.channels;
		this->fds = src.fds;
		this->isBotfull = src.isBotfull;
	}
	return *this;
}
//---------------//Getters
int Server::GetPort()
{return this->port;}

std::string Server::getPort_str()
{
	std::ostringstream oss;
	oss << this->GetPort();  
	std::string str = oss.str();
	return str;
}
int Server::GetFd()
{return this->server_fdsocket;}

std::string Server::GetFd_str()
{
	std::ostringstream oss;
	oss << this->GetFd() + 1;  
	std::string str = oss.str();
	return str;
}

Client *Server::GetClient(int fd)
{
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			return &this->clients[i];
	}
	return NULL;
}



Client *Server::GetClientNick(std::string nickname)
{
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetNickName() == nickname)
			return &this->clients[i];
	}
	return NULL;
}

Channel *Server::GetChannel(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].getName() == name)
			return &channels[i];
	}
	return NULL;
}
//---------------//Getters
//---------------//Setters
void Server::SetFd(int fd){this->server_fdsocket = fd;}
void Server::SetPort(int port){this->port = port;}
void Server::setPassword(std::string password){this->password = password;}
std::string Server::GetPassword(){return this->password;}
void Server::AddClient(Client newClient){this->clients.push_back(newClient);}
void Server::AddChannel(Channel newChannel){this->channels.push_back(newChannel);}
void Server::AddFds(pollfd newFd){this->fds.push_back(newFd);}
//---------------//Setters
//---------------//Remove Methods
void Server::RemoveClient(int fd){
	for (size_t i = 0; i < this->clients.size(); i++){
		if (this->clients[i].GetFd() == fd)
			{this->clients.erase(this->clients.begin() + i); return;}
	}
}
void Server::RemoveChannel(std::string name){
	for (size_t i = 0; i < this->channels.size(); i++){
		if (this->channels[i].getName() == name)
			{this->channels.erase(this->channels.begin() + i); return;}
	}
}
void Server::RemoveFds(int fd){
	for (size_t i = 0; i < this->fds.size(); i++){
		if (this->fds[i].fd == fd)
			{this->fds.erase(this->fds.begin() + i); return;}
	}
}
void	Server::RmChannels(int fd){
	for (size_t i = 0; i < this->channels.size(); i++){
		int flag = 0; // 1 = client; 2 = admin;
		std::string rpl;
		if (channels[i].getClient(fd))
		{
			channels[i].removeClient(fd); 
			rpl = "CHANNEL <#" + channels[i].getName() +">: Client :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + " is leaving (Reason: SIGINT (Signal(strg+c)).\r\n";
			flag = 1;
		}
		else if (channels[i].getAdmin(fd))
		{
			channels[i].removeAdmin(fd);
			rpl =  "CHANNEL <#" + channels[i].getName() +">: Admin :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + " is leaving (Reason: SIGINT (Signal(strg+c)).\r\n";
			flag = 2;
		}
		if (channels[i].getClientsNumber() == 0)
			{channels.erase(channels.begin() + i); i--; continue;}
		if (flag){
			if (channels[i].getAllAdmins().size() == 0 && flag == 2)
			{
				rpl = std::string(RED) + "CHANNEL <#" + channels[i].getName() +">: Last admin :" + GetClient(fd)->GetNickName() + "!~" + GetClient(fd)->GetUserName() + \
				" is leaving (Reason: SIGINT (Signal(strg+c)). \nNo Admin in Channel #" + channels[i].getName() + \
				std::string(RESET) +  "\r\n";
			}
			channels[i].broadcastMessage(rpl);
		}
	}
}
//---------------//Remove Methods
//---------------//Send Methods
void Server::senderror(int code, std::string clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void Server::senderror(int code, std::string clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if(send(fd, resp.c_str(), resp.size(),0) == -1)
		std::cerr << "send() faild" << std::endl;
}

void Server::_sendResponse(std::string response, int fd)
{
	if((send(fd, response.c_str(), response.size(), 0) == -1) && fd > 3)

		std::cerr << "Response send(" << fd <<") faild" << std::endl;
}
//---------------//Send Methods
//---------------//Close and Signal Methods
bool Server::Signal = false;
void Server::SignalHandler(int signum)
{
	//(void)signum;
	if (signum == 2)  
		std::cout << std::endl << "Signal \"SIGINT (ctrl + c)\" Received, Stop Serverprogramm ... " << std::endl;
	if (signum == 3)  
		std::cout << std::endl << "Signal \"SIGQUIT (ctrl + \\)\" Received, Stop Serverprogramm ... " << std::endl;
	Server::Signal = true;
}

void	Server::close_fds(){
	for(size_t i = 0; i < clients.size(); i++){
		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(clients[i].GetFd());
	}
	if (server_fdsocket != -1){	
		std::cout << RED << "Server <" << server_fdsocket << "> Disconnected" << WHI << std::endl;
		close(server_fdsocket);
	}
}
//---------------//Close and Signal Methods
//---------------//Server Methods
/**
 * @brief Open a loop till no signal (SIGINT; SIGQUIT) was send!
 * 			In case of signal was send -> send a message to the Server
 * 
 * @param portnummber 
 * @param password 
 */
void Server::init(int portnummber, std::string password)
{
	this->password = password;
	this->port = portnummber;
	this->set_sever_socket();

	std::cout << GRE << "Server <" << server_fdsocket << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";
	while (!Server::Signal)
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && !Server::Signal)
			throw(std::runtime_error("poll() faild"));
		
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == server_fdsocket)
					this->accept_new_client();
				else if ((Flag_PrintInfoServer == 1) && (fds[i].fd == fileno(stdin)))
					{this->CheckServerInput();}
				else
					this->reciveNewData(fds[i].fd);
			}
		}

	}
	// Signal (SIGINT, SIGQUIT) was send (like "strg+c")! 
	if (Server::Signal == true)
	{
		if ( Flag_PrintInfoClient == 1){
			for (size_t i = 0; i < fds.size(); i++)
				{_sendResponse(ERR_SUMMONDISABLED(getPort_str()),fds[i].fd);}
		}
	}
	close_fds();
}



void Server::set_sever_socket()
{
	int en = 1;
	serv_socket_add.sin_family = AF_INET;
	serv_socket_add.sin_addr.s_addr = INADDR_ANY;
	serv_socket_add.sin_port = htons(port);
	server_fdsocket = socket(AF_INET, SOCK_STREAM, 0);
	if(server_fdsocket == -1)
		throw(std::runtime_error("faild to create socket"));
	if(setsockopt(server_fdsocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(server_fdsocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(server_fdsocket, (struct sockaddr *)&serv_socket_add, sizeof(serv_socket_add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(server_fdsocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));
	new_cli.fd = server_fdsocket;
	new_cli.events = POLLIN;
	new_cli.revents = 0;
	fds.push_back(new_cli);

	// set serverinput
	if (Flag_PrintInfoServer == 1)
	{ 
		struct pollfd server_input;
		server_input.fd = fileno(stdin);
		server_input.events = POLLIN;
		fds.push_back(server_input);
	}
}


void Server::accept_new_client()
{
	Client cli;
	memset(&cliadd, 0, sizeof(cliadd));
	socklen_t len = sizeof(cliadd);
	int incofd = accept(server_fdsocket, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}
	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
		{std::cout << "fcntl() failed" << std::endl; return;}
	new_cli.fd = incofd;

	new_cli.events = POLLIN;
	new_cli.revents = 0;
	cli.SetFd(incofd);
	cli.setIpAddress(inet_ntoa((cliadd.sin_addr)));
	clients.push_back(cli);      
	fds.push_back(new_cli);
	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;

	if ( Flag_PrintInfoClient == 1)
		{_sendResponse(MSG_CLIENTNUMMBER(getPort_str(),GetFd_str()),cli.GetFd());}
}

void Server::reciveNewData(int fd)
{
	std::vector<std::string> cmd;
	char buff[1024];
	memset(buff, 0, sizeof(buff));
	Client *cli = GetClient(fd);
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0);
	if(bytes <= 0)
	{
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		RmChannels(fd);
		RemoveClient(fd);
		RemoveFds(fd);
		close(fd);
	}
	else
	{ 
		cli->setBuffer(buff);
		if(cli->getBuffer().find_first_of("\r\n") == std::string::npos)
			return;
		cmd = split_recivedBuffer(cli->getBuffer());
		for(size_t i = 0; i < cmd.size(); i++)
			this->parse_exec_cmd(cmd[i], fd);
		
		// test
		if(GetClient(fd))
			GetClient(fd)->clearBuffer();
	}
}

//---------------//Server Methods
//---------------//Parsing Methods
std::vector<std::string> Server::split_recivedBuffer(std::string str)
{
	std::vector<std::string> vec;
	std::istringstream stm(str);
	std::string line;
	while(std::getline(stm, line))
	{
		size_t pos = line.find_first_of("\r\n");
		if(pos != std::string::npos)
			line = line.substr(0, pos);
		vec.push_back(line);
	}
	return vec;
}

/**
 * @brief 
 * Split Param("cmd") by token and save each token to the 'vec' and return it.
 */
std::vector<std::string> Server::split_cmd(std::string& cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while(stm >> token)
	{
		vec.push_back(token);
		token.clear();
	}
	return vec;
}

bool Server::notregistered(int fd)
{
	if (!GetClient(fd) || GetClient(fd)->GetNickName().empty() || GetClient(fd)->GetUserName().empty() || GetClient(fd)->GetNickName() == "*"  || !GetClient(fd)->GetLogedIn())
		return true;
	return false;
}

void Server::parse_exec_cmd(std::string &cmd, int fd)
{
	if(cmd.empty())
		return ;
	std::vector<std::string> splited_cmd = split_cmd(cmd);
	size_t found = cmd.find_first_not_of(" \t\v");
	if(found != std::string::npos)
		cmd = cmd.substr(found);
	if(splited_cmd.size() && (splited_cmd[0] == "BONG" || splited_cmd[0] == "bong"))
		return;
    if(splited_cmd.size() && (splited_cmd[0] == "PASS" || splited_cmd[0] == "pass"))
		client_authen(fd, cmd);
	else if (splited_cmd.size() && (splited_cmd[0] == "NICK" || splited_cmd[0] == "nick"))
		set_nickname(cmd,fd);
	else if(splited_cmd.size() && (splited_cmd[0] == "USER" || splited_cmd[0] == "user"))
		set_username(cmd, fd);
	else if (splited_cmd.size() && (splited_cmd[0] == "QUIT" || splited_cmd[0] == "quit"))
		QUIT(cmd,fd);
	else if (Flag_PrintInfoClient == 1 && splited_cmd.size() && (splited_cmd[0] == "HELP" || splited_cmd[0] == "help") )
		HELP(fd);
	else if (Flag_PrintInfoClient == 1 && splited_cmd.size() && (splited_cmd[0] == "WHOIAM" || splited_cmd[0] == "whoiam"))
		WHOIAM(fd);
	else if(!notregistered(fd))
	{
		if (splited_cmd.size() && (splited_cmd[0] == "KICK" || splited_cmd[0] == "kick"))
			KICK(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "JOIN" || splited_cmd[0] == "join"))
			JOIN(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "TOPIC" || splited_cmd[0] == "topic"))
			Topic(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "MODE" || splited_cmd[0] == "mode"))
			mode_command(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PART" || splited_cmd[0] == "part"))
			PART(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "PRIVMSG" || splited_cmd[0] == "privmsg"))
			PRIVMSG(cmd, fd);
		else if (splited_cmd.size() && (splited_cmd[0] == "INVITE" || splited_cmd[0] == "invite"))
			Invite(cmd,fd);
		else if (Flag_PrintInfoClient == 1 && splited_cmd.size() && (splited_cmd[0] == "CLIENTS" || splited_cmd[0] == "clients"))
			CLIENTS(fd);
		else if (Flag_PrintInfoClient == 1 && splited_cmd.size() && (splited_cmd[0] == "CHANNELS" || splited_cmd[0] == "channels"))
			CHANNELS(fd);
		else if (splited_cmd.size())
			_sendResponse(ERR_CMDNOTFOUND(GetClient(fd)->GetNickName(),splited_cmd[0]),fd);
	}
	else if (notregistered(fd))
		_sendResponse(ERR_NOTREGISTERED(std::string("*")),fd);
}
//---------------//Parsing Methods
// ------ Loggint Prints
void Server::CheckServerInput()
{
        std::string Input;
        std::getline(std::cin, Input);
        if (!Input.empty())
        {
            std::cout << YEL << "[SERVER INPUT]: |" << Input << "|" << RESET << std::endl;
			if (Input == "PR_CLIENTS" || Input == "pr_clients")
            	{printClients();}
			else if (Input == "PR_CHANNELS" || Input == "pr_channels")
            	{printChannels();}
			else if (Input == "EXIT" || Input == "exit")
            	{Server::Signal = true;}
			else
				{std::cout << " --> Unknown Command! Use (pr_clients; pr_channels)" << std::endl;}
        }
}

void Server::printClients() {
    if (clients.empty()) {
        std::cout << " --> No clients connected." << std::endl;
        return;
    }
    std::cout << " --> Connected Clients: " << std::endl;
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        std::cout 	<< GRE
					<< " --> FD |"  << it->GetFd() << "| -- "  
					<< "Registed |" << it->getRegistered() << "| -- "
					<< "Nickname |" << it->GetNickName() << "| -- "
					<< "Username |" << it->GetUserName() << "| -- "
					<< "LoggedIn |" << it->GetLogedIn() << "|"
					<< RESET << std::endl; 
    }
}

void Server::printChannels() {
    if (channels.empty()) {
        std::cout << " --> No channel" << std::endl;
        return;
    }
    std::cout << " --> Channels: " << std::endl;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it) 
	{
        std::cout 	<< BLU 
					<< " --> Channel Name |"  << it->getName() << "|" 
					<< " Topic |" << it->getTopicName() << "|"
					<< " Password |" << it->getModeStatusAtIndex(2) << "|"
					<< RESET << std::endl;
		it->PrintClientsServer(GRE);
		it->PrintAdminsServer(GRE);
		for (size_t i = 0; i < fds.size(); ++i) {
			std::cout << "Index " << i << ":\n";
			std::cout << "  fd: " << fds[i].fd << "\n";
			std::cout << "  events: " << fds[i].events << "\n";
			std::cout << "  revents: " << fds[i].revents << "\n";
		}
    }
}

std::string Server::getClientNbrNick(int fd)
{
	std::stringstream ss;
	ss << GetClient(fd) -> GetFd();
	return (" Client <" + ss.str() + "," + GetClient(fd)->GetNickName() + ">" );
}

std::string printBackground(std::string FontColor, std::string BackgroundColor, std::string text)
	{return FontColor + BackgroundColor + text + RESET;}

bool chrInStr(const std::string& input, const char chr) {
		for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
			if (*it == chr) {
				return true;
			}
		}
		return false;
}

bool Server::userIsTheOnlyAdmininChannels(Channel channel, int &fd)
{
	std::vector<Client> adminCli = channel.getAllAdmins();
	if ((adminCli.size() == 1) && (GetClient(fd)->GetNickName() == adminCli[0].GetNickName())){
		return true;
	}
	return false;
}
