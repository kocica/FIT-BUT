/**
 * @file    Feed.h
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * This file contains definition of class representing
 * one parsed feed
 */


#pragma once

// STD/STL
#include <iostream>
#include <vector>
#include <fstream>

//Local
#include "Arguments.h"

class CFeed;

/**
 * @brief Buffer of feeds
 */
using FeedBuffer = std::vector<CFeed>;


class CFeed
{
public:
	/** @brief Regular constructor */
	CFeed();

	/** @brief Constructor accepting feed data */
	CFeed(const std::string& title,
		  const std::string& date,
		  const std::vector<std::string>& authors,
		  const std::string& url);

	/** @brief Virtual destructor */
	virtual ~CFeed();

	/**
	 * @brief Printing feed to specified ostream
	 *
	 * @param args[in] Input arguments (decides what will be printed)
	 * @param os [in] Output stream
	 * @return Whether new line should be printed
	 */
	bool printFeed(SArguments& args,
		           std::ostream& os,
				   CFeed& header) const;

	/** @brief Printing name of feed to specified stream */
	void printName(std::ostream& os) const;


protected:

public:

	std::string    m_title,
	               m_actDate,
	               m_url;
	std::vector<std::string> m_authors;
};