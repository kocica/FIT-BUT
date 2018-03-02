/**
 *		@file 		BlockScheme.hpp
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
  /// Class which holds data about whole scheme and implements all actions
  /// with scheme such as save, open saved, etc.
  ///
  class CBlockScheme
  {
  public:
    void saveScheme();
  protected:

  private:
    // deque of block; each block deque of ports; each port has type
  };
}