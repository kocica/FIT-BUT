/**
 * @file    SSL.h
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Creates an secured connection with server and sending
 * HTTP request for feed file, returns received feed file content
 */


#pragma once

// STD
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>

// OpenSSL
#include "openssl/bio.h"
#include "openssl/ssl.h"
#include "openssl/err.h"

// Local
#include "FeedreaderException.h"
#include "Arguments.h"


/**
 * @brief Class ensuring secured connection with server
 *        Also sending HTTP request and receiving data
 * 
 * Code copied from IBM tutorial: https://www.ibm.com/developerworks/library/l-openssl/ and modified
 *     - Connection, verification, send, receive.
 *     - Author: Kenneth Ballard
 *     - Published on July 22, 2004/Updated: June 28, 2012
 */
class CSSL
{
public:
	/**
	 * @brief We need server & port
	 */
	CSSL() = delete;

	/**
	 * @brief Constructor accepting server name, port & args
	 */
	CSSL(const std::string& server, int port, SArguments& args, bool secured);

	/**
	 * @brief Virtual destructor
	 */
	virtual ~CSSL();


	/**
	 * @brief Sending HTTP request to server
	 *        and returns reveived data
	 *
	 * @param path [in] Path to RSS feed on server
	 * @return Received data from server
	 */
	std::string get(const std::string& path);

protected:
	/** @brief Maximal hostname & port size */
	const int HOSTNAME_PORT_SIZE = 256;
	/** @brief Maximal output buffer size */
	const int OUTPUT_BUFFER_SIZE = 1024;
	/** @brief Maximal input buffer size */
	const int INPUT_BUFFER_SIZE  = 4096;

	/** @brief Method which initialize SSL */
	void initSSL();
	/** @brief Method providing verification of certificate */
	void verification(SArguments& args);

	/**
	 * @brief Sending HTTP request to server
	 *
	 * @param path [in] Path to RSS feed on server
	 */
	void sendRequest(const std::string& path);

	/**
	 * @brief Returns reveived data from server
	 *
	 * @return Received data from server
	 */
	std::string receiveAnswer();


private:
	bool                  m_secured;
	std::string           m_server;

	BIO                 * m_bio;
	SSL_CTX             * m_ctx;
	SSL                 * m_ssl;
};