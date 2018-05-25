/**
 *
 * @file    ipk-mtrip.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    7/3/2018
 *
 * IPK project 2
 *
 * Bandwidth Measurement
 *   Find out maximal, minimal and regular speeds of end-to-end
 *   connections via UDP protocol.
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>
#include <chrono>
#include <unordered_map>
#include <thread>
#include <cmath>

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
#include <sys/time.h>
#include <time.h>
#include <limits.h>
#include <float.h>

/** Allias for timers */
using Clock = std::chrono::high_resolution_clock;

/** Max size of recvd/sent data */
#define BUFFER_SIZE 2048          // TODO: Do dokumentace

/**
 * Enum with error codes
 */
enum
{
	E_OK            = 0,
	E_BAD_PARAMS    = 1,
	E_SOCKET        = 2,
	E_FORK          = 3,
	E_CHECK         = 4,

};

/**
 * Type of application
 */
enum Type
{
	T_REFLECTOR,
	T_METER
};


/**
 * @brief Class which parses command line arguments and allows other functions to get'em
 *
 */
class CArguments
{
private:


	int 			port;
	int     		size;
	int 			time;

	bool 			p_flag;
	bool 			h_flag;
	bool 			s_flag;
	bool 			t_flag;

	std::string 	host;

	Type 			app_type;
	
public:
	CArguments() : port{0}, p_flag{false}, h_flag{false}, s_flag{false},
				t_flag{false} {}
	
	
	void printHelp()
	{
		std::cout << "IPK Project 2, Filip Kocica <xkocic01>\n\nUsage:" << std::endl;
		std::cout << "\t./ipk-mtrip reflect -p <port_num>" << std::endl;
		std::cout << "\t./ipk-mtrip meter -p <port_num> -h <host> -s <size_of_pkt> -t <timeout>" << std::endl;
		std::cout << "Description:" << std::endl;
		std::cout << "Bandwidth Measurement between two stations in internet network." << std::endl;
		std::cout << " - s <size_of_pkt> Minimal size is " << sizeof(struct timeval) << " Bytes. (For timestamp)" << std::endl;
	}

