/**
 *		@file 		PortType.hpp
 *		@date 		02/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *		@brief
 */

#pragma once

#include <iostream>

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
  ///
  /// Each port has type, which consists of the name and value of type double
  ///
  class CPortType
  {
  public:

  protected:

  private:
    std::string       m_name;     /**< Name of port */
    double            m_value;    /**< Port value */
  };
}