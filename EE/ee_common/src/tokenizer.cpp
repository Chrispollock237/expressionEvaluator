/** @file: tokenizer.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.3.0
	@date 2017-11-23
	@note Compiles under Visual C++ 15.4.4

	@brief Tokenizer class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "../inc/token.hpp"
#include "../inc/tokenizer.hpp"
#include "../inc/boolean.hpp"
#include "../inc/function.hpp"
#include "../inc/integer.hpp"
#include "../inc/operator.hpp"
#include "../inc/pseudo_operation.hpp"
#include "../inc/real.hpp"
#include "../inc/variable.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;

#include <exception>
#include <limits>
#include <sstream>
#include <stack>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

/** Default constructor loads the keywords_ dictionary(map). */
Tokenizer::Tokenizer() {
	// Functions
	keywords_["abs"] = keywords_["Abs"] = keywords_["ABS"] = make<Abs>();
	keywords_["arccos"] = keywords_["Arccos"] = keywords_["ARCCOS"] = make<Arccos>();
	keywords_["arcsin"] = keywords_["Arcsin"] = keywords_["ARCSIN"] = make<Arcsin>();
	keywords_["arctan"] = keywords_["Arctan"] = keywords_["ARCTAN"] = make<Arctan>();
	keywords_["ceil"] = keywords_["Ceil"] = keywords_["CEIL"] = make<Ceil>();
	keywords_["cos"] = keywords_["Cos"] = keywords_["COS"] = make<Cos>();
	keywords_["exp"] = keywords_["Exp"] = keywords_["EXP"] = make<Exp>();
	keywords_["floor"] = keywords_["Floor"] = keywords_["FLOOR"] = make<Floor>();
	keywords_["lb"] = keywords_["Lb"] = keywords_["LB"] = make<Lb>();
	keywords_["ln"] = keywords_["Ln"] = keywords_["LN"] = make<Ln>();
	keywords_["log"] = keywords_["Log"] = keywords_["LOG"] = make<Log>();
	keywords_["results"] = keywords_["Results"] = keywords_["RESULTS"] = make<Result>();
	keywords_["sin"] = keywords_["Sin"] = keywords_["SIN"] = make<Sin>();
	keywords_["sqrt"] = keywords_["Sqrt"] = keywords_["SQRT"] = make<Sqrt>();
	keywords_["tan"] = keywords_["Tan"] = keywords_["TAN"] = make<Tan>();
	keywords_["fib"] = keywords_["Fib"] = keywords_["FIB"] = make<Fib>();
	keywords_["pel"] = keywords_["Pel"] = keywords_["PEL"] = make<Pel>();
	keywords_["syl"] = keywords_["Syl"] = keywords_["SYL"] = make<Syl>();
	keywords_["arctan2"] = keywords_["Arctan2"] = keywords_["ARCTAN2"] = make<Arctan2>();
	keywords_["max"] = keywords_["Max"] = keywords_["MAX"] = make<Max>();
	keywords_["min"] = keywords_["Min"] = keywords_["MIN"] = make<Min>();
	keywords_["pow"] = keywords_["Pow"] = keywords_["POW"] = make<Pow>();
	
	//Operators
	keywords_["**"] = make<Power>();
	keywords_["="] = make<Assignment>();
	keywords_["*"] = make<Multiplication>();
	keywords_["/"] = make<Division>();
	keywords_["%"] = keywords_["mod"] = keywords_["Mod"] = keywords_["MOD"] = make<Modulus>();
	keywords_["-"] = make<Subtraction>();
	keywords_["+"] = make<Addition>();
	keywords_["=="] = make<Equality>();
	keywords_["!="] = make<Inequality>();
	keywords_[">"] = make<Greater>();
	keywords_[">="] = make<GreaterEqual>();
	keywords_["<"] = make<Less>();
	keywords_["<="] = make<LessEqual>();
	keywords_["and"] = keywords_["And"] = keywords_["AND"] = make<And>();
	keywords_["nand"] = keywords_["Nand"] = keywords_["NAND"] = make<Nand>();
	keywords_["or"] = keywords_["Or"] = make<Or>();
	keywords_["nor"] = keywords_["Nor"] = keywords_["NOR"] = make<Nor>();
	keywords_["xor"] = keywords_["Xor"] = keywords_["XOR"] = make<Xor>();
	keywords_["xnor"] = keywords_["Xnor"] = keywords_["XNOR"] = make<Xnor>();
	keywords_["&"] = make<BitAnd>();
	keywords_["|"] = make<BitOr>();
	keywords_["<<"] = make<BitShiftLeft>();
	keywords_[">>"] = make<BitShiftRight>();
	keywords_["!"] = make<Factorial>();
	keywords_["+"] = make<Identity>();
	keywords_["-"] = make<Negation>();
	keywords_["not"] = keywords_["Not"] = keywords_["NOT"] = make<Not>();
	keywords_["~"] = make<BitNot>();

	// Classes
	keywords_["boolean"] = keywords_["Boolean"] = keywords_["BOOLEAN"] = make<Boolean>();
	keywords_["true"] = keywords_["True"] = keywords_["TRUE"] = make<True>();
	keywords_["false"] = keywords_["False"] = keywords_["FALSE"] = make<False>();
	keywords_["oneargfunction"] = keywords_["Oneargfunction"] = keywords_["ONEARGFUNCTION"] = make<OneArgFunction>();
	keywords_["twoargfunction"] = keywords_["Twoargfunction"] = keywords_["TWOARGFUNCTION"] = make<TwoArgFunction>();
	keywords_["integer"] = keywords_["Integer"] = keywords_["INTEGER"] = make<Integer>();
	keywords_["operand"] = keywords_["Operand"] = keywords_["OPERAND"] = make<Operand>();
	keywords_["parenthesis"] = keywords_["Parenthesis"] = keywords_["PARENTHESIS"] = make<Parenthesis>();
	keywords_["("] =  make<LeftParenthesis>();
	keywords_[")"] =  make<RightParenthesis>();
	keywords_["leftparenthesis"] = keywords_["Leftparenthesis"] = keywords_["LEFTPARENTHESIS"] = make<LeftParenthesis>();
	keywords_[","] =  make<ArgumentSeparator>();
	keywords_["real"] = keywords_["Real"] = keywords_["REAL"] = make<Real>();
	keywords_["pi"] = keywords_["Pi"] = keywords_["PI"] = make<Pi>();
	keywords_["e"] = keywords_["E"] = make<E>();
	keywords_["token"] = keywords_["Token"] = keywords_["TOKEN"] = make<Token>();
	keywords_["variable"] = keywords_["Variable"] = keywords_["VARIABLE"] = make<Variable>();
	keywords_["result"] = keywords_["Result"] = keywords_["RESULT"] = make<Result>();

}