	/**
	 * @brief Parses command line data and sets the member variables
	 *
	 * @param argc - number of args
	 * @param argv - array of args
	 */
	void parseArgs(int argc, char *argv[])
	{
		int c;
		std::string getoptStr = "p:";

		if (argc == 1)
		{
			std::cerr << "ERROR: Bad params." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}

		// Is it reflector ?
		if (std::string(argv[1]) == "reflect")
		{
			app_type = T_REFLECTOR;
		}
		// Is it meter ?
		else if (std::string(argv[1]) == "meter")
		{
			app_type = T_METER;
			getoptStr = "p:h:s:t:";
		}
		// Nope, its wrong
		else
		{
			std::cerr << "ERROR: Unknown application type." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}

		// Skip meter/reflect argument
		argc--;
		argv++;

		while ((c = getopt (argc, argv, getoptStr.c_str())) != -1)
		switch(c)
		{
			case 'h':
				if (h_flag)
				{
					std::cerr << "ERROR: Parameter twice." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				h_flag = true;
				if (optarg)
				{
					this->host = std::string{optarg};
				}
				break;
			
			case 'p':
				if (p_flag)
				{
					std::cerr << "ERROR: Parameter twice." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				p_flag = true;
				if (std::string{optarg}.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "ERROR: Port has to be number." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				port = std::stoi(std::string{optarg});
				break;

			case 's':
				if (s_flag)
				{
					std::cerr << "ERROR: Parameter twice." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				s_flag = true;
				if (std::string{optarg}.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "ERROR: Size has to be number." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				size = std::stoi(std::string{optarg});
				break;

			case 't':
				if (t_flag)
				{
					std::cerr << "ERROR: Parameter twice." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				t_flag = true;
				if (std::string{optarg}.find_first_not_of("0123456789") != std::string::npos)
				{
					std::cerr << "ERROR: Time has to be number." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
				time = std::stoi(std::string{optarg});
				break;
			
			case '?':
				if (optopt == 'c')
				{
					std::cerr << "ERROR: Unknown option." << std::endl;
					printHelp();
					exit(E_BAD_PARAMS);
				}
			default:
				std::cerr << "ERROR: Unknown option." << std::endl;
				printHelp();
				exit(E_BAD_PARAMS);
		}
		
		if (!p_flag)
		{
			std::cerr << "ERROR: Define port." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}

		else if (app_type == T_METER && (!h_flag || !s_flag || !t_flag))
		{
			std::cerr << "ERROR: Missing some params." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}
		
		else if (this->port < 1024 || this->port > 65535)
		{
			std::cerr << "ERROR: Port out of range <1024; 65535>." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}
		
		for (int index = optind; index < argc; index++)
		{
			std::cerr << "ERROR: Unknown argument." << std::endl;
			printHelp();
			exit(E_BAD_PARAMS);
		}
	}
	

	/* Get functions */

	int getPort() const
	{
		return this->port;
	}

	int getSize() const
	{
		return this->size;
	}

	int getTime() const
	{
		return this->time;
	}
	
	std::string getHost() const
	{
		return this->host;
	}

	Type getType() const
	{
		return this->app_type;
	}
};

/**
 * @brief Signal handle function
 *
 * @param signal - which signal was caught (unsused)
 */
void sigHandler(int signal)
{
	exit(E_OK);
}


/**
 * @brief Class which implements reflector actions
 *
 * Socket is created and binded in constructor due to command line args
 * Reflector is executed by calling runReflector method and in loop
 * receives packets and forwards them back -- simple reflector.
 *
 */
class CReflector
{
private:
	int Socket, SocketAccept, port;
	unsigned int saddrin_len;
	struct sockaddr_in saddrin;
	
public:
	CReflector(int _port) : port(_port)
	{
		if ((Socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		{
			std::cerr << "REFLECTOR: Cannot create socket." << std::endl;
			exit(E_SOCKET);
		}
		
		saddrin.sin_family = AF_INET;                   // Internet protocol
		saddrin.sin_port = htons(port);   				// Set port
		saddrin.sin_addr.s_addr = htonl(INADDR_ANY);    // All IP's

		if (bind(Socket, (struct sockaddr *) &saddrin, sizeof(saddrin)) < 0)
		{
			std::cerr << "REFLECTOR: Cannot bind socket." << std::endl;
			exit(E_SOCKET);
		}
	}
	
	void runReflector()
	{
		std::string read, write, printRead;
		pid_t process_id;
		int nb_rx_bytes = 0;
		char buffer[BUFFER_SIZE];
		
		// Endless loop (Unless signal ends it)
		while (true)
		{
			saddrin_len = sizeof(saddrin);

			// Receive meter message
			nb_rx_bytes = recvfrom(Socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)& saddrin, &saddrin_len);

			// Reflect data back to meter
			if (sendto(Socket, buffer, nb_rx_bytes, 0, (struct sockaddr *)& saddrin, saddrin_len) < 0)
			{
				std::cerr << "REFLECTOR: Failed to answer meter." << std::endl;
				exit(E_SOCKET);
			}
		}
		
		if (close(Socket) < 0)
		{
			std::cerr << "REFLECTOR: Failed to close socket." << std::endl;
			exit(E_SOCKET);
		}
	}
};

/**
 *
 * @brief Class which implements meter actions
 *
 * In constructor socket creation and init is performed
 * then runMeter may be executed, which forks to
 * recv and send processes. Both recvs/sends until they reach
 * time set by user.
 * Then prints statistics.
 *
 */
class CMeter
{
private:
	int Socket, SocketAccept, port;
	unsigned int saddrin_len;
	struct sockaddr_in saddrin;
	struct hostent *hp;
	CArguments args;
	
public:
	CMeter(CArguments &_args) : args(_args)
	{
		if ((Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
		{
			std::cerr << "METER: Cannot create socket." << std::endl;
			exit(E_SOCKET);
		}
		
		memset((char *) &saddrin, 0, sizeof(saddrin));
		saddrin.sin_family = AF_INET;                   	// Internet protocol
		saddrin.sin_port = htons(this->args.getPort());   	// Set port
		
		if ((hp = gethostbyname(this->args.getHost().c_str())) == NULL)
		{
			std::cerr << "METER: Cannot get host name." << std::endl;
			exit(E_SOCKET);
		}

		memcpy(&saddrin.sin_addr, hp->h_addr, hp->h_length);

		/*
		if (inet_aton(hp->h_addr, &saddrin.sin_addr) == 0)
	    {
	        std::cerr << "METER: inet_aton() failed." << std::endl;
			exit(E_SOCKET);
	    }
	    */


		// 10 usec timeout
		/*struct timeval read_timeout;
		read_timeout.tv_sec = 0;
		read_timeout.tv_usec = 10;
		setsockopt(Socket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);*/

		saddrin_len = sizeof(saddrin);
	}
	
	uint64_t recvd = 0, noTimes = 0;
	double sumTime = 0.0;
	bool end = false;

	void runMeter()
	{
		const int 					numberOfMeasurments = 100;
		double 						time = args.getTime();
		char 						*buffer = static_cast<char *>(calloc(args.getSize(), 1));
		struct timeval 				tv, tvalBefore, tvalAfter, act_tv;
		unsigned int 				loopLen = (time*1000000L)/numberOfMeasurments;
		unsigned int 				sleepDuration = loopLen/numberOfMeasurments;
		unsigned int 				decreaseSleepDuration = sleepDuration/(numberOfMeasurments/10);
		int 						nb_rx_bytes = 0;
		uint64_t					sent = 0, recvdTmp;
		double 						avgSpeed = .0, minSpeed = DBL_MAX,
									maxSpeed = DBL_MIN, actSpeed = .0, variance = .0, t = .0, diff;
		uint32_t 					avgSpeedCounter = 0;
		std::thread 				recvThread(&CMeter::recvMeter, this);


		for (int i = 0; i < numberOfMeasurments; i++)
		{	
			// Start new measurment
			sent = recvd = 0;
			gettimeofday (&tvalBefore, NULL);
			do
			{
				// Set ts to pckt
				gettimeofday(&tv,NULL);
				memcpy(buffer, &tv, sizeof(struct timeval));

				// Send msg to reflextor
				if (sendto(Socket, buffer, args.getSize(), 0, (struct sockaddr *)& saddrin, saddrin_len) < 0)
				{
					std::cerr << "METER: Function sendto failed." << std::endl;
					exit(E_SOCKET);
				}
				sent++;

				// sleep
				usleep(sleepDuration);
				gettimeofday (&tvalAfter, NULL);
			} while ((((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L
           		+tvalAfter.tv_usec) - tvalBefore.tv_usec) < (loopLen));
			recvdTmp = recvd;

			std::cout << "Sleep duration in us: " << sleepDuration << std::endl;
			std::cout << "Sent:                 " << sent << std::endl;
			std::cout << "Recv:                 " << recvdTmp << std::endl;

          	// Calculate the speed
			if (recvdTmp >= (sent>10?sent-10:sent))
			{
				if (sleepDuration > 0)
				{
					sleepDuration -= decreaseSleepDuration;
				}
				std::cout << "Speed up -> decrease sleep duration" << std::endl;
			}
			else
			{
				sleepDuration += decreaseSleepDuration;
				std::cout << "Slow down -> increase sleep duration" << std::endl;
			}

			if (recvdTmp == 0)
			{
				std::cout << std::endl;
				continue;
			}

			std::cout << std::endl;

			//            PER SECOND                              IN MEGABITS
			actSpeed = (((1000000.0/loopLen)*sent*args.getSize())/125000.0);

			// Standard deviation
			// Inspired by:
			//  - https://stackoverflow.com/questions/33268513/calculating-standard-deviation-variance-in-c?utm_medium=organic&utm_source=google_rich_qa&utm_campaign=google_rich_qa
			if (avgSpeedCounter != 0)
			{
				t += actSpeed;
          		diff = ((avgSpeedCounter + 1) * actSpeed) - t;
          		variance += (diff * diff) / ((i + 1.0) *i);
          	}
          	else
          	{
          		variance = .0;
          		t = avgSpeed;
          	}

          	avgSpeedCounter++;

			// Actualize avg/min/max speeds
			if (actSpeed > maxSpeed)
			{
				maxSpeed = actSpeed;
			}
			if (actSpeed < minSpeed)
			{
				minSpeed = actSpeed;
			}
			avgSpeed += actSpeed;
		}

		std::cout << "=============================================" << std::endl;
		std::cout.precision(10);
		std::cout << "Avg speed:            " << avgSpeed/avgSpeedCounter << " Mb/s" << std::endl;
		std::cout.precision(10);
		std::cout << "Min speed:            " << minSpeed << " Mb/s" << std::endl;
		std::cout.precision(10);
		std::cout << "Max speed:            " << maxSpeed << " Mb/s" << std::endl;
		std::cout.precision(10);
		std::cout << "STDEV    :            " << std::sqrt(variance/(avgSpeedCounter-1)) << std::endl;
		std::cout.precision(10);
 		std::cout << "RTT      :            " << (sumTime/noTimes)*1000 << " ms" << std::endl;
 		std::cout << "=============================================" << std::endl;

		end = true;
		free(buffer);
		exit(E_OK);
	}

	void recvMeter()
	{
		char *buffer = static_cast<char *>(calloc(args.getSize(), 1));
		struct timeval tv, act_tv;
		int nb_rx_bytes = 0;

		while (!end)
		{
			nb_rx_bytes = recvfrom(Socket, buffer, args.getSize(), 0, (struct sockaddr *)& saddrin, &saddrin_len);
			recvd++;

			/*if (nb_rx_bytes < 0 && errno == EWOULDBLOCK)
			{
				std::cout << "Zahazuje na: " << sleepDuration << std::endl;
			}*/

			// Get ts from pckt
			memcpy(&tv, buffer, sizeof(struct timeval));

			// Get act time
			gettimeofday(&act_tv,NULL);

			// Avg time
			sumTime += (((act_tv.tv_sec - tv.tv_sec) * 1000000L + act_tv.tv_usec) - tv.tv_usec)/1000000.0;
			noTimes++;
		}
	}
};

int main (int argc, char *argv[])
{
	signal(SIGTERM, sigHandler);
	signal(SIGINT, sigHandler);
	
	CArguments args;
	args.parseArgs(argc, argv);

	if (args.getType() == T_METER)
	{
		if (args.getSize() < (sizeof(struct timeval)))
		{
			std::cerr << "METER: - s <size_of_pkt> Minimal size is " << sizeof(struct timeval) << " Bytes. (For timestamp)" << std::endl;
			exit(E_BAD_PARAMS);
		}
		CMeter meter(args);
		meter.runMeter();
	}
	else
	{
		CReflector reflector(args.getPort());
		reflector.runReflector();
	}

	return E_OK;
}