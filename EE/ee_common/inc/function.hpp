#if !defined(GUARD_function_hpp20091126_)
#define GUARD_function_hpp20091126_

/** @file: function.hpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 1.0.0
	@date 2012-11-13
	@note Compiles under Visual C++ v120

	@brief function token declarations.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "operation.hpp"

/** Function token base class. */
class Function : public Operation { };

		/** One argument function token base class. */
		class OneArgFunction : public Function {
		public:
			virtual unsigned number_of_args() const override { return 1; }
		};

				/** Absolute value function token. */
				class Abs : public OneArgFunction {	};

				/** Arccos value function token. */
				class Arccos : public OneArgFunction {	};

				/** Arcsin value function token. */
				class Arcsin : public OneArgFunction {	};

				/** Arctan value function token. */
				class Arctan : public OneArgFunction {	};

				/** Ceil value function token. */
				class Ceil : public OneArgFunction {	};

				/** Cos value function token. */
				class Cos : public OneArgFunction {	};

				/** Exp value function token. */
				class Exp : public OneArgFunction {	};

				/** Floor value function token. */
				class Floor : public OneArgFunction {	};

				/** Lb value function token. */
				class Lb : public OneArgFunction {	};

				/** Ln value function token. */
				class Ln : public OneArgFunction {	};

				/** Log value function token. */
				class Log : public OneArgFunction {	};

				/** Result value function token. */
				class Result : public OneArgFunction {	};

				/** Sin value function token. */
				class Sin : public OneArgFunction {	};

				/** Sqrt value function token. */
				class Sqrt : public OneArgFunction {	};

				/** Tan value function token. */
				class Tan : public OneArgFunction {	};

				/** Fib value function token. */
				class Fib : public OneArgFunction {	};

				/** Pel value function token. */
				class Pel : public OneArgFunction {	};

				/** Syl value function token. */
				class Syl : public OneArgFunction {	};

			/** One argument function token base class. */
			class TwoArgFunction : public Function {
			public:
				virtual unsigned number_of_args() const override { return 2; }
			};

				/** Arctan2 value function token. */
				class Arctan2 : public TwoArgFunction {	};

				/** Max value function token. */
				class Max : public TwoArgFunction {	};

				/** Min value function token. */
				class Min : public TwoArgFunction {	};

				/** Pow value function token. */
				class Pow : public TwoArgFunction {	};


/*=============================================================

Revision History

Version 1.0.0: 2016-11-02
Added 'override' keyword where appropriate.

Version 0.0.2: 2014-10-30
Removed binary function

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
#endif // GUARD_function_hpp20091126_
