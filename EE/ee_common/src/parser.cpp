/** @file: parser.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 1.0.0
	@date 2012-11-13
	@note Compiles under Visual C++ v120

	@brief Parser class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/


#include "../inc/parser.hpp"
#include "../inc/integer.hpp"
#include "../inc/variable.hpp"
#include "../inc/boolean.hpp"
#include "../inc/function.hpp"
#include "../inc/operation.hpp"
#include "../inc/operator.hpp"
#include "../inc/real.hpp"
#include "../inc/token.hpp"
#include "../inc/pseudo_operation.hpp"

using namespace std;

TokenList Parser::parse(TokenList const& infixTokens) {
	// Create Stack and Queue
	stack<Token::pointer_type> operation;
	vector<Token::pointer_type> output;

	bool leftParenthesis = false;
	bool rightParenthesis = false;
	
	// Loop through the infix TokenList and Parse the pointers
	for (auto t : infixTokens)
	{
		// Is the token a Operand(Digit or Variable)
		if (Operand::pointer_type opPtr = dynamic_pointer_cast<Operand>(t))
		{
			if (Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(t))
			{
				// Push the Operand onto the vector
				output.push_back(integerPtr);
				continue;
			}
			else if (Real::pointer_type realPtr = dynamic_pointer_cast<Real>(t))
			{
				// Push the Operand onto the vector
				output.push_back(realPtr);
				continue;
			}
			else if (Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(t))
			{
				// Push the Operand onto the vector
				output.push_back(boolPtr);
				continue;
			}
			else if (Variable::pointer_type variablePtr = dynamic_pointer_cast<Variable>(t))
			{
				// Push the Operand onto the vector
				output.push_back(variablePtr);
				continue;
			}
			//// Push the Operand onto the vector
			//output.push_back(t);
		}
		else if (Function::pointer_type funcPtr = dynamic_pointer_cast<Function>(t))
		{
			// Push Function onto the vector
			operation.push(t);
			continue;
		}
		else if (PseudoOperation::pointer_type pseuPtr = dynamic_pointer_cast<ArgumentSeparator>(t))
		{
			// While the top of the stack is not a left parenthesis push the arguments to the vector 
			// and pop them from the stack
			while (!is<LeftParenthesis>(operation.top()))
			{
				output.push_back(operation.top());
				operation.pop();
			}
			continue;
		}
		else if (PseudoOperation::pointer_type pseuPtr = dynamic_pointer_cast<LeftParenthesis>(t))
		{
			leftParenthesis = true;
			// Push the left parenthesis onto the stack
			operation.push(t);
			continue;
		}
		else if (PseudoOperation::pointer_type pseuPtr = dynamic_pointer_cast<RightParenthesis>(t))
		{
			rightParenthesis = true;
			// Once a right parenthesis has been hit, you want to pop every Operator in the stack
			while (!dynamic_pointer_cast<LeftParenthesis>(operation.top()))
			{
				output.push_back(operation.top());
				operation.pop();

				// If the while loop empties the stack and it doesn't find a left parenthesis then
				// the exoression is incorrect and it throws an arguments stating this
				if (operation.empty())
					throw "Parser::Right parenthesis, has no matching left parethesis.";

			} // end while

			// Once the while loop hits a Left Parenthesis '(' it while exit and this will pop
			// it from the stack so it doesn't go onto the vector
			operation.pop();

			// if the stack isn't empty, check if the pointer on the top of the stack is a function
			// If it is the push it to the vector
			if (!operation.empty())
			{
				if (Function::pointer_type funcPtr = dynamic_pointer_cast<Function>(operation.top()))
				{
					output.push_back(operation.top());
					operation.pop();
				}
			}
			continue;
		}
		else if (Operator::pointer_type opPtr = dynamic_pointer_cast<Operator>(t))
		{
			// While the stack isn't empty check for the precedence of the Operators
			// Push them onto the vector and pop from the stack in the correct order by checking
			// their precedence
			while (!operation.empty())
			{
				Operator::pointer_type topOpPtr = dynamic_pointer_cast<Operator>(operation.top());

				if (!is<Operator>(operation.top()))
					break;
				if (is<NonAssociative>(operation.top()) && !is<Factorial>(topOpPtr) && !is<Not>(topOpPtr))
					break;
				if (is<LAssocOperator>(operation.top()) && opPtr->get_precedence() > topOpPtr->get_precedence())
					break;
				if (is<RAssocOperator>(operation.top()) && (opPtr->get_precedence() > topOpPtr->get_precedence() 
					|| opPtr->get_precedence() == topOpPtr->get_precedence()))
					break;

				output.push_back(operation.top());
				operation.pop();

			} // end while

			if (is<Multiplication>(t))
			{
				Multiplication::pointer_type multiplicationPtr = dynamic_pointer_cast<Multiplication>(t);
				operation.push(multiplicationPtr);
				continue;
			}
			else if (is<Division>(t))
			{
				Division::pointer_type divisionPtr = dynamic_pointer_cast<Division>(t);
				operation.push(divisionPtr);
				continue;
			}
			else if (is<Modulus>(t))
			{
				Modulus::pointer_type modulusPtr = dynamic_pointer_cast<Modulus>(t);
				operation.push(modulusPtr);
				continue;
			}
			else
			{
				operation.push(opPtr);
				continue;
			}

			if (leftParenthesis == true && rightParenthesis == true)
			{
				leftParenthesis = false;
				rightParenthesis = false;
			}
		}
		else // If everything has been checked, then the token is Unknown and this throws a statement saying such
			throw "Unknown token.";
	} // end for each loop

	if (leftParenthesis == true && rightParenthesis == false)
	{
		throw exception("Error: Missing ')' bracket.");
	}
	else if (leftParenthesis == false && rightParenthesis == true)
	{
		throw exception("Error: Missing '(' bracket.");
	}
	// If there are no more tokens and the stack is not empty, then
	// pop everything from the stack and push it to the vector
	while (!operation.empty())
	{
		if (is<LeftParenthesis>(operation.top()))
			throw "Missing right-parenthesis.";

		output.push_back(operation.top());
		operation.pop();
	}


	return output;	// Returns the postfixed vector.
}

/*=============================================================

Revision History
Version 1.0.0: 2014-10-31
Visual C++ 2013

Version 0.0.1: 2012-11-13
C++ 11 cleanup

Version 0.0.0: 2009-12-02
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
