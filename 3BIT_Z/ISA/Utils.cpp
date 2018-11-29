/**
 * @file    Utils.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Useful utilities used by many moduls
 */


#include "Utils.h"

namespace Utils
{

	std::string basename(const std::string& pathname)
	{
	    return std::move(std::string{std::find_if(pathname.rbegin(), pathname.rend(),
	                        [](char c) { return c == '/'; }).base(),
	            			pathname.end()});
	}


	std::string domain(const std::string& url)
	{
		char domain[1024];
		int  retLen;

		if (url.find("http://") != std::string::npos)                  // http
		{                                                              // eg. "http://techkthub.com/domaintest/cosi.php"
			retLen = sscanf(url.c_str(), "http://%[^/]", domain);
		}
		else if (url.find("https://") != std::string::npos)            // https
		{                                                              // eg. "https://techkthub.com/domaintest/cosi.php"
			retLen = sscanf(url.c_str(), "https://%[^/]", domain);
		}
		else if (url.find("/") != std::string::npos)                   // Without http/https
		{                                                              // eg. "www.techkthub.com/domaintest/cosi.php" | "techkthub.com/domaintest/cosi.php"
			retLen = sscanf(url.c_str(), "%[^/]", domain);
		}
		else                                                           // Wrong URL
		{
			throw CFeedreaderException(std::string("ERROR: Wrong URL (" + url + ")."), E_WRONG_URL);
		}

		// TODO: Check returned value

		return std::move(std::string(domain));
	}


	std::string path(const std::string& url)
	{
		std::string domain = Utils::domain(url);

		std::size_t domainPos = url.find(domain);

		std::string path = std::string(url).substr(domainPos + domain.length());

		return std::move(path);
	}


	void parseFeedfile(const std::string& feedfile, std::vector<std::string>& urls)
	{
		std::ifstream  f { feedfile };
		std::string    line;

		while (std::getline(f, line))
		{
		    if (line.length() >= 1)  // Skip empty lines
		    {
		    	if (line.at(0) == '#')
		    	{
		    		// Skip comment
		    	}
		    	else
		    	{
		    		urls.push_back(line); // Store URL
		    	}
		    }
		}
	}


	int getPort(std::string& url)
	{
		std::regex   portRegex(":\\d{1,5}");
		std::smatch  match;
		std::string  port;
		int          portNumber = -1;
		size_t       pos;

		if (std::regex_search (url, match, portRegex))   // URL contains port number
		{
			port = match.str(0);     // Find port in URL
			pos = url.find(port);    // Save position of port in URL
			port.erase(0, 1);        // Remove the ':'

			portNumber = std::stoi(port);

			url.erase(pos, port.length() + 1);
		}
		else                                              // URL doesnt contain port number
		{
			if (url.find("https://") != std::string::npos)
			{
				portNumber = HTTPS_port;
			}
			else
			{
				portNumber = HTTP_port;
			}
		}

		return portNumber;
	}


	bool isConnectionSecured(const std::string& url)
	{
		return (url.find("https://") != std::string::npos) ? true : false;
	}

}