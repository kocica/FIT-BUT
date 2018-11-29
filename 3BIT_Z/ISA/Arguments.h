/**
 * @file    Arguments.h
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    25/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
*
 * This file contains definition of class providing
 * parsing & check of input arguments.
 */


#pragma once

// STD
#include <unistd.h>
#include <iostream>

// Local
#include "FeedreaderException.h"



/**
 * @brief POD (Plain old data) filled with parsed arguments
 */
struct SArguments
{
public:
	std::string URL,
	            feedfile,
	            cerfile,
	            certaddr;
	bool        url_flag,
	            f_flag,
	            c_flag,
	            C_flag,
	            T_flag,
	            a_flag,
	            u_flag;
};


/**
 * @brief Class parsing input arguments & provides check if they are valid
 *        Usage: feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]
 *        TODO: https://wis.fit.vutbr.cz/FIT/st/phorum-msg-show.php?id=51586
 */
class CArgumentsParser
{	
public:
	/** @brief Do nothing, succesfully */
	explicit CArgumentsParser();
	/** @brief Virtual destructor */
	virtual ~CArgumentsParser();
	
	/**
	 * @brief Parsing input arguments using getopt
	 *
	 * @param argc [in] Number of arguments
	 * @param argv [in] Argument values
	 * @return Parsed arguments
	 */
	SArguments parseArgs(int argc, char *argv[]);

	/**
	 * @brief Pefrorm arguments check after parsing
	 */
	void checkArgs() const;

	/**
	 * @brief Prints help/usage to standard output
	 */
	static void printHelp();

protected:
	/** @brief Debug -- TODO: Delete */
	void printArgs_debug() const;

private:
	/** @brief Parsed arguments */
	SArguments m_args = {};
};