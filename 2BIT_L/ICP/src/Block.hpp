/**
 *		@file 		Block.hpp
 *		@date 		01/03/2018
 *		@author 	Filip Kocica <xkocic01@fit.vutbr.cz>
 *		@brief 		Abstract implementation of Block, from which other specific
 *					blocks inherits.
 */

#include <iostream>

#include "BlockType.hpp"

///
/// Namespace with implementation of logic of an application
///
namespace BlockEditorLogic
{
	///
	/// Interface which declares actions that all the blocks have same
	///
	class CBlock
	{
	public:


	protected:
		EBlockType		 m_bt;			/**< Type of block {ADD|SUB|etc.} */

	private:


	};
}