/** Get an identifier from the expression.
	Assumes that the currentChar is pointing to a alphabetic character.
*/
Token::pointer_type Tokenizer::_get_identifier(Tokenizer::string_type::const_iterator& currentChar, Tokenizer::string_type const& expression) {
	// accumulate identifier
	string_type ident;
	
	// Grab all the characters if they are a alphabetic charater or a digit
	// and concatenate them into the identity string
	do
		ident += *currentChar++;
	while (currentChar != end(expression) && isalnum(*currentChar));
	 

	// check for predefined identifier in the keywords_ dictonary(map)
	dictionary_type::iterator iter = keywords_.find(ident);
	if (iter != end(keywords_))
	{
			// Returns the Token::pointer type is found in keywords_ 
			return iter->second;
	}
	else 
	{
		// If the identity is not in the keywords_, then search to see if 
		// it has been made as a variable and is already in the variables_ (map)
		iter = variables_.find(ident);

		// If it is in the variables map_ then return that Token::pointer type
		if (iter != end(variables_))
			return iter->second;
		else // If not in variables_ then make the key word the "ident" and the the value the "Token::pointer_type"
			return variables_[ident] = make<Variable>();  
	}

	// If it is none of these then throw the bad character exception
	throw XBadCharacter(expression, currentChar - begin(expression));
}

