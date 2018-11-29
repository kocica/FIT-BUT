/**
 * @file    Arguments.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    25/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * This file contains definition of class providing
 * parsing of input arguments.
 */

#include "Arguments.h"

CArgumentsParser::CArgumentsParser()
{

}

CArgumentsParser::~CArgumentsParser()
{

}

SArguments CArgumentsParser::parseArgs(int argc, char *argv[])
{
	int c = 1;

	while (c < argc)
	{
		if (std::string{argv[c]} == "-h")
		{
			printHelp();
			throw CFeedreaderException("HELP", I_HELP);
		}
		else if (std::string{argv[c]} == "-T")
		{
			if (m_args.T_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -T multiple times.");
			}
			m_args.T_flag = true;
		}
		else if (std::string{argv[c]} == "-a")
		{
			if (m_args.a_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -a multiple times.");
			}
			m_args.a_flag = true;
		}
		else if (std::string{argv[c]} == "-u")
		{
			if (m_args.u_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -u multiple times.");
			}
			m_args.u_flag = true;
		}
		else if (std::string{argv[c]} == "-f")
		{
			if ((argc - c) == 1)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -f require an argument.");
			}
			if (m_args.f_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -f multiple times.");
			}
			m_args.f_flag = true;
			m_args.feedfile = std::string{argv[c + 1]};
			c++;
		}
		else if (std::string{argv[c]} == "-c")
		{
			if ((argc - c) == 1)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -c require argument.");
			}
			if (m_args.c_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -c multiple times.");
			}
			m_args.c_flag = true;
			m_args.cerfile = std::string{argv[c + 1]};
			c++;
		}
		else if (std::string{argv[c]} == "-C")
		{
			if ((argc - c) == 1)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -C require argument.");
			}
			if (m_args.C_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -C multiple times.");
			}
			m_args.C_flag = true;
			m_args.certaddr = std::string{argv[c + 1]};
			c++;
		}
		else
		{
			if (argv[c][0] == '-')
			{
				printHelp();
				throw CFeedreaderException("ERROR: Unknown argument.");
			}
			
			if (m_args.url_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: Unknown argument.");
			}
			m_args.url_flag = true;
			m_args.URL = std::string{argv[c]};
		}

		c++;
	}

	/*while ((c = getopt (argc, argv, "f:c:C:hTau")) != -1)
	switch(c)
	{
		case 'f':
			if (m_args.f_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -f multiple times.");
			}
			m_args.f_flag = true;
			m_args.feedfile = std::string{optarg};
			break;

		case 'c':
			if (m_args.c_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -c multiple times.");
			}
			m_args.c_flag = true;
			m_args.cerfile = std::string{optarg};
			break;

		case 'C':
			if (m_args.C_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -C multiple times.");
			}
			m_args.C_flag = true;
			m_args.certaddr = std::string{optarg};
			break;
		
		case 'T':
			if (m_args.T_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -T multiple times.");
			}
			m_args.T_flag = true;
			break;

		case 'a':
			if (m_args.a_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -a multiple times.");
			}
			m_args.a_flag = true;
			break;

		case 'u':
			if (m_args.u_flag)
			{
				printHelp();
				throw CFeedreaderException("ERROR: -u multiple times.");
			}
			m_args.u_flag = true;
			break;

		case 'h':
			printHelp();
			throw CFeedreaderException("HELP", I_HELP);
			break;

		case '?':
		default:
			printHelp();
			throw CFeedreaderException("ERROR: Wrong input.");
			break;
	}

	for (int index = optind; index < argc; index++)
	{
		if (m_args.url_flag)
		{
			printHelp();
			throw CFeedreaderException("ERROR: URL multiple times.");
		}
		m_args.url_flag = true;
		m_args.URL = std::string{argv[index]};
	}*/

	checkArgs();
	//printArgs_debug();

	return m_args; // TODO: std::move() ?
}

void CArgumentsParser::printHelp()
{
	std::cout <<                                                                                             std::endl
	          << "Usage: feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]" << std::endl
			  << "       URL              - Source"                                                       << std::endl
			  << "       -f <feedfile>    - File with sources"                                            << std::endl
			  << "       -c <certfile>    - File with certificates"                                       << std::endl
			  << "       -C <certaddr>    - Folder where certificates are looked for"                     << std::endl
			  << "       -T               - For each feed, prints actualization date"                     << std::endl
			  << "       -a               - For each feed, prints author"                                 << std::endl
			  << "       -u               - For each feed, prints its URL"                                << std::endl
			  <<                                                                                             std::endl;
}

void CArgumentsParser::checkArgs() const
{
	/*if (m_args.url_flag && m_args.f_flag)
	{
		throw CFeedreaderException("FEEDREADER: Both URL & feedfile inserted.");
	}*/
	
	if ((!m_args.url_flag) && (!m_args.f_flag))
	{
		printHelp();
		throw CFeedreaderException("ERROR: Insert URL and/or Feedfile.");
	}
}

void CArgumentsParser::printArgs_debug() const
{
	std::cout << "URL: " << m_args.url_flag    << ": " << m_args.URL      << std::endl;
	std::cout << "f  : " << m_args.f_flag      << ": " << m_args.feedfile << std::endl;
	std::cout << "c  : " << m_args.c_flag      << ": " << m_args.cerfile  << std::endl;
	std::cout << "C  : " << m_args.C_flag      << ": " << m_args.certaddr << std::endl;
	std::cout << "T  : " << m_args.T_flag      << std::endl;
	std::cout << "a  : " << m_args.a_flag      << std::endl;
	std::cout << "u  : " << m_args.u_flag      << std::endl;
}