/** @file: boolean.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@date 2012-11-13
	@version 0.0.1
	@note Compiles under Visual C++ v110

	@brief Boolean class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "../inc/boolean.hpp"
#include <boost/lexical_cast.hpp>


// Boolean to_string method body
// lexical_cast casts the bool value to a string
Boolean::string_type Boolean::to_string() const {
	return string_type("Boolean: ") + boost::lexical_cast<string_type>(get_value());
}

/*=============================================================

Revision History

Version 0.0.1: 2012-11-13
C++ 11 cleanup

Version 0.0.0: 2009-11-26
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
