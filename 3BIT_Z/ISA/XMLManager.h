/**
 * @file    XMLManager.h
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Provides parsing of XML feed file
 */


#pragma once

// STD/STL
#include <memory>
#include <string.h>
#include <vector>

// libxml2
#include <libxml/xmlreader.h>

// Local
#include "Feed.h"
#include "FeedreaderException.h"


// Atom file format
const char * const ATOM_ENTRY = "entry";
const char * const ATOM_UPDT  = "updated";
const char * const ATOM_FEED  = "feed";
const char * const ATOM_NAME  = "name";

// RSS 2.0 file format
const char * const RSS2_ENTRY  = "item";
const char * const RSS2_UPDT   = "pubDate";
const char * const RSS2_FEED   = "rss";
const char * const RSS2_NAME   = "name";

// RSS 0.91 / 1.0 file format
const char * const RSS1_ENTRY  = "item";
const char * const RSS1_UPDT   = "date";
const char * const RSS1_FEED   = "RDF";
const char * const RSS1_NAME   = "creator";


class CXMLManager
{
public:
	/** @brief We need feed buffer */
	CXMLManager() = delete;
	/** @brief Constructor accepting feed buffer */
	CXMLManager(FeedBuffer&);
	/** @brief Virtual destructor */
	virtual ~CXMLManager();

	/**
	 * @brief Method parsing feedfile in XML format
	 *
	 * @param feedfile [in] Feedfile content
	 */
	void parseXML(const std::string& feedfile);

protected:

	// (Modified) Function copied from examples of libxml2:
	//     Link:   http://xmlsoft.org/examples/reader2.c
	//     Author: Daniel Veillard
	virtual void streamFile(const char *data, size_t size);

	/**
	 * @brief Removes HTTP data from answer & leave only XML
	 * */
	void removeHttpData(const std::string& data, std::string& xmlData);

	/**
	 * @brief Get element attribute value
	 * */
	void readAttribText(xmlTextReaderPtr reader, const std::string& attrib, std::string& attrValue);

	/**
	 * @brief Read element text
	 * Code copied from StackExchange and modified
	 * URL: https://stackoverflow.com/questions/52562040/missed-element-values-when-parsing-xml-file-using-libxml2
	 * Author: Remy Lebeau (https://stackoverflow.com/users/65863/remy-lebeau)
	 */
	void readText(xmlTextReaderPtr reader, std::string &text/*, const xmlChar *tagName */);

	/** @brief Reed header */
	void readHeader(xmlTextReaderPtr reader, CFeed &entry);

	/**
	 * @brief Reed feed entry
	 * Code copied from StackExchange and modified
	 * URL: https://stackoverflow.com/questions/52562040/missed-element-values-when-parsing-xml-file-using-libxml2
	 * Author: Remy Lebeau (https://stackoverflow.com/users/65863/remy-lebeau)
	 */
	void readFeedEntry(xmlTextReaderPtr reader, CFeed &entry);

	/**
	 * @bried Reed Feed
	 * Code copied from StackExchange and modified
	 * URL: https://stackoverflow.com/questions/52562040/missed-element-values-when-parsing-xml-file-using-libxml2
	 * Author: Remy Lebeau (https://stackoverflow.com/users/65863/remy-lebeau)
	 */
	void readFeed(xmlTextReaderPtr reader);

private:
	/** @brief Says whether header was already parsed */
	bool               m_header;

	/** @brief Where feeds are stored */
	FeedBuffer&        m_feedBuffer;

	/** @brief Entry specific for RSS/Atom format, initialized
	 *         for the sake of bad feedfile
	 */
	const char *       m_feed  = "";
	const char *       m_entry = "";
	const char *       m_updt  = "";
	const char *       m_fname = "";
};