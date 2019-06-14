#if !defined(GUARD_operator_hpp20091126_)
#define GUARD_operator_hpp20091126_

/** @file: operator.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@date 2012-11-16
	@version 0.2.0
	@note Compiles under Visual C++ v110

	@brief Operator class declarations

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "operation.hpp"
#include "operand.hpp"


/** Operator Precedence values. */
typedef enum { MIN = 0,
	ASSIGNMENT, LOGOR, LOGXOR, LOGAND, BITOR, BITXOR, BITAND, EQUALITY, RELATIONAL,
	BITSHIFT, ADDITIVE, MULTIPLICATIVE,  POWER, UNARY, POSTFIX,
	MAX } precedence_type;



/** Defines a precedence category method.  Used inside a class declaration. */
#define DEF_PRECEDENCE(category)	public: precedence_type get_precedence() const override { return category; }



/** Operator token base class. */
class Operator : public Operation {
public:
	DEF_POINTER_TYPE(Operator)
	virtual precedence_type get_precedence() const = 0;
};

/** Binary operator token base class. */
class BinaryOperator : public Operator {
public:
	virtual unsigned number_of_args() const override { return 2; }
};

/** Compare two tokens for different value. */
// todo: operator > (Token,Token)
inline bool operator > (Operator::pointer_type const& lhs, Operator::pointer_type const& rhs)
{
	return lhs->get_precedence() > rhs->get_precedence();
};

/** Compare two tokens for different value. */
// todo: operator < (Token,Token)
inline bool operator < (Operator::pointer_type const& lhs, Operator::pointer_type const& rhs)
{
	return lhs->get_precedence() < rhs->get_precedence();
};



/* Unary opertaor token base class. */
class UnaryOperator : public Operator {
public:
	virtual unsigned number_of_args() const override { return 1; }
	DEF_PRECEDENCE(UNARY);
};

/* Non-Associative token class */
class NonAssociative : public UnaryOperator {
public:
	virtual unsigned number_of_args() const override { return 1; }
};

/* Negation token class */
class Negation : public NonAssociative {
	DEF_PRECEDENCE(UNARY);
};

/* Identity token class */
class Identity : public NonAssociative {
	DEF_PRECEDENCE(UNARY);
};

/* Not token class */
class Not : public NonAssociative {
	DEF_PRECEDENCE(UNARY);
};

/* BitNot token class */
class BitNot : public NonAssociative {
	DEF_PRECEDENCE(UNARY);
};

/* PostfixOperator token class */
class PostfixOperator : public NonAssociative {
	DEF_PRECEDENCE(POSTFIX);
};

/* Factorial token class */
class Factorial : public PostfixOperator {
	DEF_PRECEDENCE(POSTFIX);
};

/** Right-associative operator base class. */
class RAssocOperator : public BinaryOperator { };

		/** Power token class */
		class Power : public RAssocOperator {
		DEF_PRECEDENCE(POWER)
		};

/** Left-associative operator base class. */
class LAssocOperator : public BinaryOperator { };

/** Multiplicative operator base class. */
class Multiplicative : public LAssocOperator {
	DEF_PRECEDENCE(MULTIPLICATIVE);
};

		/** Multiplication token class */
		class Multiplication : public  Multiplicative {
			DEF_PRECEDENCE(MULTIPLICATIVE);
		};

		/** Division token class */
		class Division : public Multiplicative {
			DEF_PRECEDENCE(MULTIPLICATIVE);
		};

		/** Modulus token class */
		class Modulus : public Multiplicative {
			DEF_PRECEDENCE(MULTIPLICATIVE);
		};

/** Additive operator base class. */
class Additive : public LAssocOperator {
	DEF_PRECEDENCE(ADDITIVE);
};

	/** Addition token class */
	class Addition : public Additive {
		DEF_PRECEDENCE(ADDITIVE);
	};

	/** Subtraction token class */
	class Subtraction : public Additive {
		DEF_PRECEDENCE(ADDITIVE);
	};

/** BitShift operator base class. */
class BitShift : public LAssocOperator {
	DEF_PRECEDENCE(BITSHIFT);
};

	/** BitShiftLeft token class */
	class BitShiftLeft : public LAssocOperator {
		DEF_PRECEDENCE(BITSHIFT);
	};

	/** BitShiftRight token class */
	class BitShiftRight : public LAssocOperator {
		DEF_PRECEDENCE(BITSHIFT);
	};

/** Relational operator base class. */
class Relational : public LAssocOperator {
	DEF_PRECEDENCE(RELATIONAL);
};

	/** Greater token class */
	class Greater : public Relational {
		DEF_PRECEDENCE(RELATIONAL);
	};

	/** Less token class */
	class Less : public Relational {
		DEF_PRECEDENCE(RELATIONAL);
	};

	/** GreaterEqual token class */
	class GreaterEqual : public Relational {
		DEF_PRECEDENCE(RELATIONAL);
	};

	/** LessEqual token class */
	class LessEqual : public Relational {
	DEF_PRECEDENCE(RELATIONAL);
	};

/** Equality token class */
class Equality : public LAssocOperator {
	DEF_PRECEDENCE(EQUALITY);
};

/** Inequality token class */
class Inequality : public LAssocOperator {
	DEF_PRECEDENCE(EQUALITY);
};

/** BitAnd token class */
class BitAnd : public LAssocOperator {
	DEF_PRECEDENCE(BITAND);
};

/** BitOr token class */
class BitOr : public LAssocOperator {
	DEF_PRECEDENCE(BITOR);
};

/** LogAnd token base class */
class LogAnd : public LAssocOperator {
	DEF_PRECEDENCE(LOGAND);
};

	/** And token class */
	class And : public LogAnd {
		DEF_PRECEDENCE(LOGAND);
	};

	/** Nand token class */
	class Nand : public LogAnd {
		DEF_PRECEDENCE(LOGAND);
	};

/** LogXor token base class */
class LogXor : public LAssocOperator {
	DEF_PRECEDENCE(LOGXOR);
};

	/** Xor token class */
	class Xor : public LogXor {
		DEF_PRECEDENCE(LOGXOR);
	};

	/** Xnor token class */
	class Xnor : public LogXor {
		DEF_PRECEDENCE(LOGXOR);
	};

/** LogOr token base class */
class LogOr : public LAssocOperator {
	DEF_PRECEDENCE(LOGOR);
};

	/** Or token class */
	class Or : public LogOr {
		DEF_PRECEDENCE(LOGOR);
	};

	/** Nor token class */
	class Nor : public LogOr {
		DEF_PRECEDENCE(LOGOR);
	};

/** Assignment token class */
class Assignment : public RAssocOperator {
	DEF_PRECEDENCE(ASSIGNMENT);
};


/*=============================================================

Revision History
Version 1.0.0: 2016-11-02
Added 'override' keyword where appropriate.

Version 0.3.0: 2014-10-30
Removed bitwise operators.

Version 0.2.0: 2012-11-16
Added BitAnd, BitNot, BitOr, BitShiftLeft, BitShiftRight

Version 0.1.1: 2012-11-13
C++ 11 cleanup

Version 0.1.0: 2010-11-09
Switched boost::shared_ptr<> to std::shared_ptr<>.
Added TOKEN_PTR_TYPE macro.

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
#endif // GUARD_operator_hpp20091126_
