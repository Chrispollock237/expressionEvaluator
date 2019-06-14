#if !defined(GUARD_operand_hpp20091125_)
#define GUARD_operand_hpp20091125_

/** @file: operand.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.1.1
	@date 2012-11-13
	@note Compiles under Visual C++ v110
	@brief Operand class declaration.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/


#include "token.hpp"
#include <cassert>


/** Operand token base class. */
class Operand : public Token {
public:
	DEF_POINTER_TYPE(Operand)
};


/** Make a new smart-pointer managed Token object with constructor parameter. */
template <typename T, typename U> inline Operand::pointer_type make_operand( U const& param ) {
	return Operand::pointer_type( new T(param) );
}


/** Gets the value from an operand. */
template <typename OPERAND_TYPE>
typename OPERAND_TYPE::value_type get_value( Token::pointer_type const& operand ) {
	assert( is<OPERAND_TYPE>( operand ) );
	return dynamic_cast<OPERAND_TYPE*>( operand.get() )->get_value();
}



/*=============================================================

Revision History

Version 0.1.1: 2012-11-13
C++ cleanup.

Version 0.1.0: 2010-11-09
Switched boost::shared_ptr<> to std::shared_ptr<>.
Added TOKEN_PTR_TYPE macro.

Version 0.0.0: 2009-11-25
Alpha release.

=============================================================

Copyright Garth Santor/Trinh Han

The copyright to the computer program(s) herein
is the property of Garth Santor/Trinh Han of Canada.
The program(s) may be used and/or copied only with
the written permission of Garth Santor/Trinh Han
or in accordance with the terms and conditions
stipulated in the agreement/contract under which
the program(s) have been supplied.
=============================================================*/
#endif // GUARD_operand_hpp20091125_
