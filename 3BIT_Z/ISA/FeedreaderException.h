/**
 * @file    FeedreaderException.h
 * @author  Filip Kocica <xkocic01@stud.fit.vutbr.cz>
 * @date    28/09/2018
 *
 * ISA project -- Feed reader in Atom format with TLS support
 *
 * Special exception holding exception string & code
 */

#pragma once

#include <iostream>
#include <exception>


enum EErrorCode
{
	I_HELP              =  0,   /**< Print help and exit succesfully */
	E_RUNTIME_ERROR     = -1,   /**< FATAL runtime error -> Program is terminated */
	E_VALIDATION_FAILED = -2,   /**< Certificate validation error */
	E_CONNECTION_FAILED = -3,   /**< Connection to server failed */
	E_WRITE_READ_FAILED = -4,   /**< Write/read to/from server failed */
	E_WRONG_URL         = -5,   /**< Wrong input URL */
	E_WRONG_XML         = -6,   /**< Wrong format of XML file */
};


/**
 *  @brief Specific exception for feed reader
 */
class CFeedreaderException : public std::exception
{
public:
	/**
	*  @brief Constructor (C strings).
	*  @param message C-style string error message.
	*                 The string contents are copied upon construction.
	*                 Hence, responsibility for deleting the char* lies
	*                 with the caller.
	*/
	explicit CFeedreaderException(const char* message, EErrorCode ec = EErrorCode::E_RUNTIME_ERROR)
	: m_msg{message}
	, m_err{ec}
	{

	}

	/**
	*  @brief Constructor (C++ STL strings).
	*  @param message The error message.
	*/
	explicit CFeedreaderException(const std::string& message, EErrorCode ec = EErrorCode::E_RUNTIME_ERROR)
	: m_msg{message}
	, m_err{ec}
	{

	}

	/**
	* @brief Destructor
	* Virtual to allow for subclassing.
	*/
	virtual ~CFeedreaderException() throw ()
	{

	}

	/**
	*  @brief Returns a pointer to the (constant) error description.
	*  @return A pointer to a const char*. The underlying memory
	*          is in posession of the Exception object. Callers must
	*          not attempt to free the memory.
	*/
	virtual const char * what() const throw ()
	{
		return m_msg.c_str();
	}

	/**
	*  @brief Get function
	*  @return Error code of exception
	*/
	virtual EErrorCode getErrCode() const throw ()
	{
		return m_err;
	}

protected:
	/** @brief Error message */
	std::string   m_msg;

	/** @brief Error code */
	EErrorCode    m_err;
};