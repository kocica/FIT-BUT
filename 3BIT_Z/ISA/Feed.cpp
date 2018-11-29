/**
 * @file    Feed.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * This file contains definition of class representing
 * one parsed feed
 */

#include "Feed.h"

CFeed::CFeed()
{

}

CFeed::CFeed(const std::string& title,
		     const std::string& date,
		     const std::vector<std::string>& authors,
		     const std::string& url)
: m_title { title }
, m_actDate { date }
, m_authors { authors }
, m_url { url }
{

}

CFeed::~CFeed()
{

}

void CFeed::printName(std::ostream& os) const
{
	os << "*** " << m_title << " ***" << std::endl;
}
	
bool CFeed::printFeed(SArguments& args,
		              std::ostream& os,
				      CFeed& header) const
{
	                 os <<                    (m_title.empty()   ? "Neznámý název"   : m_title)   << std::endl;
	if (args.u_flag) os << "URL: "         << (m_url.empty()     ? ""                : m_url)     << std::endl;
	if (args.T_flag) os << "Aktualizace: " << (m_actDate.empty() ? ""                : m_actDate) << std::endl;    // TODO: Order of URL/Actualization/Author
	
	// Print authors
	for (auto& it : m_authors)
	{
		if (args.a_flag) os << "Autor: "       << it << std::endl;
	}

	// No authors
	if (m_authors.empty())
	{
		if (args.a_flag) os << "Autor: "       << (header.m_authors.size() > 0 ? header.m_authors.at(0) : "") << std::endl;
	}

	return (args.a_flag || args.T_flag || args.u_flag);
}