/**
 * @file    main.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    25/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 */

/**
 * TODO:
 */

// STD/STL
#include <iostream>
#include <sstream>
#include <fstream>
#include <tuple>

// Sockets
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

// Local
#include "Arguments.h"
#include "XMLManager.h"
#include "Feed.h"
#include "SSL.h"
#include "Utils.h"
#include "FeedreaderException.h"


/** @brief Allias for buffer of URLs to feedfiles */
using FeedSources      = std::vector<std::string>;
/** @brief Allias for tuple of Domain, path, port number, connection type */
using DomainPathPort   = std::tuple<std::string, std::string, int, bool>;
/** @brief Allias for buffer of pairs of Domain and Path */
using DomainPathBuffer = std::vector<DomainPathPort>;


int main(int argc, char** argv)
{
	// Output stream
	auto& out = std::cout;
	// Error stream
	auto& err = std::cerr;


	///////////////////////////// Parse input args /////////////////////////////
	CArgumentsParser  argsParser;
	SArguments        args;
	try
	{
		args = argsParser.parseArgs(argc, argv);
	}
	catch (CFeedreaderException& e)
	{
		if (e.getErrCode() == I_HELP)
		{
			return 0;
		}
		else
		{
			err << e.what() << std::endl;
			return e.getErrCode();
		}
	}
	////////////////////////////////////////////////////////////////////////////

	

	///////////////////////////// URLs to feed files ///////////////////////////
	FeedSources feedSources;

	if (args.url_flag)   // URL
	{
		feedSources.push_back(args.URL);
	}
	if (args.f_flag)     // Feed file
	{
		Utils::parseFeedfile(args.feedfile, feedSources);
	}
	////////////////////////////////////////////////////////////////////////////



	/////////////////////////// Domain and Path pairs //////////////////////////
	DomainPathBuffer  hostPathBuffer;
	int               port;

	try
	{
		for (auto& it : feedSources)
		{
			port = Utils::getPort(it);

			hostPathBuffer.emplace_back( std::make_tuple( Utils::domain(it), Utils::path(it), port, Utils::isConnectionSecured(it) ) );
		}
	}
	catch (CFeedreaderException& e)
	{
		err << e.what() << std::endl;

		return e.getErrCode();
	}
	////////////////////////////////////////////////////////////////////////////

	for (auto it = hostPathBuffer.begin(); it != hostPathBuffer.end(); ++it)
	{
		try
		{
			////////////////////////// Create secured connection ///////////////////////
			CSSL sslClient(std::get<0>(*it), std::get<2>(*it), args, std::get<3>(*it));

			std::string feeds = sslClient.get(std::get<1>(*it));
			////////////////////////////////////////////////////////////////////////////



			/////////////////////////////// Parse xml feeds ////////////////////////////
			FeedBuffer  feedbuffer;
			CXMLManager xmlManager { feedbuffer };

			xmlManager.parseXML(feeds);
			////////////////////////////////////////////////////////////////////////////



			///////////////////////////////// Print feeds //////////////////////////////
			for (auto it2 = feedbuffer.begin(); it2 != feedbuffer.end(); ++it2)
			{
				if (it2 == feedbuffer.begin())
				{
					it2->printName(out);
				}
				else
				{
					if (it2->printFeed(args, out, *feedbuffer.begin())
						&& it2 != (feedbuffer.end() - 1))
					{
						out << std::endl;
					}
				}
			}
			////////////////////////////////////////////////////////////////////////////


			// Print new line between feed sources
			if (it != (hostPathBuffer.end() - 1))
			{
				out << std::endl;
			}
			
		}
		// Catch errors and keep parsing next source if its non-fatal error
		// Terminate program otherwise
		catch (CFeedreaderException& e)
		{
			err << e.what() << " (Source: " << std::get<0>(*it) << std::get<1>(*it) << ")" << std::endl;

			if (e.getErrCode() == E_RUNTIME_ERROR)
			{
				return E_RUNTIME_ERROR;
			}

			err << std::endl;
		}
	}


	return 0;

}