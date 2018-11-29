/**
 * @file    SSL.cpp
 * @author	Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    26/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Creates an secured connection with server and sending
 * HTTP request for feed file, returns received feed file content
 */

#include "SSL.h"


CSSL::CSSL(const std::string& server, int port, SArguments& args, bool secured)
: m_server { server }
, m_secured { secured }
{
	initSSL();
	if (m_secured)
	{
		verification(args);
	}

	char hostnamePort[HOSTNAME_PORT_SIZE] = "";
	sprintf(hostnamePort, "%s:%d", server.c_str(), port);


	if (!m_secured)
	{
		// Create new unsecured connection
		m_bio = BIO_new_connect(hostnamePort);
	}
	else
	{
		// Create new SSL secured connection
		m_bio = BIO_new_ssl_connect(m_ctx);
		BIO_get_ssl(m_bio, &m_ssl);
		SSL_set_mode(m_ssl, SSL_MODE_AUTO_RETRY);

		BIO_set_conn_hostname(m_bio, hostnamePort);
	}

	if(m_bio == NULL)
	{
		throw CFeedreaderException("ERROR: Connection failed.", E_CONNECTION_FAILED);
	}

	if (BIO_do_connect(m_bio) <= 0)
	{
	    throw CFeedreaderException("ERROR: Connection failed.", E_CONNECTION_FAILED);
	}

	if(m_secured && SSL_get_verify_result(m_ssl) != X509_V_OK)
	{
	    throw CFeedreaderException("ERROR: Verification failed.", E_VALIDATION_FAILED);
	}
}

CSSL::~CSSL()
{
	if (m_secured)
	{
		EVP_cleanup();
		SSL_CTX_free(m_ctx);
	}

	ERR_free_strings();
	BIO_free_all(m_bio);
}

void CSSL::initSSL()
{
	SSL_library_init();
	
	if (m_secured)
	{
		m_ctx = SSL_CTX_new(SSLv23_client_method());
	}

	SSL_load_error_strings();
	ERR_load_BIO_strings();
	OpenSSL_add_all_algorithms();
}

void CSSL::verification(SArguments& args)
{
	if (args.C_flag && args.c_flag)  // Certfile & Certaddr specified
	{
		if(! SSL_CTX_load_verify_locations(m_ctx, args.cerfile.c_str(), args.certaddr.c_str()))
		{
	   		throw CFeedreaderException("ERROR: Certificate verification failed.", E_VALIDATION_FAILED);
		}
	}
	else if (args.c_flag)           // Certfile specified
	{
		if(! SSL_CTX_load_verify_locations(m_ctx, args.cerfile.c_str(), NULL))
		{
	   		throw CFeedreaderException("ERROR: Certificate verification failed.", E_VALIDATION_FAILED);
		}
	}
	else if (args.C_flag)           // Certaddr specified
	{
		if(! SSL_CTX_load_verify_locations(m_ctx, NULL, args.certaddr.c_str()))
		{
	   		throw CFeedreaderException("ERROR: Certificate verification failed.", E_VALIDATION_FAILED);
		}
	}
	else                            // Neither Certfile nor Certfolder specified, use default paths
	{
		SSL_CTX_set_default_verify_paths(m_ctx);
	}
}

std::string CSSL::get(const std::string& path)
{
	sendRequest(path);
	return std::move(receiveAnswer());
}

void CSSL::sendRequest(const std::string& path)
{
	char buf_request[OUTPUT_BUFFER_SIZE] = "";
	
	sprintf(buf_request, "GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent: feedreader-xkocic01\r\nConnection: close\r\n\r\n", path.c_str(), m_server.c_str());

	if (BIO_write(m_bio, buf_request, strlen(buf_request)) <= 0)
	{
	    if(!BIO_should_retry(m_bio))
	    {
	        throw CFeedreaderException("ERROR: Write failed.", E_WRITE_READ_FAILED);
	    }
	}
}

std::string CSSL::receiveAnswer()
{
	std::string  toReturn;
	char         buffer[INPUT_BUFFER_SIZE] = "";
    int          x;


    while ((x = BIO_read(m_bio, buffer, INPUT_BUFFER_SIZE)) > 0)
    {
    	toReturn += std::string(buffer, x);
    }
	
	if (x < 0)
	{
	   if (!BIO_should_retry(m_bio))
	    {
	        throw CFeedreaderException("ERROR: Read failed.", E_WRITE_READ_FAILED);
	    }
	}

    return std::move(toReturn);
}