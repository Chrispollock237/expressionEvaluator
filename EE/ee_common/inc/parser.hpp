#ifndef GUARD_parser_hpp20091201_
#define GUARD_parser_hpp20091201_

/** @file: parser.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 1.0.0
	@date 2012-11-13
	@note Compiles under Visual C++ v120

	@brief Parser class declaration.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include <boost/noncopyable.hpp>
#include "token.hpp"
#include <stack>
#include <queue>

class Parser : boost::noncopyable {
public:
	TokenList parse( TokenList const& infixTokens );
};

/*=============================================================

Revision History
Version 1.0.0: 2014-10-31
Visual C++ 2013

Version 0.0.1: 2012-11-13
C++ 11 cleanup

Version 0.0.0: 2009-12-01
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
#endif // GUARD_parser_hpp20091201_
