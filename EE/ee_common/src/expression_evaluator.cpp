/** @file: expression_evaluator.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.0.1
	@date 2012-11-13
	@note Compiles under Visual C++ v110

	@brief ExpressionEvaluator class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "../inc/expression_evaluator.hpp"

ExpressionEvaluator::result_type ExpressionEvaluator::evaluate(expression_type const& expr)
{
	std::vector<Token::pointer_type> tokenVec = ExpressionEvaluator::tokenizer_.tokenize(expr);
	std::vector<Token::pointer_type> parsedVec = ExpressionEvaluator::parser_.parse(tokenVec);
	Token::pointer_type evaluatedOperand = ExpressionEvaluator::rpn_.evaluate(parsedVec);

	return evaluatedOperand;
}
/*=============================================================

Revision History

Version 0.0.0: 2010-10-31
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
