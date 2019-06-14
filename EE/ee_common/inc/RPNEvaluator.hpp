#ifndef GUARD_rpnevaluator_hpp20091210_
#define GUARD_rpnevaluator_hpp20091210_

/** @file: RPNEvaluator.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.0.1
	@date 2012-11-13
	@note Compiles under Visual C++ v110

	@brief RPN Evaluator class declaration.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "operand.hpp"

class RPNEvaluator {
public:
	Operand::pointer_type evaluate(TokenList const& container);
};

/*=============================================================

Revision History

Version 0.0.1: 2012-11-13
C++ 11 cleanup

Version 0.0.0: 2009-12-10
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
#endif // GUARD_rpnevaluator_hpp20091210_
