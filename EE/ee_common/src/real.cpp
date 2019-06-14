/** @file: real.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@date 2012-11-13
	@version 1.0.0
	@note Compiles under Visual C++ v120

	@brief Real class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "../inc/real.hpp"

// Real to_string method body, 
// lexical_cast casts the boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000, int32_t, void>>
// which is the value type of the Real number to a string.
Real::string_type Real::to_string() const {
	return string_type("Real: ") + boost::lexical_cast<string_type>(get_value());
}

/*=============================================================

Revision History
Version 1.0.0: 2014-10-29
C++ 11 refactor.
Switched value_type to boost::multiprecision::cpp_dec_float_100

Version 0.0.1: 2012-11-13
C++ 11 cleanup

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
