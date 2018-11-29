/**
 * @file    XMLManager.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Provides parsing of XML feed file
 */

#include "XMLManager.h"

CXMLManager::CXMLManager(FeedBuffer& feedbuffer) : m_feedBuffer{ feedbuffer }, m_header { true }
{

}

CXMLManager::~CXMLManager()
{
 
}

void CXMLManager::readFeed(xmlTextReaderPtr reader)
{
    if (xmlTextReaderIsEmptyElement(reader))
    {
        return;
    }

    int depth = xmlTextReaderDepth(reader);
    int ret;

    while ((ret = xmlTextReaderRead(reader)) == 1)
    {
        switch (xmlTextReaderNodeType(reader))
        {
            case XML_READER_TYPE_ELEMENT:
            {
                if (m_header)
                {
                    CFeed header;
                    readHeader(reader, header);
                    m_feedBuffer.push_back(header);
                    m_header = false;
                }
                
                if (xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST m_entry))
                {
                    CFeed entry;
                    readFeedEntry(reader, entry);
                    m_feedBuffer.push_back(entry);
                }
                break;
            }

            case XML_READER_TYPE_END_ELEMENT:
            {
                if ((xmlTextReaderDepth(reader) == depth)
                    /*&& xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST "feed")*/)
                {
                    return;
                }
                break;
            }
        }
    }

    if (ret == -1)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }
}

void CXMLManager::readFeedEntry(xmlTextReaderPtr reader, CFeed &entry)
{
    if (xmlTextReaderIsEmptyElement(reader))
    {
        return;
    }
    
    int depth = xmlTextReaderDepth(reader);
    int ret;

    do
    {
        switch (xmlTextReaderNodeType(reader))
        {
            case XML_READER_TYPE_ELEMENT:
            {
                const xmlChar *name = xmlTextReaderConstLocalName(reader);

                if (xmlStrEqual(name, BAD_CAST "title"))
                {
                    readText(reader, entry.m_title);
                }
                else if (xmlStrEqual(name, BAD_CAST ATOM_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS1_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS2_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST ATOM_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS1_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS2_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST "link"))
                {
                    if (!strcmp(m_feed, ATOM_FEED))
                    {
                        readAttribText(reader, "href", entry.m_url);
                    }
                    else if (!strcmp(m_feed, RSS1_FEED))
                    {
                        readText(reader, entry.m_url);
                    }
                    else if (!strcmp(m_feed, RSS2_FEED))
                    {
                        readText(reader, entry.m_url);
                    }
                }

                break;
            }

            case XML_READER_TYPE_END_ELEMENT:
            {
                if ((xmlTextReaderDepth(reader) == depth)
                    /*&& xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST "entry")*/)
                {
                    return;
                }
                break;
            }
        }
    } while ((ret = xmlTextReaderRead(reader)) == 1);

    if (ret == -1)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }
}

void CXMLManager::readHeader(xmlTextReaderPtr reader, CFeed &entry)
{
    int ret;
    int depth = xmlTextReaderDepth(reader);

    do
    {
        switch (xmlTextReaderNodeType(reader))
        {
            case XML_READER_TYPE_ELEMENT:
            {
                const xmlChar* name = xmlTextReaderConstLocalName(reader);

                if (xmlStrEqual(name, BAD_CAST m_entry))
                {
                    return;
                }
                else if (xmlStrEqual(name, BAD_CAST "title") && (depth == xmlTextReaderDepth(reader)
                || xmlTextReaderDepth(reader) == depth + 1))
                {
                    readText(reader, entry.m_title);
                }
                else if (xmlStrEqual(name, BAD_CAST ATOM_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS1_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS2_UPDT))
                {
                    readText(reader, entry.m_actDate);
                }
                else if (xmlStrEqual(name, BAD_CAST ATOM_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS1_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST RSS2_NAME))
                {
                    std::string author;
                    readText(reader, author);
                    entry.m_authors.push_back(author);
                }
                else if (xmlStrEqual(name, BAD_CAST "link"))
                {
                    if (!strcmp(m_feed, ATOM_FEED))
                    {
                        readAttribText(reader, "href", entry.m_url);
                    }
                    else if (!strcmp(m_feed, RSS1_FEED))
                    {
                        readText(reader, entry.m_url);
                    }
                    else if (!strcmp(m_feed, RSS2_FEED))
                    {
                        readText(reader, entry.m_url);
                    }
                }

                break;
            }
        }
    } while ((ret = xmlTextReaderRead(reader)) == 1);

    if (ret == -1)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }
}

