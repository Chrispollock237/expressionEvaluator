#if !defined(GUARD_integer_hpp20091125_)
#define GUARD_integer_hpp20091125_

/** @file: integer.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 1.0.0
	@date 2012-11-15
	@note Compiles under Visual C++ v110

	@brief Integer class declaration.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "operand.hpp"
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/debug_adaptor.hpp>


/** Integer token. */
class Integer : public Operand {
public:
	using value_type = boost::multiprecision::cpp_int;
	DEF_POINTER_TYPE(Integer)
private:
	value_type	value_;
public:
	//C'tor
	Integer( value_type value = 0 )
		: value_( value ) { }

	// Getter method to get the value of the Integer
	value_type				get_value() const { return value_; }

	// to_string declaration method
	string_type				to_string() const;

};



/*=============================================================

Revision History
Version 1.0.0: 2014-10-29
C++ 11 refactor.
Converted Integer::value_type to boost::multiprecision::cpp_int
Removed BinaryInteger

Version 0.2.0: 2012-11-15
Added BinaryInteger.

Version 0.1.1: 2012-11-13
C++ 11 cleanup.

Version 0.1.0: 2010-11-09
Switched boost::shared_ptr<> to std::shared_ptr<>.
Added TOKEN_PTR_TYPE macro.
Switched __int64 to long long.

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
#endif // GUARD_integer_hpp20091125_
