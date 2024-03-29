/** @file: ut_rpn_evaluator.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 1.0.1
	@date 2012-11-16
	@note Compiles under Visual C++ v120

	@brief RPN evaluator unit test.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

// unit test library
//#define _CRT_SECURE_NO_WARNINGS
#define BOOST_TEST_MODULE RpnEvaluatorUnitTest
#include <boost/test/auto_unit_test.hpp>

#include "../ee_common/inc/RPNEvaluator.hpp"
#include "../ee_common/inc/boolean.hpp"
#include "../ee_common/inc/integer.hpp"
#include "../ee_common/inc/function.hpp"
#include "../ee_common/inc/operator.hpp"
#include "../ee_common/inc/real.hpp"
#include "../ee_common/inc/variable.hpp"

#include <boost/math/constants/constants.hpp>

#include "../phase_list/ut_test_phase.hpp"

#if TEST_RPN_EVALUATOR

#if TEST_REAL
Real::value_type round(Real::value_type x) {
	auto exp = pow(Real::value_type("10.0"), Real::value_type("990"));
	x *= exp;
	x += 0.5;
	x = floor(x);
	x /= exp;
	return x;
}

Real::value_type round(Token::pointer_type const& v) {
	return round(get_value<Real>(v));
}
#endif


BOOST_AUTO_TEST_CASE(no_operand) {
	try {
		auto t = RPNEvaluator().evaluate(TokenList());
		BOOST_FAIL("Failed to throw exception");
	}
	catch( std::exception& e ) {
		BOOST_CHECK( strcmp( e.what(), "Error: insufficient operands" ) == 0 );
	}
}


#if TEST_INTEGER
	BOOST_AUTO_TEST_CASE(too_many_operand) {
		try {
			TokenList tl = { make<Integer>(3), make<Integer>(4) };
			auto t = RPNEvaluator().evaluate(tl);
			BOOST_FAIL("Failed to throw exception");
		}
		catch( std::exception& e ) {
			BOOST_CHECK( strcmp( e.what(), "Error: too many operands" ) == 0 );
		}
	}
#endif

// literals
#if TEST_INTEGER
	BOOST_AUTO_TEST_CASE(Integer_3){
		auto result = RPNEvaluator().evaluate({ make<Integer>(3) });
		BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(3));
	}
#endif
#if TEST_REAL
	BOOST_AUTO_TEST_CASE(Real_3_14){
		auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3.14")) });
		BOOST_CHECK(get_value<Real>(result) == Real::value_type("3.14"));
	}
#endif



// Constants
#if TEST_BOOLEAN
	BOOST_AUTO_TEST_CASE(constant_true) {
		auto result = RPNEvaluator().evaluate({ make<True>() });
		BOOST_CHECK(get_value<True>(result) == true);
	}
	BOOST_AUTO_TEST_CASE(constant_false) {
		auto result = RPNEvaluator().evaluate({ make<False>() });
		BOOST_CHECK(get_value<False>(result) == false);
	}
#endif
#if TEST_REAL
	BOOST_AUTO_TEST_CASE(constant_Pi) {
		auto result = RPNEvaluator().evaluate({ make<Pi>() });
		BOOST_CHECK(get_value<Real>(result) == boost::math::constants::pi<Real::value_type>());
	}
	BOOST_AUTO_TEST_CASE(constant_E) {
		auto result = RPNEvaluator().evaluate({ make<E>() });
		BOOST_CHECK(get_value<Real>(result) == boost::math::constants::e<Real::value_type>());
	}
#endif

// Identity
#if TEST_UNARY_OPERATOR
	#if TEST_INTEGER
		BOOST_AUTO_TEST_CASE(identity_test_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Identity>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(3));
		}
	#endif
	#if TEST_REAL
		BOOST_AUTO_TEST_CASE(identity_test_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3")), make<Identity>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("3.0"));
		}
	#endif


	// Negation
	#if TEST_INTEGER
		BOOST_AUTO_TEST_CASE(negation_test_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Negation>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(-3));
		}
	#endif
	#if TEST_REAL
		BOOST_AUTO_TEST_CASE(negation_test_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(3), make<Negation>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("-3.0"));
		}
	#endif


	// Factorial
	#if TEST_INTEGER
		BOOST_AUTO_TEST_CASE(factorial_test_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(5), make<Factorial>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(120));
		}
	#endif


	// Logical not
	#if TEST_BOOLEAN
		BOOST_AUTO_TEST_CASE(not_test_Boolean) {
			auto result = RPNEvaluator().evaluate({ make<Boolean>(true), make<Not>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<Boolean>(false), make<Not>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
		}
	#endif
#endif // TEST_UNARY_OPERATOR


#if TEST_BINARY_OPERATOR
	#if TEST_INTEGER
		BOOST_AUTO_TEST_CASE(test_multiply_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Integer>(4), make<Multiplication>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(12));
		}
		BOOST_AUTO_TEST_CASE(test_divide_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(45), make<Integer>(3), make<Division>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(15));
		}
		BOOST_AUTO_TEST_CASE(test_addition_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Integer>(4), make<Addition>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(7));
		}
		BOOST_AUTO_TEST_CASE(test_subtraction_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Integer>(4), make<Subtraction>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(-1));
		}
		BOOST_AUTO_TEST_CASE(test_modulus_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(12), make<Integer>(5), make<Modulus>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(2));
		}
		BOOST_AUTO_TEST_CASE(test_power_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Integer>(4), make<Power>() });
			BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(81));
		}
		#if TEST_VARIABLE
			BOOST_AUTO_TEST_CASE( assignment_test ) {
				auto result = RPNEvaluator().evaluate({ make<Variable>(), make<Integer>(4), make<Assignment>() });

				BOOST_CHECK(is<Variable>(result));
				Variable::pointer_type v = convert<Variable>(result);
				BOOST_CHECK( is<Integer>( v->get_value() ) );
				Integer::pointer_type i = convert<Integer>( v->get_value() );
				BOOST_CHECK( i->get_value() == 4 );
			}
		#endif
	#endif  // TEST_INTEGER

	#if TEST_REAL
		BOOST_AUTO_TEST_CASE(test_multiply_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(3.0), make<Real>(4.0), make<Multiplication>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("12.0"));
		}
		BOOST_AUTO_TEST_CASE(test_divide_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("45.0")), make<Real>(Real::value_type("3.0")), make<Division>() });
			BOOST_CHECK(round(result) == round(Real::value_type("15.0")));
		}
		BOOST_AUTO_TEST_CASE(test_addition_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3.0")), make<Real>(Real::value_type("4.0")), make<Addition>() });
			BOOST_CHECK(round(result) == round(Real::value_type("7.0")));
		}
		BOOST_AUTO_TEST_CASE(test_subtraction_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3.0")), make<Real>(Real::value_type("4.0")), make<Subtraction>() });
			BOOST_CHECK(round(result) == round(Real::value_type("-1.0")));
		}
		BOOST_AUTO_TEST_CASE(test_power_Real) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3.0")), make<Real>(Real::value_type("4.0")), make<Power>() });
			BOOST_CHECK(round(result) == round(Real::value_type("81")));
		}
	#endif	// TEST_REAL

	#if TEST_MIXED
		BOOST_AUTO_TEST_CASE(test_multiply_Integer_Real) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Real>(4.0), make<Multiplication>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("12.0"));
		}
		BOOST_AUTO_TEST_CASE(test_multiply_Real_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Real>(3.0), make<Integer>(4), make<Multiplication>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("12.0"));
		}
		BOOST_AUTO_TEST_CASE(test_divide_Integer_Real) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Real>(6.0), make<Division>() });
			BOOST_CHECK(round(result) == round(Real::value_type("0.5")));
		}
		BOOST_AUTO_TEST_CASE(test_divide_Real_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Real>(3.0), make<Integer>(6), make<Division>() });
			BOOST_CHECK(round(result) == round(Real::value_type("0.5")));
		}
		BOOST_AUTO_TEST_CASE(test_addition_Integer_Real) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Real>(Real::value_type("4.2")), make<Addition>() });
			BOOST_CHECK(round(result) == round(Real::value_type("7.2")));
		}
		BOOST_AUTO_TEST_CASE(test_addition_Real_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("3.3")), make<Integer>(4), make<Addition>() });
			BOOST_CHECK(round(result) == round(Real::value_type("7.3")));
		}
		BOOST_AUTO_TEST_CASE(test_subtraction_Integer_Real) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(3), make<Real>(Real::value_type("0.5")), make<Subtraction>() });
			BOOST_CHECK(round(result) == round(Real::value_type("2.5")));
		}
		BOOST_AUTO_TEST_CASE(test_subtraction_Real_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("4.25")), make<Integer>(4), make<Subtraction>() });
			BOOST_CHECK(round(result) == round(Real::value_type("0.25")));
		}
		BOOST_AUTO_TEST_CASE(test_neg_power_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(-4), make<Power>() });
			BOOST_CHECK(get_value<Real>(result) == Real::value_type("1.0") / Real::value_type("16.0"));
		}
		BOOST_AUTO_TEST_CASE(test_power_Integer_Real) {
			auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("3.0")), make<Power>() });
			BOOST_CHECK(round(result) == round(Real::value_type("8.0")));
		}
		BOOST_AUTO_TEST_CASE(test_power_Real_Integer) {
			auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Integer>(3), make<Power>() });
			BOOST_CHECK(round(result) == round(Real::value_type("8.0")));
		}

		#if TEST_VARIABLE
			BOOST_AUTO_TEST_CASE( assignment_to_constant_fail ) {
				try {
					auto t = RPNEvaluator().evaluate({ make<Pi>(), make<Integer>(4), make<Assignment>() });
					BOOST_FAIL( "Failed to throw an 'assignment to a non-variable' exception." );
				}
				catch( std::exception const& ex ) {
					BOOST_CHECK( std::string( ex.what() ) == std::string("Error: assignment to a non-variable.") );
				}
			}
		#endif // TEST_VARIABLE
	#endif	// TEST_MIXED

	#if TEST_BOOLEAN
		BOOST_AUTO_TEST_CASE(test_and) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<And>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<And>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<And>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<False>(), make<False>(), make<And>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_nand) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Nand>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<Nand>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Nand>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<False>(), make<Nand>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
		}
		BOOST_AUTO_TEST_CASE(test_nor) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Nor>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<Nor>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Nor>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<False>(), make<False>(), make<Nor>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
		}
		BOOST_AUTO_TEST_CASE(test_or) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Or>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<Or>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Or>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<False>(), make<Or>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_xor) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Xor>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<Xor>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Xor>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<False>(), make<Xor>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_equality_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Equality>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Equality>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_inequality_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Inequality>() });
			BOOST_CHECK(get_value<Boolean>(result) != true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Inequality>() });
			BOOST_CHECK(get_value<Boolean>(result) != false);
		}
		BOOST_AUTO_TEST_CASE(test_greater_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Greater>() });
			BOOST_CHECK(get_value<Boolean>(result) != true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Greater>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_greater_equal_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<GreaterEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<GreaterEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<GreaterEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
		}
		BOOST_AUTO_TEST_CASE(test_less_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<Less>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<Less>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<Less>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		BOOST_AUTO_TEST_CASE(test_less_equal_boolean) {
			auto result = RPNEvaluator().evaluate({ make<True>(), make<True>(), make<LessEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<False>(), make<True>(), make<LessEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == true);
			result = RPNEvaluator().evaluate({ make<True>(), make<False>(), make<LessEqual>() });
			BOOST_CHECK(get_value<Boolean>(result) == false);
		}
		#if TEST_INTEGER
			// Equality
			BOOST_AUTO_TEST_CASE(test_equality_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_inequality_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_equal_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(0), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
			}
			BOOST_AUTO_TEST_CASE(test_less_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(0), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_less_equal_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(1), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Integer>(1), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(0), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
		#endif
		#if TEST_REAL
			BOOST_AUTO_TEST_CASE(test_equality_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Real>(Real::value_type("1.0")), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_inequality_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Real>(Real::value_type("1.0")), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Real>(Real::value_type("1.0")), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) != false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_equal_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("1.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("2.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_less_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("0.1")), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
			}
			BOOST_AUTO_TEST_CASE(test_less_equal_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("1.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("1.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("2.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
			}
		#endif

		#if TEST_MIXED
			BOOST_AUTO_TEST_CASE(test_equality_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<Equality>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_inequality_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<Inequality>() });
				BOOST_CHECK(get_value<Boolean>(result) != false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) != true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<Greater>() });
				BOOST_CHECK(get_value<Boolean>(result) != false);
			}
			BOOST_AUTO_TEST_CASE(test_greater_equal_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(1), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(2), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("1.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("2.0")), make<GreaterEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
			}
			BOOST_AUTO_TEST_CASE(test_less_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Integer>(0), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("1.0")), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(1), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("0.1")), make<Integer>(1), make<Less>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
			}
			BOOST_AUTO_TEST_CASE(test_less_equal_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(1), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.1")), make<Integer>(1), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(2), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("1.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
				result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("1.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == false);
				result = RPNEvaluator().evaluate({ make<Integer>(1), make<Real>(Real::value_type("2.0")), make<LessEqual>() });
				BOOST_CHECK(get_value<Boolean>(result) == true);
			}
		#endif
	#endif // TEST_BOOLEAN
#endif // TEST_BINARY_OPERATOR


#if TEST_FUNCTION
	#if TEST_SINGLE_ARG
		#if TEST_INTEGER
			BOOST_AUTO_TEST_CASE(test_abs_Integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(4), make<Abs>() });
				BOOST_CHECK(get_value<Integer>(result) == 4);
			}
			BOOST_AUTO_TEST_CASE(test_abs_Integer_neg) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(-4), make<Abs>() });
				BOOST_CHECK(get_value<Integer>(result) == 4);
			}
			BOOST_AUTO_TEST_CASE(test_fib_Integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(4), make<Fib>() });
				BOOST_CHECK(get_value<Integer>(result) == 5);
			}
			BOOST_AUTO_TEST_CASE(test_pel_Integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(4), make<Pel>() });
				BOOST_CHECK(get_value<Integer>(result) == 12);
			}
			BOOST_AUTO_TEST_CASE(test_syl_Integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(4), make<Syl>() });
				BOOST_CHECK(get_value<Integer>(result) == 1807);
			}
#endif
		#if TEST_REAL
			BOOST_AUTO_TEST_CASE(test_abs_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("4.0")), make<Abs>() });
				BOOST_CHECK(round(result) == round(Real::value_type("4.0")));
			}
			BOOST_AUTO_TEST_CASE(test_abs_Real_neg) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("-4.0")), make<Abs>() });
				BOOST_CHECK(round(result) == round(Real::value_type("4.0")));
			}
			BOOST_AUTO_TEST_CASE(test_acos_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Arccos>() });
				BOOST_CHECK(round(result) == Real::value_type("0.0"));
			}
			BOOST_AUTO_TEST_CASE(test_asin_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Arcsin>() });
				BOOST_CHECK(get_value<Real>(result) == boost::math::constants::half_pi<Real::value_type>());
			}
			BOOST_AUTO_TEST_CASE(test_atan_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("0.0")), make<Arctan>() });
				BOOST_CHECK(round(result) == Real::value_type("0.0"));
			}
			BOOST_AUTO_TEST_CASE(test_ceil_high_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("4.3")), make<Ceil>() });
				BOOST_CHECK(round(result) == round(Real::value_type("5.0")));
			}
			BOOST_AUTO_TEST_CASE(test_ceil_low_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("-4.3")), make<Ceil>() });
				BOOST_CHECK(round(result) == round(Real::value_type("-4.0")));
			}
			BOOST_AUTO_TEST_CASE(test_cos_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("0.0")), make<Cos>() });
				BOOST_CHECK(round(result) == round(Real::value_type("1.0")));
			}
			BOOST_AUTO_TEST_CASE(test_exp) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Exp>() });
				BOOST_CHECK(round(result) == round(boost::math::constants::e<Real::value_type>()));
			}
			BOOST_AUTO_TEST_CASE(test_floor_positive_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("4.3")), make<Floor>() });
				BOOST_CHECK(round(result) == round(Real::value_type("4.0")));
			}
			BOOST_AUTO_TEST_CASE(test_floor__negative_Real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("-4.3")), make<Floor>() });
				BOOST_CHECK(round(result) == round(Real::value_type("-5.0")));
			}
			BOOST_AUTO_TEST_CASE(test_lb) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("8.0")), make<Lb>() });
				BOOST_CHECK(round(get_value<Real>(result)) == round(Real::value_type("3.0")));
			}
			BOOST_AUTO_TEST_CASE(test_ln) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Ln>() });
				BOOST_CHECK(round(get_value<Real>(result)) == boost::multiprecision::log(Real::value_type("1.0")));
			}
			BOOST_AUTO_TEST_CASE(test_sin) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("0.0")), make<Sin>() });
				BOOST_CHECK(round(get_value<Real>(result)) == Real::value_type("0.0"));
			}
			BOOST_AUTO_TEST_CASE(test_sqrt) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("16.0")), make<Sqrt>() });
				BOOST_CHECK(round(get_value<Real>(result)) == round(Real::value_type("4.0")));
			}
			BOOST_AUTO_TEST_CASE(test_tan) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Tan>() });
				BOOST_CHECK(get_value<Real>(result) == tan(Real::value_type("1.0")));
			}
		#endif
	#endif //TEST_SINGLE_ARG
	#if TEST_MULTI_ARG
		#if TEST_INTEGER
			BOOST_AUTO_TEST_CASE(test_max_int_int_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(2), make<Max>() });
				BOOST_CHECK(get_value<Integer>(result) == 2);
			}
			BOOST_AUTO_TEST_CASE(test_max_int_int_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(1), make<Max>() });
				BOOST_CHECK(get_value<Integer>(result) == 2);
			}
			BOOST_AUTO_TEST_CASE(test_max_int_int_same) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(2), make<Max>() });
				BOOST_CHECK(get_value<Integer>(result) == 2);
			}
			BOOST_AUTO_TEST_CASE(test_min_int_int_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(1), make<Integer>(2), make<Min>() });
				BOOST_CHECK(get_value<Integer>(result) == 1);
			}
			BOOST_AUTO_TEST_CASE(test_min_int_int_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(1), make<Min>() });
				BOOST_CHECK(get_value<Integer>(result) == 1);
			}
			BOOST_AUTO_TEST_CASE(test_min_int_int_same) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(2), make<Min>() });
				BOOST_CHECK(get_value<Integer>(result) == 2);
			}
			BOOST_AUTO_TEST_CASE(test_pow_integer) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Integer>(3), make<Pow>() });
				BOOST_CHECK(get_value<Integer>(result) == Integer::value_type(8));
			}
		#endif // TEST_INTEGER
		#if TEST_REAL
			BOOST_AUTO_TEST_CASE(test_arctan2) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("2.0")), make<Arctan2>() });
				BOOST_CHECK(get_value<Real>(result) == atan2(Real::value_type("1.0"), Real::value_type("2.0")));
			}
			BOOST_AUTO_TEST_CASE(test_max_real_real_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("2.0")), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_max_real_real_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("1.0")), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_max_real_real_same) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("2.0")), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_real_real_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Real>(Real::value_type("2.0")), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("1.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_real_real_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("1.0")), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("1.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_real_real_same) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("2.0")), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_pow_real) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Real>(Real::value_type("3.0")), make<Pow>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("8.0"));
			}
		#endif // TEST_REAL
		#if TEST_MIXED
			BOOST_AUTO_TEST_CASE(test_max_real_int_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(2), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_max_int_real_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("1.0")), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_max_int_real_same) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Integer>(2), make<Max>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_real_int_rhs) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("1.0")), make<Integer>(2), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("1.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_int_real_lhs) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("1.0")), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("1.0"));
			}
			BOOST_AUTO_TEST_CASE(test_min_int_real_same) {
				auto result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Integer>(2), make<Min>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("2.0"));
			}
			BOOST_AUTO_TEST_CASE(test_pow_mixed) {
				auto result = RPNEvaluator().evaluate({ make<Integer>(2), make<Real>(Real::value_type("3.0")), make<Pow>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("8.0"));
				result = RPNEvaluator().evaluate({ make<Real>(Real::value_type("2.0")), make<Integer>(3), make<Pow>() });
				BOOST_CHECK(get_value<Real>(result) == Real::value_type("8.0"));
			}
		#endif // TEST_MIXED
	#endif // TEST_MULTI_ARG
#endif // TEST_FUNCTION


#endif // TEST_RPN_EVALUATOR




/*=============================================================

Revision History
Version 1.0.1: 2014-11-21
Improved round() - reduced float point conversion errors.

Version 1.0.0: 2014-10-31
Visual Studio 2013 update.
Removed bit operations
Added multiprecision.

Version 0.2.0: 2012-11-16
Added BitAnd, BitNot, BitXOr, BitOr, BitShiftLeft, BitShiftRight

Version 0.1.0: 2012-11-15
Added BinaryInteger

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
