/**
 *		@file 		BlockEditorException.hpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *		@brief
 */

#pragma once

#include <iostream>
#include <exception>

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  /**
   *  Exception specific for block editor application
   */
  class CBlockEditorException : public std::exception
  {
  public:
      /** Constructor (C strings).
       *  @param message C-style string error message.
       *                 The string contents are copied upon construction.
       *                 Hence, responsibility for deleting the char* lies
       *                 with the caller.
       */
      explicit CBlockEditorException(const char* message) :
        m_msg(message)
        {

        }

      /** Constructor (C++ STL strings).
       *  @param message The error message.
       */
      explicit CBlockEditorException(const std::string& message) :
        m_msg(message)
        {

        }

      /** Destructor.
       * Virtual to allow for subclassing.
       */
      virtual ~CBlockEditorException() throw ()
      {

      }

      /** Returns a pointer to the (constant) error description.
       *  @return A pointer to a const char*. The underlying memory
       *          is in posession of the Exception object. Callers must
       *          not attempt to free the memory.
       */
      virtual const char * what() const throw ()
      {
         return m_msg.c_str();
      }

  protected:
      /** Error message.
       */
      std::string m_msg;
  };
}