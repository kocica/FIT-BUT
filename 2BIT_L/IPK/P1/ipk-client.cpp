/**
 *
 * @file    ipk-client.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    23/2/2018
 *
 * IPK project 1
 *
 * Client <-> Server communication
 * Grab data about users and display them.
 */

#include <iostream>
#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <locale.h>
#include <pwd.h>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <getopt.h>

#include "base64.hpp"


const int BUFFER_SIZE = 1024;

enum
{
	E_OK            = 0,
	E_BAD_PARAMS    = 1,
	E_SOCKET        = 2,
	E_SERVER        = 3
};

class CArguments
{
private:
	int port, flags;
	enum
	{
		n_flag = 1,
		f_flag = 2,
		l_flag = 4,
		h_flag = 8,
		p_flag = 16
	};
	std::string login, host;
	
public:
	CArguments() : port{0}, flags{0} {}
	
	
	void parseArgs(int argc, char *argv[])
	{
		int c;

		while ((c = getopt (argc, argv, "n:f:h:p:l::")) != -1)
		switch(c)
		{
			case 'n':
				if (this->flags & n_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->flags |= n_flag;
				this->login = std::string{optarg};
				break;
			
			case 'f':
				if (this->flags & f_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->flags |= f_flag;
				this->login = std::string{optarg};
				break;
			
			case 'l':
				if (this->flags & l_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->flags |= l_flag;
				if (optarg)
				{
					this->login = std::string{optarg}; // TODO: Optional login
				}
				break;

			case 'h':
				if (this->flags & h_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->flags |= h_flag;
				if (optarg)
				{
					this->host = std::string{optarg};
				}
				break;
			
			case 'p':
				if (this->flags & p_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->flags |= p_flag;
				if (std::string{optarg}.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "CLIENT: Port has to be number." << std::endl;
					exit(E_BAD_PARAMS);
				}
				port = std::stoi(std::string{optarg});
				break;
			
			case '?':
				if (optopt == 'c')
				{
					std::cerr << "CLIENT: Unknown option." << std::endl;
					exit(E_BAD_PARAMS);
				}
			default:
				std::cerr << "SERVER: Unknown option." << std::endl;
				exit(E_BAD_PARAMS);
		}
		
		if ((flags & p_flag) == 0 || (flags & h_flag) == 0)
		{
			std::cerr << "CLIENT: Define port and host." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		else if (this->port < 1024 || this->port > 65535)
		{
			std::cerr << "CLIENT: Port out of range <1024; 65535>." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		else if (((flags & n_flag) && (flags & f_flag)) ||
				((flags & n_flag) && (flags & l_flag)) ||
				((flags & f_flag) && (flags & l_flag)))
		{
			std::cerr << "CLIENT: Only one of the [n|f|l] options may be used." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		else if (!((flags & n_flag) ||
				(flags & f_flag) ||
				(flags & l_flag)))
		{
			std::cerr << "CLIENT: At least one of the [n|f|l] options have to be used." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		for (int index = optind; index < argc; index++)
		{
			if (index == (argc - 1) && (flags & l_flag) && this->login == "")
			{
				login = std::string{argv[index]};
				break;
			}
			std::cerr << "CLIENT: Unknown argument." << std::endl;
			exit(E_BAD_PARAMS);
		}
	}
	
	int getPort() const
	{
		return this->port;
	}
	
	std::string getHost() const
	{
		return this->host;
	}

	std::string getLogin() const
	{
		return this->login;
	}
	
	int getFlags() const
	{
		return this->flags;
	}
};

class CSocket
{
private:
	int Socket, SocketAccept, port, saddrin_len;
	struct sockaddr_in saddrin;
	struct hostent *hp;
	CArguments args;
	
public:
	CSocket(CArguments &_args) : args(_args)
	{
		if ((Socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << "CLIENT: Cannot create socket." << std::endl;
			exit(E_SOCKET);
		}
		
		saddrin.sin_family = PF_INET;                   	// Internet protocol
		saddrin.sin_port = htons(this->args.getPort());   	// Set port
		
		if ((hp = gethostbyname(this->args.getHost().c_str())) == NULL)
		{
			std::cerr << "CLIENT: Cannot get host name." << std::endl;
			exit(E_SOCKET);
		}

		memcpy(&saddrin.sin_addr, hp->h_addr, hp->h_length);
		if (connect(Socket, (struct sockaddr *) &saddrin, sizeof(saddrin)) < 0)
		{
			std::cerr << "CLIENT: Cannot connect." << std::endl;
			exit(E_SOCKET);
		}
	}
	
	void runClient()
	{
		std::string read, toSend, printRead, encodedData, decodedData, tmp;
		char buffer[BUFFER_SIZE];
		int nb_rx_bytes;
		
		// Data to send to server
		toSend += std::string("IPK-xkocic01-protocol\n") + std::to_string(args.getFlags())
						+ std::string("\n") + args.getLogin() + std::string("\n");
		
		// Encode data
		encodedData = base64_encode(reinterpret_cast<unsigned char *>(const_cast<char *>(&toSend.c_str()[0])),
						toSend.length());
	
		// Send msg to server
		if (send(Socket, encodedData.c_str(), encodedData.length(), 0) < 0)
		{
			std::cerr << "CLIENT: Cannot connect." << std::endl;
			exit(E_SOCKET);
		}
		
		// Get msg from server
		while ((nb_rx_bytes = recv(Socket, buffer, BUFFER_SIZE, 0)) > 0)
		{
			// Decode data
			tmp = std::string(buffer, nb_rx_bytes);
			decodedData = base64_decode(tmp);

			if (decodedData == "IPK-xkocic01-ERROR")
			{

				std::cerr << "CLIENT: SERVER: Unknown login." << std::endl;
				exit(E_SERVER);
			}

			// Append read data
			read += decodedData;

			// If we read all data end cycle
			if(nb_rx_bytes < (BUFFER_SIZE - 1))
			{
				break;
			}
		}

		// Print data received from server
		std::istringstream readData(read.c_str());
		while (std::getline(readData, printRead))
		{
			std::cout << printRead << std::endl;
		}
	}
};

void sigHandler(int signal) 
{
	exit(E_OK);
}

int main (int argc, char *argv[])
{	
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	signal(SIGCHLD, SIG_IGN);
	
	CArguments args;
	args.parseArgs(argc, argv);
	
	CSocket client(args);
	client.runClient();
	
	return E_OK;
}