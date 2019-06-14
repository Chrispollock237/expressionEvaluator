#ifndef GUARD_tokenizer_hpp20091125_
#define GUARD_tokenizer_hpp20091125_

/** @file: tokenizer.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.1.0
	@date 2012-11-15
	@note Compiles under Visual C++ v110

	@brief Tokenizer class declaration for Expression Evaulator.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "token.hpp"
#include <map>
#include <string>
#include <boost/noncopyable.hpp>


/** Tokenizer class is used to create lists of tokens from expression strings.
	It maintains a dictionary of variable tokens introduced by the expression strings.
	*/
class Tokenizer : boost::noncopyable {
// types
public:
	typedef std::string	string_type;

	// Class to throw personalized exceptions
	class XTokenizer : public std::exception {
		string_type	expression_;
		size_t		location_;
	public:
		XTokenizer(string_type const& expression, size_t location, char const* msg)
			: std::exception(msg)
			, expression_( expression )
			, location_( location )
		{ }

		/**	Gets the expression string containing the bad character. */
		string_type	get_expression() const { return expression_; }

		/** Gets the location of the bad character in the expression string. */
		size_t		get_location() const { return location_; }
	};

	/** Bad character exception class. */
	class XBadCharacter : public XTokenizer {
	public:
		XBadCharacter(string_type const& expression, size_t location)
			: XTokenizer(expression, location, "Tokenizer::Bad character in expression.") { }
	};

	/**	Numeric overflow exception class. */
	class XNumericOverflow : public XTokenizer {
	public:
		XNumericOverflow(string_type const& expression, size_t location)
			: XTokenizer(expression, location, "Tokenizer::Too many digits in number.") { }
	};

	/** Function not followed by exception class **/
	class XFunctionNotFollowedBy : public XTokenizer {
	public:
		XFunctionNotFollowedBy(string_type const& expression, size_t location)
			: XTokenizer(expression, location, "Tokenizer::Function not followed by (.") { }
	};

	/** Factorial expression exception class **/
	class XFactorialExperssion : public XTokenizer {
	public:
		XFactorialExperssion(string_type const& expression, size_t location)
			: XTokenizer(expression, location, "Tokenizer::Factorial must follow Expression.") { }
	};

// Typedef for a map containing a string as a "keyword" and a Token pointer as its value
private:
	using dictionary_type = std::map<string_type, Token::pointer_type>;

// Data
private:
	dictionary_type	keywords_;
	dictionary_type variables_;

// Methods
public:
	// C'tor
	Tokenizer();
	TokenList tokenize(string_type const& expression);

private:
	Token::pointer_type _get_identifier(Tokenizer::string_type::const_iterator& currentChar, Tokenizer::string_type const& expression);
	Token::pointer_type _get_number(Tokenizer::string_type::const_iterator& currentChar, Tokenizer::string_type const& expression);
};


/*=============================================================

Revision History

Version 0.1.0: 2012-11-15
Replaced BadCharacter with XTokenizer, XBadCharacter, and XNumericOverflow

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
#endif // GUARD_tokenizer_hpp20091125_
