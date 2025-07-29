/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hfilipe- <hfilipe-@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 14:56:19 by hfilipe-          #+#    #+#             */
/*   Updated: 2025/07/07 21:48:24 by hfilipe-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Irc.hpp"

int main(int ac, char **av){
    if (ac != 3)
        std::cerr << "Invalid number of arguments!" << std::endl 
    << "Please insert: <executable name> <listening port> <password>" << std::endl;
    
    Irc::channels;

}


/* nc - C <ip> (pode usar hostanem, on terminal) <porta>

c1r3e6

 nc -C localhost 6679 */

 //TCP/6697 for TLS connections.
 //TCP/6667


 //JOIN enters canal if allowed, if it does not exists it will creat new 
 //CREATE CHANELS
 //CHANELL OPERATOR ( list )
// invite-only mode (+i),
//chanel topic
// chanell limit !

//OPERATOR COMANDS :: KICK (Remove someone from channel), MODE"CHANGE MODES",
//INVITE (Invite someone to thhe channel, TOPIC , change the topic)
//OPERATORS HAVE @ before nicj
//others %

//dan and DAN is exacly the same (as user name)
//IRC message is a single line, delimited by a pair of CR ('\r' and ('\n')
//CR (Carriage Return): , LF (Line Feed):

// @id=234AB :dan!d@localhost PRIVMSG #chan :Hey what's up!
//   SERVER    USER IP       <CMD>   <CHANEL> <MESsAGE>

//PASS (first command)
//NICK
//USER

//if success on registation !! 
//RPL_WELCOME"<client> :Welcome to the <networkname> Network, <nick>[!<user>@<host>]"

//RPL_YOURHOST"<client> :Your host is <servername>, running version <version>"

// RPL_CREATED"<client> :This server was created <datetime>"
//RPL_MYINFO "<client> <servername> <version> <available user modes>
 // <available channel modes> [<channel modes with a parameter>]"

 // ERR_PASSWDMISMATCH   
    //"<client> :Password incorrect"

 //ERR_NEEDMOREPARAMS (461) 
  //"<client> <command> :Not enough parameters"

 // ERR_ALREADYREGISTERED
  //"<client> :You may not reregister"
  //Returned when a client tries to change a detail that can only be set during registration
  // (such as resending the PASS or USER after registration). 

//NICK command is used to give the client a nickname or change the previous one.
//ERR_NICKNAMEINUSE (433) 
  //"<client> <nick> :Nickname is already in use"

//ERR_NICKNAMEINUSE If the server receives a NICK command from a client where the desired nickname is already

//nick can not have :no leading # character or other character advertized in CHANTYPES
//&#, =#, #&
//no leading colon (:)
//no ASCII space
//ERR_ERRONEUSNICKNAME (432) 
  //"<client> <nick> :Erroneus nickname"

//  NICK Wiz                  ; Requesting the new nick "Wiz".
//:WiZ NICK Kilroy          ; WiZ changed his nickname to Kilroy.

//USER guest 0 * :Ronnie Reagan

// QUIT command is used to terminate a client’s connection to the server. 
//usage  QUIT :Gone to have lunch

//JOIN
//ERR_NOSUCHCHANNEL (403) 
  //"<client> <channel> :No such channel"

/* ERR_BADCHANNELKEY (475) 
  "<client> <channel> :Cannot join channel (+k)"
Returned to indicate that a JOIN command failed because the channel requires a key and the key was either incorrect or not supplied. 
The text used in the last param of this message may vary. */

/* ERR_BANNEDFROMCHAN (474) 
  "<client> <channel> :Cannot join channel (+b)"
Returned to indicate that a JOIN command failed because the client has been banned from the channel and has not had a ban exception set for them. 
The text used in the last param of this message may vary. */

/* ERR_CHANNELISFULL (471) 
  "<client> <channel> :Cannot join channel (+l)" */

/* ERR_INVITEONLYCHAN (473) 
  "<client> <channel> :Cannot join channel (+i)" */

/*   JOIN #foobar                    ; join channel #foobar.

  JOIN &foo fubar                 ; join channel &foo using key "fubar". */

//  PART
//PART #twilight_zone             ; leave channel "#twilight_zone"

//ERR_NEEDMOREPARAMS (461) 
//  "<client> <command> :Not enough parameters"

/* ERR_NOTONCHANNEL (442) 
  "<client> <channel> :You're not on that channel" */

//

 //Command: TOPIC
  //Parameters: <channel> [<topic>]
/* ERR_NOTONCHANNEL (442) 
  "<client> <channel> :You're not on that channel" */
/*   ERR_NEEDMOREPARAMS (461)

  ERR_NOSUCHCHANNEL (403) 
  "<client> <channel> :No such channel" */
/* 
  ERR_CHANOPRIVSNEEDED (482) 
  "<client> <channel> :You're not channel operator" */
/* f the protected topic mode is set on a channel, 
then clients MUST have appropriate channel permissions to modify the topic of that channel */
/* 
  TOPIC #test :New topic          ; Setting the topic on "#test" to
                                  "New topic".

  TOPIC #test :                   ; Clearing the topic on "#test"

  TOPIC #test                     ; Checking the topic for "#test" */

/*  NAMES #twilight_zone,#42        ; List all visible users on
                                  "#twilight_zone" and "#42".
  NAMES  */

/*   Command: INVITE
  Parameters: <nickname> <channel> */
/*   Only members of the channel are allowed to invite other users. Otherwise,
  the server MUST reject the command with the ERR_NOTONCHANNEL numeric. */
/* Servers MAY reject the command with the ERR_CHANOPRIVSNEEDED numeric. In particular, 
they SHOULD reject it when the channel has invite-only mode set, 
and the user is not a channel operator. */

/* RPL_INVITING (341) 
  "<client> <nick> <channel>"
Sent as a reply to the INVITE command to indicate that the attempt was successful 
and the client with the nickname <nick> has been invited to <channel>. */
//INVITE Wiz #foo_bar    ; Invite Wiz to #foo_bar

// KICK
/*  KICK #Finnish Matthew           ; Command to kick Matthew from
                                   #Finnish

   KICK &Melbourne Matthew         ; Command to kick Matthew from
                                   &Melbourne */


//Command: MODE
//'+': Adds the following mode(s).
//'-': Removes the following mode(s).
//i, t, k, o , l



//PRIVMSG 
 /*  PRIVMSG Angel :yes I'm receiving it !
                                  ; Command to send a message to Angel.

  PRIVMSG %#bunny :Hi! I have a problem! */

//You make a call to the socket() system routine. 
// It returns the socket descriptor, and you communicate through it using the specialized 
// send() and recv() (man send, man recv) socket calls.
