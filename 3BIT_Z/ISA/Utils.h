/**
 * @file    Utils.h
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Useful utilities used by some moduls
 */


#pragma once

// STD/STL
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <regex>


// Local
#include "FeedreaderException.h"



/**
 * @brief Namespace with utilities -- to avoid name collisions
 */
namespace Utils
{

	/** @brief Secured connection (HTTPS) port */
	constexpr int HTTPS_port = 443;
	/** @brief Unsecured connection (HTTP) port */
	constexpr int HTTP_port  = 80;


	/**
	 * @brief Parse filename (extenstion included) from path
	 * 
	 * Code copied from StackExchange and modified
	 * URL: https://stackoverflow.com/questions/8520560/get-a-file-name-from-a-path
	 * Author: alveko (https://stackoverflow.com/users/565368/alveko)
	 * 
	 * @param [in] pathname - Path with filename
	 * @return Filename
	 */
	std::string basename(const std::string& pathname);


	/**
	 * @brief Parse domain from URL
	 *
	 * @param [in] pathname - URL
	 * @return Filename
	 */
	std::string domain(const std::string& url);


	/**
	 * @brief Parse path from URL
	 *
	 * @param [in] pathname - URL
	 * @return Filename
	 */
	std::string path(const std::string& url);


	/**
	 * @brief Parse a given feedfile
	 *
	 * @param [in] feed file
	 * @param [out] feed source URLs
	 */
	void parseFeedfile(const std::string& feedfile, std::vector<std::string>& urls);


	/**
	 * @brief Get and remove port from URL
	 * 
	 * @param [in, out] URL
	 * @return Port or -1
	 */
	int getPort(std::string& url);


	/**
	 * @brief Check whether connection should be secured or not
	 * 
	 * @param [in] URL
	 * @return true = secured
	 */
	bool isConnectionSecured(const std::string& url);
}