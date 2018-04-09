/**
 *
 * @file    ipk-server.cpp
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
	E_FORK			= 3,
	E_CHECK         = 4,
	E_LOGIN         = 5
};

class CSocket
{
private:
	int Socket, SocketAccept, port, saddrin_len;
	struct sockaddr_in saddrin;
	
	enum
	{
		n_flag = 1,
		f_flag = 2,
		l_flag = 4,
		h_flag = 8,
		p_flag = 16
	};
	
public:
	CSocket(int _port) : port(_port)
	{
		if ((Socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
		{
			std::cerr << "SERVER: Cannot create socket." << std::endl;
			exit(E_SOCKET);
		}
		
		saddrin.sin_family = PF_INET;                   // Internet protocol
		saddrin.sin_port = htons(port);   				// Set port
		saddrin.sin_addr.s_addr = INADDR_ANY;           // All IP's
		
		/// Get socket address
		if (bind(Socket, (struct sockaddr *) &saddrin, sizeof(saddrin)) < 0)
		{
			std::cerr << "SERVER: Cannot bind socket." << std::endl;
			exit(E_SOCKET);
		}

		// Up to 10 pending connections
		if (listen(Socket, 10))
		{ 
			std::cerr << "SERVER: Cannot listen socket." << std::endl;
			exit(E_SOCKET);
		}
		
		saddrin_len = sizeof(saddrin);
	}
	
	void runServer()
	{
		std::string read, write, printRead, flags, login, tmp, decodedData, encodedData;
		pid_t process_id;
		int nb_rx_bytes = 0;
		char buffer[BUFFER_SIZE];
		
		// Endless loop (Unless signal ends it)
		while (true)
		{
			if ((SocketAccept = accept(Socket, (struct sockaddr *) &saddrin, (socklen_t*) &saddrin_len)) < 0)
			{
				std::cerr << "SERVER: Failed to accept socket." << std::endl;
				exit(E_SOCKET);
			}
			
			// Double process
			process_id = fork();
			
			// Failed to fork
			if (process_id < 0)
			{
				std::cerr << "SERVER: Failed to fork process." << std::endl;
				exit(E_FORK);
			}
			// Child process
			else if (process_id == 0)
			{
				// Receive client message
				while ((nb_rx_bytes = recv(SocketAccept, buffer, BUFFER_SIZE, 0)) > 0)
				{
					// Append read data
					tmp = std::string(buffer, nb_rx_bytes);
					decodedData = base64_decode(tmp);
					read += decodedData;
					
					// If we read all data end cycle
					if(nb_rx_bytes < (BUFFER_SIZE - 1))
					{
						break;
					}
				}
				
				// Parse packet
				std::istringstream readData(read.c_str());
				
				// Check protocol validity
				std::getline(readData, printRead);
				if(printRead.compare("IPK-xkocic01-protocol") != 0)
				{
					std::cerr << "SERVER: Failed to check protocol." << std::endl;
					close(SocketAccept);
					exit(E_CHECK);
				}
				
				// Read flags
				std::getline(readData, flags);

				// Read login
				std::getline(readData, login);
				
				// Get user data from passwd
				write = passwd(login, flags);
				
				// Empty return means wrong login
				if (write.empty())
				{
					std::cerr << "SERVER: Unknown login." << std::endl;
					write = "IPK-xkocic01-ERROR";
					encodedData = base64_encode(reinterpret_cast<unsigned char *>(const_cast<char *>(&write.c_str()[0])),
						write.length());
					if (send(SocketAccept, encodedData.c_str(), encodedData.length(), 0) < 0)
					{
						std::cerr << "SERVER: Failed to answer client." << std::endl;
						exit(E_SOCKET);
					}
					close(SocketAccept);
					exit(E_LOGIN);
				}
				
				encodedData = base64_encode(reinterpret_cast<unsigned char *>(const_cast<char *>(&write.c_str()[0])),
						write.length());
				
				// Send user data to client
				if (send(SocketAccept, encodedData.c_str(), encodedData.length(), 0) < 0)
				{
					std::cerr << "SERVER: Failed to answer client." << std::endl;
					exit(E_SOCKET);
				}
				
				// Close socket
				if(close(SocketAccept) < 0)
				{
					std::cerr << "SERVER: Failed to close socket." << std::endl;
					exit(E_SOCKET);
				}
				
				// Clear strings
				write.clear(); read.clear();
				break;
			}
		}
		
		if (close(Socket) < 0)
		{
			std::cerr << "SERVER: Failed to close socket." << std::endl;
			exit(E_SOCKET);
		}
	}
	
	std::string passwd(std::string &login, std::string &flags)
	{
		struct passwd *pswd;
		int iflags = std::stoi(flags);
		std::string toReturn, loginUpper, passwdUpper;
		
		loginUpper = login;
		std::transform(loginUpper.begin(), loginUpper.end(), loginUpper.begin(), ::toupper);
		
		// Init passwd
		setpwent();
		
		// List passwd
		while ((pswd = getpwent()) != NULL)
		{
			passwdUpper = std::string(pswd->pw_name);
			std::transform(passwdUpper.begin(), passwdUpper.end(), passwdUpper.begin(), ::toupper);
			
			if (iflags & n_flag)
			{
				if (loginUpper == passwdUpper)
				{
					toReturn += std::string(pswd->pw_gecos) + "\n";
				}
			}
			else if (iflags & f_flag)
			{
				if (loginUpper == passwdUpper)
				{
					toReturn += std::string(pswd->pw_dir) + "\n";
				}
			}
			else if (iflags & l_flag)
			{
				// Login is empty -> pick all users
				if (login.empty())
				{
					toReturn += std::move(std::string(pswd->pw_name)) + "\n";
				}
				// Select only user with prefix
				else
				{
					// Is it prefix ?
					if (passwdUpper.find(loginUpper) == 0)
					{
						toReturn += std::move(std::string(pswd->pw_name)) + "\n";
					}
				}
			}
			else
			{
				std::cerr << "SERVER: Unknown flag." << std::endl;
				exit(E_CHECK);
			}

		}
		return toReturn;
	}
};


class CArguments
{
private:
	int port;
	bool p_flag;
	
public:
	CArguments() : port(0), p_flag(false) {}
	
	void parseArgs(int argc, char *argv[])
	{
		int c;

		while ((c = getopt (argc, argv, "p:")) != -1)
		switch(c)
		{
			case 'p':
				if (this->p_flag)
				{
					std::cerr << "CLIENT: Parameter twice." << std::endl;
					exit(E_BAD_PARAMS);
				}
				this->p_flag = true;
				if (std::string{optarg}.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "SERVER: Port has to be number." << std::endl;
					exit(E_BAD_PARAMS);
				}
				port = std::stoi(std::string{optarg});
				break;
			
			case '?':
				if (optopt == 'c')
				{
					std::cerr << "SERVER: Unknown option." << std::endl;
					exit(E_BAD_PARAMS);
				}
			default:
				std::cerr << "SERVER: Unknown option." << std::endl;
				exit(E_BAD_PARAMS);
		}
		
		if (!p_flag)
		{
			std::cerr << "SERVER: Define port." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		else if (this->port < 1024 || this->port > 65535)
		{
			std::cerr << "SERVER: Port out of range <1024; 65535>." << std::endl;
			exit(E_BAD_PARAMS);
		}
		
		for (int index = optind; index < argc; index++)
		{
			std::cerr << "CLIENT: Unknown argument." << std::endl;
			exit(E_BAD_PARAMS);
		}
	}
	
	int getPort() const
	{
		return this->port;
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
	
	CSocket server{args.getPort()};
	server.runServer();
	
	return E_OK;
}