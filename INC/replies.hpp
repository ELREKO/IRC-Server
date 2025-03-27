#define once

#define CRLF "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " : Welcome to the IRC server!" + CRLF)
// #define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + CRLF
#define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE #" + channelname + " " + mode + " " + arguments + CRLF)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN #" + channelname + CRLF)
#define RPL_CHANNELMODES(nickname, channelname, modes) ": 324 " + nickname + " #" + channelname + " " + modes + CRLF
#define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " #" + channelname + " " + creationtime + CRLF
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " #" +channelname + " :" + topic + "\r\n")
#define RPL_INVITING(nickname, targetnickname, channelname) (": 341 " + nickname + " " + targetnickname + " #" + channelname + CRLF)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ #" + channelname + " :" + clientslist + CRLF)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " #" + channelname + " :END of /NAMES list" + CRLF)

///////// ERRORS ////////////////
#define ERR_NOSUCHNICK(channelname, name) (std::string(RED) + ": 401 #" + channelname + " " + name + " :No such nick/channel" + std::string(RESET) + CRLF )
#define ERR_CHANNELNOTFOUND(nickname, channelname) (std::string(RED) + ": 403 " + nickname + " " + channelname + " :No such channel" + std::string(RESET) + CRLF)
#define ERR_CMDNOTFOUND(nickname, command) (std::string(RED) + ": 421 " + nickname + " " + command + " :Unknown command --> Use \"HELP\"" + std::string(RESET) + CRLF)
#define ERR_ERRONEUSNICK(nickname) (std::string(RED) + ": 432 " + nickname + " :Erroneus nickname" + std::string(RESET) + CRLF)
#define ERR_NICKINUSE(nickname) (std::string(RED) + ": 433 " + nickname + " :Nickname is already in use" + std::string(RESET) + CRLF)
#define ERR_NOTONCHANNEL(nickname, channelname) (std::string(RED) + ": 442 " + nickname + " #" + channelname + " :You're not on that channel" + std::string(RESET) + CRLF)
#define ERR_USERONCHANNEL(nickname, channelname) (std::string(RED) + ": 443 " + nickname + " #" + channelname + " :is already on channel" + std::string(RESET) + CRLF)

#define ERR_SUMMONDISABLED(port) (std::string(RED) + ": 445 Server Port (" + port + ") is down!" + std::string(RESET) + CRLF)
#define ERR_NOTREGISTERED(nickname) (std::string(RED) + ": 451 " + nickname + " :You have not registered!" + std::string(RESET) + CRLF)
#define ERR_NOTENOUGHPARAM(nickname) (std::string(RED) + ": 461 " + nickname + " :Not enough parameters." + std::string(RESET) + CRLF)
#define ERR_ALREADYREGISTERED(nickname) (std::string(RED) + ": 462 " + nickname + " :You may not reregister !" + std::string(RESET) + CRLF )
#define ERR_INCORPASS(nickname) (std::string(RED) + ": 464 " + nickname + " :Password incorrect !" + std::string(RESET) + CRLF )
#define ERR_KEYSET(channelname) (std::string(RED) + ": 467 #" + channelname + " Channel key already set. " + std::string(RESET) + CRLF)

#define ERR_UNKNOWNMODE(nickname, channelname, mode) (std::string(RED) + ": 472 " + nickname + " #" + channelname + " " + mode + " :is not a recognised channel mode" + std::string(RESET) + CRLF)
#define ERR_CHANNELISFULL(nickname, channelname) (std::string(RED) + ": 473 " + nickname + " #" + channelname + " :Channel is full" + std::string(RESET) + CRLF)
#define ERR_NOTOPERATOR(channelname) (std::string(RED) + ": 482 #" + channelname + " :You're not a channel operator" + std::string(RESET) + CRLF)
#define ERR_NEEDMODEPARM(channelname, mode) (std::string(RED) + ": 696 #" + channelname + " * You must specify a parameter for the key mode. " + std::string(RESET) + mode + CRLF)
#define ERR_INVALIDMODEPARM(channelname, mode) (std::string(RED) + ": 696 #" + channelname + " Invalid mode parameter. " + std::string(RESET) + mode + CRLF)

////////// MSG TO CLIENT /////////
#define MSG_CLIENTNUMMBER(port,fd) (std::string(YEL) + "MSG from Server (" + port + "): You are Connection as Number " + fd + "!" + CRLF \
									+ std::string(BLU) + "Please use Commands \"pass\", to login" + std::string(RESET) + CRLF)
#define MSG_SERVERACCEPT(port,cmd) (std::string(YEL) + "MSG from Server (" + port + "): " + cmd + " accepted !" + std::string(RESET) + CRLF)
#define MSG_CLIENTNEEDUSER(user) (std::string(BLU) +"Please use Commands  \"user\", to get a more information" + std::string(RESET) + CRLF)
#define MSG_CLIENTNEEDNICK(nickname) (std::string(BLU) +"Please use Commands  \"nick\", to set your nickname" + std::string(RESET) + CRLF)
#define MSG_CLIENTNICKCHANGE(oldnickname, nickname) (std::string(BLU) + "Nickname changed from |" + oldnickname + "| to NICK |" + nickname + "|" + std::string(RESET) + CRLF)
#define MSG_CLIENTSAMENICK(nickname) (std::string(BLU) +"You have allrady the nickname: " + nickname + std::string(RESET) + CRLF)
#define MSG_MSG(msg) (std::string(RESET) + "" + msg + CRLF)