/** Get a number token from the expression.
	@return One of BinaryInteger, Integer, or Real.
	@param currentChar [in,out] an iterator to the current character.  Assumes that the currentChar is pointing to a digit.
	@param expression [in] the expression being scanned.
*/
Token::pointer_type Tokenizer::_get_number(Tokenizer::string_type::const_iterator& currentChar, Tokenizer::string_type const& expression) {
	assert(isdigit(*currentChar) && "currentChar must pointer to a digit");

	// Number that will be passed back with the Integer Token
	Integer::value_type accumulatorNumber = 0;

	// Flag to set to true if the number is a real numer when currentChar is a '.'
	// Values to set to how many decimal places the number is and the number of times
	// to divide that number by 10 to add the correct preceding zeros before adding
	// to the accumulatorNumber
	Real::value_type accumulatorRealNumber = 0.0;
	bool isRealNum = false;
	int decimalPlaces = 0;
	int dividedBy = 0;
	Real::value_type realNumLiteral = 0.0;

	// Converts the currentChar to an char, then puts that char into a string
	// so we can convert the string to an int. Then we add that into onto the
	// accumlatorNumber so we can perform the operations to make the number the correct
	// return number.
	char num = *currentChar++;	
	string str;
	str = num;
	int literalValue = stoi(str);
	accumulatorNumber = literalValue;
	accumulatorRealNumber = literalValue;

	// While the currentChar is a number, does not equal the end of the expression AND
	// there is no decimal in the number, multiple by 10 for every digit after the first digit encountered.
	// Have a seperate accumulator for a Real number because we still need all the digits before the decimal
	// when adding the digits to the right of the decimal
	while (currentChar != end(expression) && isdigit(*currentChar) && isRealNum == false)
	{
		accumulatorNumber *= 10;
		accumulatorRealNumber *= 10;
		num = *currentChar++;
		str = num;
		literalValue = stoi(str);
		accumulatorNumber += literalValue;
		accumulatorRealNumber += literalValue;
	};

	// If the currentChar is not the end of expression and it wasn't a digit from before
	// then check to see if the char is a decimal '.', and if it is we can assume that it is a 
	// real number/
	if (currentChar != end(expression))
	{
		num = *currentChar;
		
		if (num == '.')
		{	
			// Increment the currentChar past the decimal, we know it 
			// has at least one decimal place and we will have to divide it by
			// at least 10 once(dividedBy = 1) and set the flag to say that it is a real number
			if (currentChar++ != end(expression))
			{
				num = *currentChar;

				// Check to make sure that there is not a second decimal and that it is not 
				// a alphabetic character or punctuation other than (!) which would mean factorial
				if (num != '.' && (isdigit(num) || num =='!'))
				{
					decimalPlaces = 1;
					dividedBy = decimalPlaces;
					isRealNum = true;
				}
				else
					throw XBadCharacter(expression, currentChar - begin(expression));
			}
			
			// Even if there it is the end of expression we can still assume that it is a Real number
			// with Zero's following the decimal, so we do not want to make it an integer.
			isRealNum = true;
		}
	}

	// If the isRealNum flag as not been set to true, which means we did not encounter a decimal
	// then make an Integer pointer Token
	if ( isRealNum == false)
		return make<Integer>(accumulatorNumber);

	// If it is a Real number than grab all the digits after the decimal
	// Then have to divide by 10 for every consecutive position to the right
	// of the decimal to add the number in the correct position to the accumulator
	while (currentChar != end(expression) && isdigit(*currentChar))
	{
		num = *currentChar++;
		str = num;
		realNumLiteral = stoi(str);

		while (dividedBy > 0)
		{
			realNumLiteral /= 10;
			dividedBy--;
		}

		accumulatorRealNumber += realNumLiteral;

		decimalPlaces++;
		dividedBy = decimalPlaces;	
	}

	// Return the a Real number Token with the value of the Real number
	return make<Real>(accumulatorRealNumber);
}



