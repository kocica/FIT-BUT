/**
 *		@file 		BlockType.hpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *		@brief 		
 */

#include <iostream>

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
	///
	/// Type of block enumator
	///
	enum EBlockType
	{
		BT_ADD,			/**< Type of block which performs suma operator on inputs */
		BT_SUB,			/**< Type of block which performs sub operator on inputs */
	};
}