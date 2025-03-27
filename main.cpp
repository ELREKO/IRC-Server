#include "INC/Server.hpp"

bool isPortValid(std::string port){
	return (port.find_first_not_of("0123456789") == std::string::npos && \
	std::atoi(port.c_str()) >= 1024 && std::atoi(port.c_str()) <= 65535);
}

bool checkArgumentOkay(int ac, char **av)
{
	std::string err_str = 	std::string(RED) + "Wrong Arguments! \n"
							+ std::string(LBLU) + "Please Usages:" + std::string(av[0]) + " <port number> <password> \n";

	// for (int i = 1; i < ac; i++) 
	// {
	// 	std::cout << av[i] << std::endl;
	// 	if (std::strstr(av[i], "&&") != NULL)
	// 	{
	// 		std::cerr << err_str 
	// 		<< YEL << "No Operator!" 
	// 		<< std::endl;
	// 		return 0;	
	// 	}	
	// }
	if (ac != 3)
	{
		std::cerr << err_str << std::endl;
		return 0;
	}
	else if (!isPortValid(av[1]))
	{
		std::cerr 	<< err_str
					<< YEL << "Invalid Port number (must be 4 digits)" 
					<< std::endl;
		return 0;
	}
	else if (!*av[2] || std::strlen(av[2]) > 20)
	{
		std::cerr 	<< err_str
					<< YEL << "Invalid Password! (min 1; max 20; Charakter)" 
					<< std::endl;
		return 0;
	}
	else if (!isatty(STDOUT_FILENO))
	{
		std::cerr 	<< err_str
					<< YEL << "No Pipe!" 
					<< std::endl;
		return 0;		
	}
	return 1;
}

/**
 * @brief Main Programm to start the Server
 * 
 * @param ac nbr of given parameter
 * @param av array of String parameter
 * @return int in case of error return != 0
 */
int main(int ac, char **av)
{
	Server ircserv;
	
	if (checkArgumentOkay(ac, av) == 0)
	{return 1;}
	
	try
	{
		signal(SIGINT, Server::SignalHandler);
		signal(SIGQUIT, Server::SignalHandler);
		signal(SIGPIPE, SIG_IGN); // or MSG_NOSIGNAL flag in send() to ignore SIGPIPE on linux systems
		std::cout << LBLU << "---- SERVER START ----" << RESET << std::endl << std::endl;
		ircserv.init(std::atoi(av[1]), av[2]);
	}
	catch(const std::exception& e)
	{
		ircserv.close_fds();
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}