/** Tokenize the expression.
	@return a TokenList containing the tokens from 'expression'.
	@param expression [in] The expression to tokenize.
	@note Tokenizer dictionary may be updated if expression contains variables.
	@note Will throws 'BadCharacter' if the expression contains an un-tokenizable character.
	*/
TokenList Tokenizer::tokenize( string_type const& expression ) {
	TokenList tokenizedExpression;
	auto currentChar = expression.cbegin();
	//string_type accum = "";
	
	for(;;)
	{
		// strip whitespace
		while (currentChar != end(expression) && isspace(*currentChar))
			++currentChar;

		// check of end of expression
		if (currentChar == end(expression)) break;

		// check for a number
		if (isdigit(*currentChar)) 
		{
			tokenizedExpression.push_back(_get_number(currentChar, expression));
			continue;
		}

		// check for assignment
		if (*currentChar == '=')
		{
			if (++currentChar == end(expression))
			{
				throw exception("Error: Not enough operands.");
			}

			// check for punctuation after the first '=', if none the make an Assignment Token.
			// If there is punctuation following the first '=', check for a second '=' which would make 
			// it an Equality Token
			if (!iswpunct(*currentChar))
				tokenizedExpression.push_back(make<Assignment>());
			else if (*currentChar == '=')
			{
				currentChar++;
				tokenizedExpression.push_back(make<Equality>());
			}
			continue;
		}

		// check for Opening-Parenthesis
		if (*currentChar == '(')
		{
			if (++currentChar == end(expression))
			{
				throw exception("Error: Missing ')' bracket.");
			}
			tokenizedExpression.push_back(make<LeftParenthesis>());
			continue;
		}

		// check for Closing-Parenthesis
		if (*currentChar == ')')
		{
			if (expression.size() == 1)
			{
				throw exception("Error: Missing '(' bracket.");
			}
			currentChar++;
			tokenizedExpression.push_back(make<RightParenthesis>());
			continue;
		}

		// check for asterisk
		if (*currentChar == '*')
		{
			string_type str;

			if(++currentChar != end(expression))
			{	
				str = *currentChar;
			}
			else if (currentChar == end(expression))
			{
				throw exception("Insufficient number of operands for operation");
			}
			
			// check to see if the character after the first asterisk is also an asterisk,
			// as this would make it a Power Token, otherwise it is a Multiplication Token
			if (str[0] == '*')
			{
				tokenizedExpression.push_back(make<Power>());
				currentChar++;
			}		
			else
				tokenizedExpression.push_back(make<Multiplication>());
			continue;
		}

		// check for virgule
		if (*currentChar == '/')
		{
			if (++currentChar != end(expression))
			{
				tokenizedExpression.push_back(make<Division>());
				continue;
			}
			else if (currentChar == end(expression))
			{
				throw exception("Insufficient number of operands for operation");
			}
		}

		// check for Exclamation 
		if (*currentChar == '!')
		{
			currentChar++;

			// If the Token vector is not empty than there should be an integer that will
			// either have a Inequality operator comparing the integer after it or if no
			// integer follows the '!' then it must be a Factorial operation.
			if (!tokenizedExpression.empty())
			{
				if (currentChar != end(expression) && *currentChar == '=')
				{
					currentChar++;
					tokenizedExpression.push_back(make<Inequality>());
					continue;
				}

				if (is<RightParenthesis>(tokenizedExpression.back()) || is<Operand>(tokenizedExpression.back())
					|| is<PostfixOperator>(tokenizedExpression.back()))
				{
					tokenizedExpression.push_back(make<Factorial>());
					continue;
				}
			}
			else // If the vector is empty then it is a bad expression
				throw XFactorialExperssion(expression, currentChar - begin(expression));
		}

		// check if plus sign
		if (*currentChar == '+')
		{
			if (++currentChar == end(expression))
			{
				throw exception("Insufficient number of operands for operation");
			}

			// Check if the vector is empty, if it is empty then we know right away it is an Identity operator.
			// If is not empty then we check for  '(', digit(operend), or ')' Tokens wich would mean it is an Addition
			// Token, otherwise it is an Identity Token.
			if (!tokenizedExpression.empty())
			{
				if (is<RightParenthesis>(tokenizedExpression.back()) || is<Operand>(tokenizedExpression.back())
					|| is<PostfixOperator>(tokenizedExpression.back()))
				{
					tokenizedExpression.push_back(make<Addition>());
					continue;
				}
				else
					tokenizedExpression.push_back(make<Identity>());
			}
			else
				tokenizedExpression.push_back(make<Identity>());
			continue;
		}

		// check if dash
		if (*currentChar == '-')
		{
			if (++currentChar == end(expression))
			{
				throw exception("Insufficient number of operands for operation");
			}

			// Check if the vector is empty, if it is empty then we know right away it is an Negation operator.
			// If is not empty then we check for  '(', digit(operend), or ')' Tokens wich would mean it is an Subtraction
			// Token, otherwise it is an Negation Token.
			if (!tokenizedExpression.empty())
			{
				if (is<RightParenthesis>(tokenizedExpression.back()) || is<Operand>(tokenizedExpression.back())
					|| is<PostfixOperator>(tokenizedExpression.back()))
				{
					tokenizedExpression.push_back(make<Subtraction>());
					continue;
				}
				else
					tokenizedExpression.push_back(make<Negation>());
			}
			else
				tokenizedExpression.push_back(make<Negation>());
			continue;
		}
		
		// check for a alphabetic character
		if (isalpha(*currentChar))
		{
			tokenizedExpression.push_back(_get_identifier(currentChar, expression));
			//currentChar++;
			continue;
		}

		// checks for anything that is not an alpha character or digit
		// Will check in our keywords for any characters that are acceptable 
		// (i.e. %), if it doesn't find it then it is a bad character
		if (!isalnum(*currentChar))
		{
			string_type nonAlphaOrDigit;
			nonAlphaOrDigit += *currentChar++;
			
			if (currentChar != end(expression) && !iswspace(*currentChar) && !isalnum(*currentChar))
			{
				nonAlphaOrDigit += *currentChar;
			}
			else // this --currentChar ensures we grab the right location of the Bad Character
				--currentChar;


			// Check the keywords for the character (i.e %) for modulus
			dictionary_type::iterator iter = keywords_.find(nonAlphaOrDigit);
			if (iter != end(keywords_))
			{
				currentChar++;
				tokenizedExpression.push_back(iter->second);
				continue;
			}

			// If not found in the key words then it is a Bad Character
			throw XBadCharacter(expression, currentChar - begin(expression));
		}
		
		if (is<Function>(tokenizedExpression.back()) && *currentChar != '(')
			throw XFunctionNotFollowedBy(expression, currentChar - begin(expression));
		
		// Not a recognized token
		throw XBadCharacter(expression, currentChar - begin(expression));
	}

	// Return the vector of Tokens
	return tokenizedExpression;
}

/*=============================================================

Revision History

Version 0.3.0: 2017-11-23
Added Python-style power operator '**'.

Version 0.2.0: 2012-11-16
Added BitAnd, BitNot, BitOr, BitXOr, BitShiftLeft, BitShiftRight
Simplified CHECK_OP macros

Version 0.1.0: 2012-11-15
Replaced BadCharacter with XTokenizer, XBadCharacter, and XNumericOverflow
Added BinaryInteger, Binary <function>

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