void CXMLManager::readText(xmlTextReaderPtr reader, std::string &text/*, const xmlChar *tagName */)
{
    text.clear();

    if (xmlTextReaderIsEmptyElement(reader))
    {
        return;
    }

    int depth = xmlTextReaderDepth(reader);
    int ret;

    while ((ret = xmlTextReaderRead(reader)) == 1)
    {
        switch (xmlTextReaderNodeType(reader))
        {
            case XML_READER_TYPE_TEXT:
            {
                const xmlChar *value = xmlTextReaderConstValue(reader);
                text += reinterpret_cast<const char*>(value);
                break;
            }

            case XML_READER_TYPE_END_ELEMENT:
            {
                if ((xmlTextReaderDepth(reader) == depth)
                    /*&& xmlStrEqual(name, tagName)*/)
                {
                    return;
                }
                break;
            }
        }
    }

    if (ret == -1)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }
}

void CXMLManager::readAttribText(xmlTextReaderPtr reader, const std::string& attrib, std::string& attrValue)
{
    xmlNodePtr node = xmlTextReaderCurrentNode(reader);
    if (xmlTextReaderNodeType(reader) == 1 && node && node->properties)
    {
        xmlAttr* attribute = node->properties;
        while (attribute && attribute->name && attribute->children)
        {
            xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
            if (!xmlStrcmp(attribute->name, (xmlChar *) attrib.c_str()))
            {
                attrValue = reinterpret_cast<const char*>(value);
                return;
            }
            xmlFree(value);
            attribute = attribute->next;
        }
    }
}

void CXMLManager::streamFile(const char *data, size_t size)
{
    xmlTextReaderPtr reader = xmlReaderForMemory(data, size, NULL, NULL,
                XML_PARSE_DTDATTR |  /* default DTD attributes */
                XML_PARSE_NOENT);    /* substitute entities */

    if (!reader)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }

    int ret;

    while ((ret = xmlTextReaderRead(reader)) == 1)
    {
        // Atom feed file
        if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
            && xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST ATOM_FEED))
        {
            m_feed  = ATOM_FEED; 
            m_entry = ATOM_ENTRY;
            m_updt  = ATOM_UPDT;
            m_fname = ATOM_NAME;
            readFeed(reader);
        }
        // RSS 1.0 feed file
        else if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
            && xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST RSS1_FEED))
        {
            m_feed  = RSS1_FEED;
            m_entry = RSS1_ENTRY;
            m_updt  = RSS1_UPDT;
            m_fname = RSS1_NAME;
            readFeed(reader);
        }
        // RSS 2.0 feed file
        else if ((xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
            && xmlStrEqual(xmlTextReaderConstLocalName(reader), BAD_CAST RSS2_FEED))
        {
            m_feed  = RSS2_FEED; 
            m_entry = RSS2_ENTRY;
            m_updt  = RSS2_UPDT;
            m_fname = RSS2_NAME;
            readFeed(reader);
        }
    }

    xmlFreeTextReader(reader);
    xmlCleanupParser(); 

    if (ret == -1)
    {
        throw CFeedreaderException("ERROR: Failed to read XML.", E_WRONG_XML);
    }
}

void CXMLManager::removeHttpData(const std::string& data, std::string& xmlData)
{
    size_t XMLstart = data.find("<?xml");

    if (XMLstart == std::string::npos)
    {
        XMLstart = data.find("<rdf");
    }

    if (XMLstart == std::string::npos)
    {
        throw CFeedreaderException("ERROR: Source not found, forbidden by server or has incorrect format.", E_WRONG_XML);
    }

    xmlData = data.substr(XMLstart);
}

void CXMLManager::parseXML(const std::string& feedfile)
{
    // Init libxml2
	LIBXML_TEST_VERSION

    // Get XML data from HTTP answer
    std::string xmlData;
    removeHttpData(feedfile, xmlData);

    // Parse XML
    streamFile(xmlData.c_str(), xmlData.length());
}