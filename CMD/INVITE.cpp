#include "../INC/Server.hpp"

/**	[ Finished ]
 * @brief 
 * [ splitedCmd example ]
 * Original cmd: "invite jason #tc"
 * 		splitedCmd[0]: invite
 * 		splitedCmd[1]: jason
 * 		splitedCmd[2]: #tc
 *
 * [ Handling Error cases ]
 *	1. ERR_NOTENOUGHPARAM (461)		: Not enough parameters
 * 	2. ERR_CHANNELNOTFOUND (403)	: Given channel doesn't exist
 * 	3. ERR_NOTONCHANNEL (442)		: Client isn't on the channel
 *  4. ERR_USERONCHANNEL (443)		: Given nickname is already on the channel
 * 	5. ERR_NOSUCHNICK (401)			: Given nickname is not found in the server
 * 	6. ERR_INVITEONLYCHAN (482)		: Channel is in invite-only mode,
 *									  Users cannot be invited unless they are admins.
 * 	7. ERR_CHANNELISFULL (473)		: Channel is already full.
 */
void Server::Invite(std::string &cmd, int &fd)
{
	std::vector<std::string> splitedCmd = split_cmd(cmd);

	// 1. ERR_NEEDMOREPARAMS (461)
	if(splitedCmd.size() < 3)
		{senderror(461, GetClient(fd)->GetNickName(), fd, " :Not enough parameters\r\n"); return;}
	
	// Extract string(= channelName: #tc) from index1 to the end.
	//	-> index[0]: #
	//	-> index[1]: t
	//	-> index[2]: c
	//	= "tc" saved in channelName.
	std::string channelName = splitedCmd[2].substr(1);

	// 2. ERR_CHANNELNOTFOUND (403)
	if(splitedCmd[2][0] != '#' || !GetChannel(channelName))
	    {senderror(403, channelName, fd, " :No such channel\r\n"); return;}

	// 3. ERR_NOTONCHANNEL (442)
	if (!(GetChannel(channelName)->getClient(fd)) && !(GetChannel(channelName)->getAdmin(fd)))
		{senderror(442, channelName, fd, " :You're not on that channel\r\n"); return;}

	// 4. ERR_USERONCHANNEL (443)
	if (GetChannel(channelName)->findClientByName(splitedCmd[1]))
	    {senderror(443, GetClient(fd)->GetNickName(), channelName, fd, " :is already on channel\r\n"); return;}

	// Retrieve the 'Client' object corresponding to the given nickname and store it in 'clt'.
	// After this line, The nickname of the users to should be invited is stored in 'clt'.
	Client *clt = GetClientNick(splitedCmd[1]);

	// 5. ERR_NOSUCHNICK (401)
	if (!clt)
		{senderror(401, splitedCmd[1], fd, " :No such nick/channel\r\n");return;}
	
	// 6. ERR_INVITEONLYCHAN (482)
	if (GetChannel(channelName)->getInviteOnlyMode() && !GetChannel(channelName)->getAdmin(fd))
		{senderror(482,GetChannel(channelName)->getClient(fd)->GetNickName(),splitedCmd[1],fd," :You're not a channel operator\r\n"); return;}

	// 7. ERR_CHANNELISFULL (473)
	if (GetChannel(channelName)->getLimit() && GetChannel(channelName)->getClientsNumber() >= GetChannel(channelName)->getLimit())
		{senderror(473,GetChannel(channelName)->getClient(fd)->GetNickName(),channelName,fd," :Cannot invit to channel (+i)\r\n"); return;}

	// When Invite was successfully, save the 'invite'infomation to clt's member variable by using the function 'AddChannelInvite'.
	clt->AddChannelInvite(channelName);

	// RPL_INVITING (341): Check detail in file (= ./INC/replies.hpp)
	// Save and send a sucess message to the client who invite 'clt'.
	// ex) jason invited Rene
	// 	   = jason receives a confirmation message using this function.
	std::string rep1 = ": 341 "+ GetClient(fd)->GetNickName()+" "+ clt->GetNickName()+" "+ splitedCmd[2]+"\r\n";
	_sendResponse(rep1, fd);

	// Save and send the invitation message to the invited client('clt').
	// ex) jason invtied rene
	//		= rene receives an invitation notification.
	std::string rep2 = ":"+ clt->getHostname() + " INVITE " + clt->GetNickName() + " " + splitedCmd[2]+"\r\n";
	_sendResponse(rep2, clt->GetFd());

	// (Optional)
	// Print debugging to the server.
	if (Flag_PrintInfoServer == 1)
		std::cout << printBackground(LBLU,BWHI,"INITE:") << BLU  << getClientNbrNick(fd) << " invite user: " << clt->GetNickName() << " to channel: " + splitedCmd[2] << RESET << std::endl;
}
