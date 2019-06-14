/** @file: RPNEvaluator.cpp
	@author Garth Santor/Trinh Han
	@author http://www.gats.ca/
	@version 0.0.1
	@date 2012-11-13
	@note Compiles under Visual C++ v110

	@brief RPN Evaluator class implementation.

	@implemented by Chris Pollock
	@date 2018 07 12
	*/

#include "../inc/RPNEvaluator.hpp"
#include "../inc/pseudo_operation.hpp"
#include "../inc/operation.hpp"
#include "../inc/operator.hpp"
#include "../inc/token.hpp"
#include "../inc/boolean.hpp"
#include "../inc/function.hpp"
#include "../inc/integer.hpp"
#include "../inc/real.hpp"
#include "../inc/variable.hpp"
#include <vector>
#include <memory>		// shared_ptr<>, dynamic_pointer_cast<>()

using namespace std;

using integer = boost::multiprecision::cpp_int;
using real = boost::multiprecision::cpp_dec_float_50;

// Templated function to calculate the Fibonacci numbers
template<typename T>
T ifibonacci(T const& n) {
	if (n <= 1)
		return n;
	T n1 = 1, n2 = 0, next;
	for (T i = 1; i <= n; ++i) {
		next = n1 + n2;
		n2 = n1;
		n1 = next;
	}
	return next;
};

// Templated function to calculate the Pell numbers
template<typename T>
T pell(T n)
{
	// Variables to hold the previous 2 values and the 
	// calculated result of the pell function
	T result = 0;
	T temp1 = 0;
	T temp2 = 0;

	// For loop to iterate through the algorithm
	for (int i = 0; i <= n; i++)
	{
		if (i == 0)
		{
			temp1 = 0;
			temp2 = 0;
		}
		else if (i == 1)
		{
			temp1 = 1;
		}

		result = (temp2 * 2) + temp1;
		temp1 = temp2;
		temp2 = result;
	}
	return result;
};

template<typename T>
T sylvester(T n)
{
	// Variable to hold the previous results and the current result
	T results = 2;
	T slyTemp1;
	T slyTemp2;

	// For loop to iterate through the algorithm
	for (int i = 0; i < n; i++)
	{
		if (n == 0)
		{
			return results;
		}

		slyTemp1 = results;
		slyTemp2 = results;
		results = pow(slyTemp1, 2) - slyTemp2 + 1;
	}
	return results;
}

Operand::pointer_type RPNEvaluator::evaluate(TokenList const& rpnExpression) {

	// ***** postfix algorithm code goes here *****
	std::vector<Token::pointer_type> operands;
	Integer::value_type evaluatedNumber = 0;
	Integer::value_type operandNumberFirst = 0;
	Integer::value_type operandNumberSecond = 0;
	
	Real::value_type operandRealNumberFirst = 0.0;
	Real::value_type operandRealNumberSecond = 0.0;
	Real::value_type evaluatedRealNumber = 0.0;

	Variable::pointer_type variable = nullptr;
	Variable::pointer_type secondVariable = nullptr;

	Boolean::pointer_type boolPtr = nullptr;
	Boolean::pointer_type boolSecondPtr = nullptr;

	bool evaluated = false;
	bool isReal = false;
	bool isSecondNum = false;
	bool isBoolValue = false;
	bool isSecondBoolValue = false;

	bool boolRealNumberFirst = false;
	bool boolRealNumberSecond = false;
	bool boolNumberFirst = false;
	bool boolNumberSecond = false;
	bool boolVariables = false;

	bool variableFirst = false;
	bool variableSecond = false;

	bool firstBool = false;
	bool secondBool = false;

	if (rpnExpression.size() == 0)
	{
		throw exception("Error: insufficient operands");
	}

	// Loop through the TokenList 
	for (auto t : rpnExpression)
	{
		// Reset the flags
		if (evaluated == true)
		{
			boolRealNumberFirst = false;
			boolRealNumberSecond = false;
			boolNumberFirst = false;
			boolNumberSecond = false;
			boolVariables = false;
			isBoolValue = false;
			isSecondBoolValue = false;
			evaluated = false;
		}

		// If token(t) is a operand(Real or Integer) push onto the stack(vector)
		if (is<Operand>(t))
		{
			if (is<Pi>(t))
			{
				operands.push_back(make<Pi>());
				continue;
			}
			else if (is<E>(t))
			{
				operands.push_back(make<E>());
				continue;
			}
			else
				operands.push_back(t);
		}
		else
		{
			// Cast the token to an Operand token
			Operation::pointer_type operationPtr = dynamic_pointer_cast<Operation>(t);

			// Return the first and second numbers of both the integer and the real numbers back to 0
			operandNumberFirst = 0;
			operandNumberSecond = 0;
			operandRealNumberFirst = 0.0;
			operandRealNumberSecond = 0.0;

			// Check if the number of arguments whether it is a Unary operator or a Binary operator
			// is greater than the number of arguments in the operands stack(vector)
			// If so throw an exception
			if (operationPtr->number_of_args() > operands.size())
			{
				throw exception("Insufficient number of operands for operation");
			}

			// Loop through the operands stack(vector) to pop off the same number of operands
			// as the number of arguments needed for the operator
			for (size_t i = 1; i <= operationPtr->number_of_args(); ++i)
			{
				// Check the back of the operands stack if it is a real number or a integer
				// There should only be a maximum of 2 operands used at anyone time in the evaluation
				if (is<Integer>(operands.back()))
				{
					if (i == 1)
					{
						// Cast the pointer at the back of the vector to an Integer pointer type
						Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(operands.back());

						// Get the numeric value of the pointer
						operandNumberFirst = intPtr->get_value();
						boolNumberFirst = true;

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}
					else if (i == 2)
					{
						// If there are two arguments for the operand then there is a second operand. 
						// Set this flag to true to say there is two operands.
						isSecondNum = true;

						// Cast the pointer at the back of the vector to an Integer pointer type
						Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(operands.back());

						// Get the numeric value of the pointer
						operandNumberSecond = intPtr->get_value();
						boolNumberSecond = true;

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}
				}
				else if (is<Real>(operands.back()))
				{
					isReal = true;

					if (i == 1)
					{
						// Cast the pointer at the back of the vector to an Real number pointer type
						Real::pointer_type realPtr = dynamic_pointer_cast<Real>(operands.back());
						
						// Get the numeric value of the pointer
						operandRealNumberFirst = realPtr->get_value();
						boolRealNumberFirst = true;

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}
					else if (i == 2)
					{
						// If there are two arguments for the operand then there is a second operand. 
						// Set this flag to true to say there is two operands.
						isSecondNum = true;

						// Cast the pointer at the back of the vector to an Real number pointer type
						Real::pointer_type realPtr = dynamic_pointer_cast<Real>(operands.back());

						// Get the numeric value of the pointer
						operandRealNumberSecond = realPtr->get_value();
						boolRealNumberSecond = true;

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}					
				}
				else if (is<Boolean>(operands.back()))
				{
					// Flag to say that there is a boolean value
					isBoolValue = true;

					if (i == 1)
					{
						// Cast the pointer at the back of the vector to an Boolean pointer type
						Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(operands.back());

						// Get the value of the bool (true or false)
						firstBool = boolPtr->get_value();

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}
					else if ( i == 2)
					{
						// Flag to say that there are two boolean values
						isSecondBoolValue = true;

						// Cast the pointer at the back of the vector to an Boolean pointer type
						Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(operands.back());
						
						// Get the value of the bool (true or false)
						secondBool = boolPtr->get_value();

						// Pop the pointer off the vector and continue through the loop
						operands.pop_back();
						continue;
					}
				}
				else if (is<Variable>(operands.back()))
				{
					// Flag to say that it is a variable that may be assigned a value
					boolVariables = true;

					if (i == 1)
					{
						// Flag to say which variable is being used
						variableFirst = true;
						
						// Cast the Variable to variable pointer to be able to empty the stack
						variable = dynamic_pointer_cast<Variable>(operands.back());

						if (Real::pointer_type real = dynamic_pointer_cast<Real>(variable->get_value()))
						{
							isReal = true;
						}

						// Pop the pointer off the vector(stack) and continue through the loop
						operands.pop_back();
						continue;
					}
					else if (i == 2)
					{
						// Flag to say which variable is being used
						variableSecond = true;

						// Cast the Variable to variable pointer to be able to empty the stack
						secondVariable = dynamic_pointer_cast<Variable>(operands.back());

						if (Real::pointer_type real = dynamic_pointer_cast<Real>(secondVariable->get_value()))
						{
							isReal = true;
						}

						// Pop the pointer off the vector(stack) and continue through the loop
						operands.pop_back();
						continue;
					}
				}
			}

			// If Operation is NonAssociative (i.e a Unary Operator)
			if (is<NonAssociative>(operationPtr))
			{
				if (is<Negation>(operationPtr))
				{
					if (boolVariables == false)
					{
						// If there is more than one operand, the throw an error stating there
						// are too many operands
						if (isSecondNum == true || isBoolValue == true)
							throw exception("Error: too many operands");
						else
						{
							// Check if it is a Real number or not, multiple by -1 for a sign change
							// then push back onto the operands vector
							if (isReal == false)
							{
								operandNumberFirst *= -1;
								operands.push_back(make<Integer>(operandNumberFirst));
								evaluated = true;
								continue;
							}
							else
							{
								operandRealNumberFirst *= -1.0;
								operands.push_back(make<Real>(operandRealNumberFirst));
								evaluated = true;
								continue;
							}
						}
					}
					else if (boolVariables == true)
					{
						if (Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(variable->get_value()))
						{
							Integer::value_type newIntPtr = intPtr->get_value() * -1;
							Token::pointer_type intToken = make<Integer>(newIntPtr);
							Integer::pointer_type intPtrVariable = dynamic_pointer_cast<Integer>(intToken);
							Variable::pointer_type newIntVar = dynamic_pointer_cast<Variable>(intToken);
							newIntVar->set_value(intPtrVariable);
							operands.push_back(newIntVar);
							evaluated = true;
							continue;
						}
						else if (Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value()))
						{
							Real::value_type newRealPtr = realPtr->get_value() * -1;
							Token::pointer_type realToken = make<Real>(newRealPtr);
							Real::pointer_type realPtrVariable = dynamic_pointer_cast<Real>(realToken);
							Variable::pointer_type newRealVar = dynamic_pointer_cast<Variable>(realToken);
							newRealVar->set_value(realPtrVariable);
							operands.push_back(newRealVar);
							evaluated = true;
							continue;
						}
					}
				}
				else if (is<Identity>(operationPtr))
				{
					// Check if it is a Real number or not, then push back onto the operands vector
					if (isReal == false)
					{
						operands.push_back(make<Integer>(operandNumberFirst));
						evaluated = true;
						continue;
					}
					else
					{
						operands.push_back(make<Real>(operandRealNumberFirst));
						evaluated = true;
						continue;
					}
				}
				else if (is<Not>(operationPtr))
				{
					//Variable::pointer_type newVariable = nullptr;
					// Check that there is only bool value to operate on
					if (isSecondBoolValue == true)
					{
						throw exception("Error: too many operands");
					}
					else
					{
						// Check to see if there is a bool, or numeric value to operate on
						// If it is a bool value, then change the value to the opposite of its
						// current value. If it is a numeric value greater than 0, then the value
						// will be changed to 0, otherwise if the value is 0, then the value will 
						// be changed to the value of 1 for Integer or 1.0 for Real
						if (isBoolValue == true && firstBool == false)
						{
							firstBool = true;
							operands.push_back(make<Boolean>(firstBool));
							evaluated = true;
							continue;
						}
						else if (isBoolValue == true && firstBool == true)
						{
							firstBool = false;
							operands.push_back(make<Boolean>(firstBool));
							evaluated = true;
							continue;
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							if (boolPtr->get_value() == true)
							{
								// Convert value to a token pointer, make a New variable token pointer which will then take 
								// the value of the first token pointer you have converted and sets the New variable token
								// pointer value to the pointer type that you have converted to.
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								Token::pointer_type newVar = make<Variable>();
								Variable::pointer_type newVariable = dynamic_pointer_cast<Variable>(newVar);
								newVariable->set_value(newBoolValue);
								operands.push_back(newVariable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false)
							{
								// Convert value to a token pointer, make a New variable token pointer which will then take 
								// the value of the first token pointer you have converted and sets the New variable token
								// pointer value to the pointer type that you have converted to.
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								Token::pointer_type newVar = make<Variable>();
								Variable::pointer_type newVariable = dynamic_pointer_cast<Variable>(newVar);
								newVariable->set_value(newBoolValue);
								operands.push_back(newVariable);
								evaluated = true;
								continue;
							}
						}			
					}
				}
				else if (is<PostfixOperator>(operationPtr) || is<Factorial>(operationPtr))
				{
					// Check that there are not two operands for the postfix(factorial) to operate on
					if (isSecondNum == true)
					{
						throw exception("Error: too many operands");
					}
					if (boolVariables == false)
					{
						if (isReal == false)
						{
							// Factorial Calculation for an Integer. It uses the operandNumberFirst variable
							// to tell the loop how many times to iterate through the loop.
							Integer::value_type accumlatedFactorial = 1;
							for (Integer::value_type i = 2; i <= operandNumberFirst; ++i)
								accumlatedFactorial *= i;

							// Push the calculated factorial onto the vector
							operands.push_back(make<Integer>(accumlatedFactorial));
							evaluated = true;
							continue;
						}
						else if (isReal == true)
						{
							// Factorial Calculation for a Real numberr. It uses the operandRealNumberFirst variable
							// to tell the loop how many times to iterate through the loop.
							Real::value_type accumlatedRealFactorial = 1.0;
							for (Real::value_type i = 2.0; i <= operandRealNumberFirst; ++i)
								accumlatedRealFactorial *= i;

							// Push the calculated factorial onto the vector
							operands.push_back(make<Real>(accumlatedRealFactorial));
							evaluated = true;
							continue;
						}
					}
					else if (boolVariables == true)
					{
						if (Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(variable->get_value()))
						{
							Integer::value_type newIntPtr = intPtr->get_value();

							// Factorial Calculation for an Integer. It uses the operandNumberFirst variable
							// to tell the loop how many times to iterate through the loop.
							Integer::value_type accumlatedFactorial = 1;
							for (Integer::value_type i = 2; i <= newIntPtr; ++i)
								accumlatedFactorial *= i;

							// Convert value to a token pointer, make a New variable token pointer which will then take 
								// the value of the first token pointer you have converted and sets the New variable token
								// pointer value to the pointer type that you have converted to.
							Token::pointer_type intToken = make<Integer>(accumlatedFactorial);
							Integer::pointer_type intPtrVariable = dynamic_pointer_cast<Integer>(intToken);
							Variable::pointer_type newIntVar = dynamic_pointer_cast<Variable>(intToken);
							newIntVar->set_value(intPtrVariable);
							operands.push_back(newIntVar);
							evaluated = true;
							continue;
						}
						else if (Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value()))
						{
							Real::value_type newRealPtr = realPtr->get_value();

							// Factorial Calculation for a Real numberr. It uses the operandRealNumberFirst variable
							// to tell the loop how many times to iterate through the loop.
							Real::value_type accumlatedRealFactorial = 1.0;
							for (Real::value_type i = 2.0; i <= newRealPtr; ++i)
								accumlatedRealFactorial *= i;

							// Convert value to a token pointer, make a New variable token pointer which will then take 
								// the value of the first token pointer you have converted and sets the New variable token
								// pointer value to the pointer type that you have converted to.
							Token::pointer_type realToken = make<Real>(accumlatedRealFactorial);
							Real::pointer_type realPtrVariable = dynamic_pointer_cast<Real>(realToken);
							Variable::pointer_type newRealVar = dynamic_pointer_cast<Variable>(realToken);
							newRealVar->set_value(realPtrVariable);
							operands.push_back(newRealVar);
							evaluated = true;
							continue;
						}
					}
				}
			}

			// Check if operation is a Binary operator
			if (is<BinaryOperator>(operationPtr))
			{
				// Check if the operator is the Power operation (2^4) = 16
				if (is<Power>(operationPtr))
				{
					// Variables to hold the lexical cast numbers
					Real::value_type resultRealPower;
					Real::value_type valueRealFirst;
					Real::value_type valueRealSecond;

					Integer::value_type resultIntegerPower;
					Integer::value_type valueIntegerFirst;
					Integer::value_type valueIntegerSecond;

					// Check to see if the operands are only Integers
					if (isReal == false)
					{
						// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
						// operand numbers to be able to use the pow() function;					
						int powerNumber = boost::lexical_cast<int>(operandNumberFirst);

						if (powerNumber < 0)
						{
							// Cast the base number to a Real number because if the exponent number(powerNum) is
							// a negative number then the result will be a Real number
							valueRealSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
							Token::pointer_type realNumber = make<Real>(valueRealSecond);
							valueRealSecond = get_value<Real>(realNumber);

							resultRealPower = boost::multiprecision::pow(valueRealSecond, powerNumber);

							// Push the real number result onto the operand stack(vector)
							operands.push_back(make<Real>(resultRealPower));
							evaluated = true;
							continue;
						}
						else if (powerNumber > 0)
						{
							// Calculate the power
							resultIntegerPower = boost::multiprecision::pow(operandNumberSecond, powerNumber);
							
							// Push the integer Result onto the operand stack(vector)
							operands.push_back(make<Integer>(resultIntegerPower));
							evaluated = true;
							continue;
						}
						else if (powerNumber == 0)
						{
							// Push the integer Result onto the operand stack(vector)
							operands.push_back(make<Integer>(1));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == true)
					{

						if (boolRealNumberFirst == true && boolNumberSecond == true)
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandRealNumberFirst);

							valueRealFirst = boost::lexical_cast<Real::value_type>(operandNumberSecond);
							Token::pointer_type realNumber = make<Real>(valueRealFirst);
							valueRealFirst = get_value<Real>(realNumber);

							Real::value_type realResult = boost::multiprecision::pow(valueRealFirst, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else if (boolNumberFirst == true && boolRealNumberSecond == true)
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandNumberFirst);

							Real::value_type realResult = pow(operandRealNumberSecond, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else if (boolRealNumberSecond == true && boolRealNumberFirst == true)
						{
							// Get the result of the power of 2 real numbers
							Real::value_type realResult = boost::multiprecision::pow(operandRealNumberSecond, operandRealNumberFirst);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandRealNumberFirst);

							Real::value_type realResult = pow(operandRealNumberSecond, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
					}
				}

				if (is<Assignment>(operationPtr))
				{
					if ((boolNumberFirst == true || boolRealNumberFirst == true  ) && 
						(boolNumberSecond == true || boolRealNumberSecond == true))
					{
						throw exception("Error: Cannot assign to a literal value.");
					}

					if (boolVariables == true)
					{
						if (boolNumberFirst == true)
						{
							// Convert the operand value to a Token pointer, then to an Integer value
							Token::pointer_type operandToken = make<Integer>(operandNumberFirst);
							Integer::pointer_type integerVariable = dynamic_pointer_cast<Integer>(operandToken);

							// Set the variable Variable::pointer_type value to an Integer::pointer_type, because the 
							// value of the Variable::pointer_type is a Operand::pointer_type
							if (variableFirst == true)
							{
								variable->set_value(integerVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(variable);
							}
							else if (variableSecond == true)
							{
								secondVariable->set_value(integerVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(secondVariable);
							}
							continue;
						}
						else if (boolRealNumberFirst == true)
						{
							// Convert the operand value to a Token pointer, then to an Real number value
							Token::pointer_type operandToken = make<Real>(operandRealNumberFirst);
							Real::pointer_type realVariable = dynamic_pointer_cast<Real>(operandToken);

							// Set the variable Variable::pointer_type value to an Real::pointer_type, because the 
							// value of the Variable::pointer_type is a Operand::pointer_type
							if (variableFirst == true)
							{
								variable->set_value(realVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(variable);
							}
							else if (variableSecond == true)
							{
								secondVariable->set_value(realVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(secondVariable);
							}
							continue;
						}
						else if (isBoolValue == true)
						{
							// Convert the operand value to a Token pointer, then to an Boolean value
							Token::pointer_type operandToken = make<Boolean>(firstBool);
							Boolean::pointer_type booleanVariable = dynamic_pointer_cast<Boolean>(operandToken);

							// Set the variable Variable::pointer_type value to an Boolean::pointer_type, because the 
							// value of the Variable::pointer_type is a Operand::pointer_type
							if (variableFirst == true)
							{
								variable->set_value(booleanVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(variable);
							}
							else if (variableSecond == true)
							{
								secondVariable->set_value(booleanVariable);

								// Push it onto the operands stack(vector)
								operands.push_back(secondVariable);
							}
							continue;
						}
					}
					else if (!boolVariables && (boolNumberFirst == true || boolRealNumberFirst == true || isBoolValue == true))
					{
						throw exception("Error: assignment to a non-variable.");
					}
				}

				if (is<Multiplicative>(operationPtr))
				{
					// If operator is multiplication, perform the operation one the operands
					if (is<Multiplication>(operationPtr))
					{
						if ((boolNumberFirst == true||boolRealNumberFirst == true || variableFirst == true) &&
							(boolNumberSecond == false && boolRealNumberSecond == false && variableSecond == false))
						{
							throw exception("Error: Not enough operands.");
						}
						
						if (boolVariables == false)
						{
							// If the operands are both Integer's perform the multiplication on it and push the product
							// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type product = operandNumberFirst * operandNumberSecond;
								operands.push_back(make<Integer>(product));
								evaluated = true;
								continue;
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type productReal;
								Real::value_type valueFirst;
								Real::value_type valueSecond;

								if (boolRealNumberFirst == true && boolNumberSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to use the multiply								
									valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
									Token::pointer_type realNumber = make<Real>(valueSecond);
									valueSecond = get_value<Real>(realNumber);

									productReal = valueSecond * operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(productReal));
									evaluated = true;
									continue;
								}
								else if (boolNumberFirst == true && boolRealNumberSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
									Token::pointer_type realNumber = make<Real>(valueFirst);
									valueFirst = get_value<Real>(realNumber);

									productReal = valueFirst * operandRealNumberSecond;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(productReal));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									productReal = operandRealNumberSecond * operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(productReal));
									evaluated = true;
									continue;
								}
							}
						}
						else if (boolVariables == true)
						{
							if (isReal == false )
							{ //*****
								if (boolNumberFirst == true && variableSecond == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(secondVariable->get_value());
									Integer::value_type product = operandNumberFirst * integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
								else if (boolNumberSecond == true && variableFirst == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(variable->get_value());
									Integer::value_type product = operandNumberSecond * integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type product;

								if ((boolRealNumberFirst == true || boolNumberFirst == true) && variableSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to use the multiply								
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(secondVariable->get_value());

									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if(boolRealNumberFirst == true)
										product = operandRealNumberFirst * realPtr->get_value();
									else if (boolNumberFirst == true)
									{
										Real::value_type valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
										Token::pointer_type realNumber = make<Real>(valueFirst);
										valueFirst = get_value<Real>(realNumber);
										product = valueFirst * realPtr->get_value();
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else if (variableFirst == true && (boolRealNumberSecond == true || boolNumberSecond== true))
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());


									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberSecond == true)
										product = operandRealNumberSecond * realPtr->get_value();
									else if (boolNumberSecond == true)
									{
										Real::value_type valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
										Token::pointer_type realNumber = make<Real>(valueSecond);
										valueSecond = get_value<Real>(realNumber);
										product = valueSecond * realPtr->get_value();
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									Real::pointer_type realPtrSecond = dynamic_pointer_cast<Real>(secondVariable->get_value());
									Real::value_type product = realPtrSecond->get_value() * realPtr->get_value();

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
							}
						}
					}
					
					// If operator is divison, perform the operation on the operands
					if (is<Division>(operationPtr))
					{
						if ((boolNumberFirst == true || boolRealNumberFirst == true || variableFirst == true) &&
							(boolNumberSecond == false && boolRealNumberSecond == false && variableSecond == false))
						{
							throw exception("Error: Not enough operands.");
						}
						if (boolVariables == false)
						{
							// If the operands are both Integer's perform the divison on it and push the quotient
							// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type quotient = operandNumberSecond / operandNumberFirst;
								operands.push_back(make<Integer>(quotient));
								continue;
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type valueFirst;
								Real::value_type valueSecond;
								Real::value_type quotientReal;

								if (boolRealNumberFirst == true && boolNumberSecond == true)
								{
									// Variables to hold the result of the divison operations, and to convert(lexical_cast) the
									// operand numbers to be able to use divison;
									valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
									Token::pointer_type realNumber = make<Real>(valueSecond);
									valueSecond = get_value<Real>(realNumber);

									quotientReal = valueSecond / operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(quotientReal));
									continue;
								}
								else if (boolNumberFirst == true && boolRealNumberSecond == true)
								{
									// Variables to hold the result of the divison operations, and to convert(lexical_cast) the
									// operand numbers to be able to use divison;
									valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
									Token::pointer_type realNumber = make<Real>(valueFirst);
									valueFirst = get_value<Real>(realNumber);

									quotientReal = operandRealNumberSecond / valueFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(quotientReal));
									continue;
								}
								else
								{
									// Variables to hold the result of the divison operations, and to convert(lexical_cast) the
									// operand numbers to be able to divison;
									quotientReal = operandRealNumberSecond / operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(quotientReal));
									continue;
								}
							}
						}
						else if (boolVariables == true)
						{
							if (isReal == false)
							{ //*****
								if (boolNumberFirst == true && variableSecond == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(secondVariable->get_value());
									Integer::value_type product = integerPtr->get_value() / operandNumberFirst;
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
								else if (boolNumberSecond == true && variableFirst == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(variable->get_value());
									Integer::value_type product = operandNumberSecond / integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type product;

								if ((boolRealNumberFirst == true || boolNumberFirst == true) && variableSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to use the multiply								
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(secondVariable->get_value());

									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberFirst == true)
										product = realPtr->get_value() / operandRealNumberFirst;
									else if (boolNumberFirst == true)
									{
										Real::value_type valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
										Token::pointer_type realNumber = make<Real>(valueFirst);
										valueFirst = get_value<Real>(realNumber);
										product = realPtr->get_value() / valueFirst;
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else if (variableFirst == true && (boolRealNumberSecond == true || boolNumberSecond == true))
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());


									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberSecond == true)
										product = operandRealNumberSecond * realPtr->get_value();
									else if (boolNumberSecond == true)
									{
										Real::value_type valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
										Token::pointer_type realNumber = make<Real>(valueSecond);
										valueSecond = get_value<Real>(realNumber);
										product = valueSecond * realPtr->get_value();
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									Real::pointer_type realPtrSecond = dynamic_pointer_cast<Real>(secondVariable->get_value());
									Real::value_type product = realPtrSecond->get_value() * realPtr->get_value();

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
							}
						}
					}

					// If operator is modulus, perform the operation on the operands
					if (is<Modulus>(operationPtr))
					{
						if (boolVariables == false)
						{
							// If the operands are both Integer's perform the modulus on it and push the reslt
							// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type modulus = operandNumberSecond % operandNumberFirst;
								operands.push_back(make<Integer>(modulus));
								evaluated = true;
								continue;
							}
							else if (isReal == true)
							{
								throw exception("Error: cannot perform <Modulus>");
							}
						}
						else if (boolVariables == true)
						{
							// If the operands are both Integer's perform the modulus on it and push the reslt
							// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type modulus;

								if (boolNumberFirst == true )
								{
									if (Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(secondVariable->get_value()))
									{
										modulus = intPtr->get_value() % operandNumberFirst;
										operands.push_back(make<Integer>(modulus));
										evaluated = true;
										continue;
									}
								}
								else if (boolNumberSecond == true)
								{
									if (Integer::pointer_type intPtr = dynamic_pointer_cast<Integer>(variable->get_value()))
									{
										modulus = operandNumberSecond & intPtr->get_value();
										operands.push_back(make<Integer>(modulus));
										evaluated = true;
										continue;
									}
								}
							}
							else if (isReal == true)
							{
								throw exception("Error: cannot perform <Modulus>");
							}
						}
					}
				}

				if (is<Additive>(operationPtr))
				{
					if ((boolNumberFirst == true || boolRealNumberFirst == true || variableFirst == true) &&
						(boolNumberSecond == false && boolRealNumberSecond == false && variableSecond == false))
					{
						throw exception("Error: Not enough operands.");
					}

					if (is<Addition>(operationPtr))
					{
						if (boolVariables == false)
						{
							// If the operands are both Integer's perform the addition on it and push the result
								// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type result = operandNumberFirst + operandNumberSecond;
								operands.push_back(make<Integer>(result));
								evaluated = true;
								continue;
							}
							else if (isReal == true)
							{
								Real::value_type resultReal;
								Real::value_type valueFirst;
								Real::value_type valueSecond;

								if (boolRealNumberFirst == true && boolNumberSecond == true)
								{
									// Variables to hold the result of the addition operations, and to convert(lexical_cast) the
									// operand numbers to be able to use addition properly;
									valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
									Token::pointer_type realNumber = make<Real>(valueSecond);
									valueSecond = get_value<Real>(realNumber);

									resultReal = valueSecond + operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
								else if (boolNumberFirst == true && boolRealNumberSecond == true)
								{
									// Variables to hold the result of the addition operations, and to convert(lexical_cast) the
									// operand numbers to be able to use addition properly;
									valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
									Token::pointer_type realNumber = make<Real>(valueFirst);
									valueFirst = get_value<Real>(realNumber);

									resultReal = operandRealNumberSecond + valueFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the addition operations, and to convert(lexical_cast) the
									// operand numbers to be able to use addition properly;
									resultReal = operandRealNumberSecond + operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
							}
						}
						else if (boolVariables == true)
						{
							if (isReal == false)
							{ 
								if (boolNumberFirst == true && variableSecond == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(secondVariable->get_value());
									Integer::value_type product = operandNumberFirst + integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
								else if (boolNumberSecond == true && variableFirst == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(variable->get_value());
									Integer::value_type product = operandNumberSecond + integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type productReal;
								Real::value_type product;

								if ((boolRealNumberFirst == true || boolNumberFirst == true)&& variableSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to use the multiply								
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(secondVariable->get_value());

									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberFirst == true)
										product = realPtr->get_value() + operandRealNumberFirst;
									else if (boolNumberFirst == true)
									{
										Real::value_type valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
										Token::pointer_type realNumber = make<Real>(valueFirst);
										valueFirst = get_value<Real>(realNumber);
										product = realPtr->get_value() + valueFirst;
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else if (variableFirst == true && (boolRealNumberSecond == true || boolNumberSecond == true))
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									
									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberSecond == true)
										product = operandRealNumberSecond + realPtr->get_value();
									else if (boolNumberSecond == true)
									{
										Real::value_type valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
										Token::pointer_type realNumber = make<Real>(valueSecond);
										valueSecond = get_value<Real>(realNumber);
										product = valueSecond + realPtr->get_value();
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									Real::pointer_type realPtrSecond = dynamic_pointer_cast<Real>(secondVariable->get_value());
									Real::value_type product = realPtrSecond->get_value() + realPtr->get_value();

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
							}
						}
					}

					if (is<Subtraction>(operationPtr))
					{
						if ((boolNumberFirst == true || boolRealNumberFirst == true || variableFirst == true) &&
							(boolNumberSecond == false && boolRealNumberSecond == false && variableSecond == false))
						{
							throw exception("Error: Not enough operands.");
						}

						if (boolVariables == false)
						{
							// If the operands are both Integer's perform the subtraction on it and push the result
								// onto the stack(vector)
							if (isReal == false)
							{
								Integer::value_type result = operandNumberSecond - operandNumberFirst;
								operands.push_back(make<Integer>(result));
								evaluated = true;
								continue;
							}
							else if (isReal == true)
							{
								Real::value_type resultReal;
								Real::value_type valueFirst;
								Real::value_type valueSecond;

								if (boolRealNumberFirst == true && boolNumberSecond == true)
								{
									// Variables to hold the result of the subtraction operations, and to convert(lexical_cast) the
									// operand numbers to be able to use subtraction properly;
									valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
									Token::pointer_type realNumber = make<Real>(valueSecond);
									valueSecond = get_value<Real>(realNumber);

									resultReal = valueSecond - operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
								else if (boolNumberFirst == true && boolRealNumberSecond == true)
								{
									// Variables to hold the result of the addition operations, and to convert(lexical_cast) the
									// operand numbers to be able to use addition properly;
									valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
									Token::pointer_type realNumber = make<Real>(valueFirst);
									valueFirst = get_value<Real>(realNumber);

									resultReal = operandRealNumberSecond - valueFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the addition operations, and to convert(lexical_cast) the
									// operand numbers to be able to use addition properly;
									resultReal = operandRealNumberSecond - operandRealNumberFirst;

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(resultReal));
									evaluated = true;
									continue;
								}
							}
						}
						else if (boolVariables == true)
						{
							if (isReal == false)
							{ //*****
								if (boolNumberFirst == true && variableSecond == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(secondVariable->get_value());
									Integer::value_type product = operandNumberFirst - integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
								else if (boolNumberSecond == true && variableFirst == true)
								{
									Integer::pointer_type integerPtr = dynamic_pointer_cast<Integer>(variable->get_value());
									Integer::value_type product = operandNumberSecond - integerPtr->get_value();
									operands.push_back(make<Integer>(product));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == true)
							{
								// Variables to hold the real number values
								Real::value_type productReal;
								Real::value_type product;

								if ((boolRealNumberFirst == true || boolNumberFirst == true)&& variableSecond == true)
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to use the multiply								
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(secondVariable->get_value());

									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberFirst == true)
										product = realPtr->get_value() + operandRealNumberFirst;
									else if (boolNumberFirst == true)
									{
										Real::value_type valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
										Token::pointer_type realNumber = make<Real>(valueFirst);
										valueFirst = get_value<Real>(realNumber);
										product = realPtr->get_value() + valueFirst;
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else if (variableFirst == true && (boolRealNumberSecond == true || boolNumberSecond == true))
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									
									// If both oerands are real numbers than just multiply, else if one is a integer
									// and one is a real convert the integer to a real number
									if (boolRealNumberSecond == true)
										product = operandRealNumberSecond * realPtr->get_value();
									else if (boolNumberSecond == true)
									{
										Real::value_type valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
										Token::pointer_type realNumber = make<Real>(valueSecond);
										valueSecond = get_value<Real>(realNumber);
										product = valueSecond * realPtr->get_value();
									}

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
								else
								{
									// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
									// operand numbers to be able to multiply
									Real::pointer_type realPtr = dynamic_pointer_cast<Real>(variable->get_value());
									Real::pointer_type realPtrSecond = dynamic_pointer_cast<Real>(secondVariable->get_value());
									Real::value_type product = realPtrSecond->get_value() - realPtr->get_value();

									// Push the result onto the operands stack(vector)
									operands.push_back(make<Real>(product));
									evaluated = true;
									continue;
								}
							}
						}
					}
				}

				// If comparing Boolean values
				if (is<LogOr>(operationPtr) || is<LogXor>(operationPtr) || is<LogAnd>(operationPtr))
				{
					if (is<And>(operationPtr))
					{
						if (boolVariables == false)
						{
							// Check the boolean values, both must be true to return true,
							// push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								
								if(operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								
								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								
								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
						}
					}

					if (is<Nand>(operationPtr))
					{
						if (boolVariables == false)
						{
							// Check the boolean values, both must be true to return false,
							// push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
						}
					}

					if (is<Nor>(operationPtr))
					{
						if (boolVariables == false)
						{
							// Check the boolean values, both must be false to return true,
							// push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
						}
					}

					if (is<Or>(operationPtr))
					{
						if (isBoolValue == true)
						{
							// Check the boolean values, one must be true to return true,
							// push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());
							
							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
						}
					}

					if (is<Xor>(operationPtr))
					{
						if (boolVariables == false)
						{
							// Check the boolean values, one must be true and one must be false
							// to return true, push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
						}
					}
					if (is<Xnor>(operationPtr))
					{
						if (boolVariables == false)
						{
							// Check the boolean values, one must be true and one must be false
							// to return true, push result onto operands stack(vector)
							if (firstBool == true && secondBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == true && secondBool == false)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == false)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
						}
						else if (boolVariables == true)
						{
							Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
							Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

							if (boolPtr->get_value() == true && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == true)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == true && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
							else if (boolPtr->get_value() == false && boolSecondPtr->get_value() == false)
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);

								if (operands.size() > 0)
									operands.push_back(variable);
								else if (operands.size() == 0)
								{
									operands.push_back(newBoolValue);
								}

								evaluated = true;
								continue;
							}
						}
					}
				}

				if (is<Equality>(operationPtr))
				{
					Real::value_type resultReal;
					Real::value_type valueFirst;
					Real::value_type valueSecond;

					if (isBoolValue == true)
					{
						// Check the boolean values, both must be the same value 
						// to return true, push result onto operands stack(vector)
						if ((firstBool == true && secondBool == true) || (firstBool == false && secondBool == false))
						{
							operands.push_back(make<Boolean>(true));
							evaluated = true;
							continue;
						}
						else if (firstBool == false || secondBool == false)
						{
							operands.push_back(make<Boolean>(false));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == false && boolVariables == false)
					{
						if (operandNumberFirst == operandNumberSecond)
						{
							operands.push_back(make<Boolean>(true));
							evaluated = true;
							continue;
						}
						else if (operandNumberFirst != operandNumberSecond)
						{
							operands.push_back(make<Boolean>(false));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == true)
					{
						if (boolRealNumberFirst == true && boolNumberSecond == true)
						{
							// Variables to hold the result of the equality operation, and to convert(lexical_cast) the
							// operand numbers to be able to use the equality operation;
							valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
							Token::pointer_type realNumber = make<Real>(valueSecond);
							valueSecond = get_value<Real>(realNumber);

							if (operandRealNumberFirst == valueSecond)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (boolRealNumberSecond == true && boolNumberFirst == true)
						{
							// Variables to hold the result of the equality operation, and to convert(lexical_cast) the
							// operand numbers to be able to use the equality operation;
							valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
							Token::pointer_type realNumber = make<Real>(valueFirst);
							valueFirst = get_value<Real>(realNumber);

							if (valueFirst == operandRealNumberSecond)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (operandRealNumberFirst == operandRealNumberSecond)
						{
							operands.push_back(make<Boolean>(true));
							evaluated = true;
							continue;
						}
						else
						{
							operands.push_back(make<Boolean>(false));
							evaluated = true;
							continue;
						}
					}
					else if (boolVariables == true)
					{
						Boolean::pointer_type boolPtr = nullptr;
						Boolean::pointer_type boolSecondPtr = nullptr;
						
						if(variableFirst == true)
							boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
						if(variableSecond == true)
							boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

						if (variableFirst == true && variableSecond == true)
						{
							if ((boolPtr->get_value() == true && boolSecondPtr->get_value() == true) ||
								(boolPtr->get_value() == false && boolSecondPtr->get_value() == false))
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
						}
						else if (variableFirst == true && secondBool == true)
						{
							if ((boolPtr->get_value() == true && secondBool == true) ||
								(boolPtr->get_value() == false && secondBool == false))
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
						}
						else if (variableSecond == true && firstBool == true)
						{
							if ((boolSecondPtr->get_value() == true && firstBool == true) ||
								(boolSecondPtr->get_value() == false && firstBool == false))
							{
								Token::pointer_type boolToken = make<Boolean>(true);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
							else
							{
								Token::pointer_type boolToken = make<Boolean>(false);
								Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
								variable->set_value(newBoolValue);
								operands.push_back(variable);
								evaluated = true;
								continue;
							}
						}
					}
				}

				if (is<Inequality>(operationPtr))
				{
					// Variables to hold the lexical_cast numbers
					Real::value_type resultReal;
					Real::value_type valueFirst;
					Real::value_type valueSecond;

					if (boolVariables == false)
					{
						if (isBoolValue == true)
						{
							// Check the boolean values, both must be the same value 
							// to return true, push result onto operands stack(vector)
							if ((firstBool == true && secondBool == true) || (firstBool == false && secondBool == false))
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if ((firstBool == false && secondBool == true) || (firstBool == true && secondBool == false))
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == false)
						{
							if (operandNumberFirst != operandNumberSecond)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (operandNumberFirst == operandNumberSecond)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == true)
						{
							if (boolRealNumberFirst == true && boolNumberSecond == true)
							{
								// Variables to hold the result of the inequality operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the inequality operation;
								valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
								Token::pointer_type realNumber = make<Real>(valueSecond);
								valueSecond = get_value<Real>(realNumber);

								if (operandRealNumberFirst != valueSecond)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (boolRealNumberSecond == true && boolNumberFirst == true)
							{
								// Variables to hold the result of the inequality operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the inequality operation;
								valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
								Token::pointer_type realNumber = make<Real>(valueFirst);
								valueFirst = get_value<Real>(realNumber);

								if (valueFirst != operandRealNumberSecond)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (operandRealNumberFirst != operandRealNumberSecond)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
					}
					else if (boolVariables == true)
					{
						Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
						Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

						if ((boolPtr->get_value() == true && boolSecondPtr->get_value() == true) ||
							(boolPtr->get_value() == false && boolSecondPtr->get_value() == false))
						{
							Token::pointer_type boolToken = make<Boolean>(false);
							Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
							variable->set_value(newBoolValue);
							operands.push_back(variable);
							evaluated = true;
							continue;
						}
						else
						{
							Token::pointer_type boolToken = make<Boolean>(true);
							Boolean::pointer_type newBoolValue = dynamic_pointer_cast<Boolean>(boolToken);
							variable->set_value(newBoolValue);
							operands.push_back(variable);
							evaluated = true;
							continue;
						}
					}
				}

				// If the boolean values are being related to eachother >, <, <=, >=
				if (is<Relational>(operationPtr))
				{
					// Variables to hold the lexical_cast numbers
					Real::value_type resultReal;
					Real::value_type valueFirst;
					Real::value_type valueSecond;

					// Returns true only when user inputs true > false, or a value1 > value2
					if (is<Greater>(operationPtr))
					{
						if (boolVariables == false)
						{
							if (isBoolValue == true)
							{
								if ((firstBool == true && secondBool == true) || (firstBool == false && secondBool == false)
									|| (firstBool == true && secondBool == false))
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == false)
							{
								// check the integer values are greater than
								if (operandNumberSecond > operandNumberFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (isReal == true)
							{
								if (boolRealNumberFirst == true && boolNumberSecond == true)
								{
									// Variables to hold the result of the greater than operation, and to convert(lexical_cast) the
									// operand numbers to be able to use the greater than operation;
									valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
									Token::pointer_type realNumber = make<Real>(valueSecond);
									valueSecond = get_value<Real>(realNumber);

									if (valueSecond > operandRealNumberFirst)
									{
										operands.push_back(make<Boolean>(true));
										evaluated = true;
										continue;
									}
									else
									{
										operands.push_back(make<Boolean>(false));
										evaluated = true;
										continue;
									}
								}
								else if (boolRealNumberSecond == true && boolNumberFirst == true)
								{
									// Variables to hold the result of the greater than operation, and to convert(lexical_cast) the
									// operand numbers to be able to use the greater than operation;
									valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
									Token::pointer_type realNumber = make<Real>(valueFirst);
									valueFirst = get_value<Real>(realNumber);

									if (operandRealNumberSecond > valueFirst)
									{
										operands.push_back(make<Boolean>(true));
										evaluated = true;
										continue;
									}
									else
									{
										operands.push_back(make<Boolean>(false));
										evaluated = true;
										continue;
									}
								}
								else if (operandRealNumberSecond > operandRealNumberFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
						}
						if (boolVariables == true)
						{
							if (isBoolValue == true)
							{
								Boolean::pointer_type boolPtr = dynamic_pointer_cast<Boolean>(variable->get_value());
								Boolean::pointer_type boolSecondPtr = dynamic_pointer_cast<Boolean>(secondVariable->get_value());

								if ((firstBool == true && boolSecondPtr->get_value() == true) || (firstBool == false && boolSecondPtr->get_value() == false)
									|| (firstBool == true && boolSecondPtr->get_value() == false) ||(secondBool == true && boolPtr->get_value() == true) 
									|| (secondBool == false && boolPtr->get_value() == false) || (secondBool == false && boolPtr->get_value() == true))
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
							}
						}
					}
					else if (is<GreaterEqual>(operationPtr))
					{
						// Check if the boolean values are greater than or equal to each other(true is > than false)
						if (isBoolValue == true)
						{
							if ((firstBool == true && secondBool == true) || (secondBool == true && firstBool == false)
								|| (secondBool == false && firstBool == false))
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == false)
						{
							// check the integer values are greater than or equal
							if (operandNumberSecond >= operandNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == true)
						{
							if (boolRealNumberFirst == true && boolNumberSecond == true)
							{
								// Variables to hold the result of the inequality operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the inequality operation;
								valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
								Token::pointer_type realNumber = make<Real>(valueSecond);
								valueSecond = get_value<Real>(realNumber);

								if (valueSecond >= operandRealNumberFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (boolRealNumberSecond == true && boolNumberFirst == true)
							{
								// Variables to hold the result of the inequality operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the inequality operation;
								valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
								Token::pointer_type realNumber = make<Real>(valueFirst);
								valueFirst = get_value<Real>(realNumber);

								if (operandRealNumberSecond >= valueFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (operandRealNumberSecond >= operandRealNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
					}
					else if (is<Less>(operationPtr))
					{
						if (isBoolValue == true)
						{
							// Check the boolean values if they are less than. Only false is less than true
							if ((firstBool == true && secondBool == true) || (firstBool == false && secondBool == true)
								|| (firstBool == false && secondBool == false))
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
							else if (secondBool == false && firstBool == true)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == false)
						{
							// check the integer values are less than
							if (operandNumberSecond < operandNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == true)
						{
							if (boolRealNumberFirst == true && boolNumberSecond == true)
							{
								// Variables to hold the result of the less than operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the less than operation;
								valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
								Token::pointer_type realNumber = make<Real>(valueSecond);
								valueSecond = get_value<Real>(realNumber);

								if (valueSecond < operandRealNumberFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (boolRealNumberSecond == true && boolNumberFirst == true)
							{
								// Variables to hold the result of the less than operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the less than operation;
								valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
								Token::pointer_type realNumber = make<Real>(valueFirst);
								valueFirst = get_value<Real>(realNumber);

								if (operandRealNumberSecond < valueFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (operandRealNumberSecond < operandRealNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
					}
					else if (is<LessEqual>(operationPtr))
					{
						if (isBoolValue == true)
						{
							if ((secondBool == false && firstBool == true) || (firstBool == false && secondBool == false)
								|| (firstBool == true && secondBool == true))
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else if (firstBool == false && secondBool == true)
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == false)
						{
							// check the integer values are less than or equal
							if (operandNumberSecond <= operandNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
						else if (isReal == true)
						{
							if (boolRealNumberFirst == true && boolNumberSecond == true)
							{
								// Variables to hold the result of the less than or equal to operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the less than or equal to operation;
								valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
								Token::pointer_type realNumber = make<Real>(valueSecond);
								valueSecond = get_value<Real>(realNumber);

								if (valueSecond <= operandRealNumberFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (operandRealNumberSecond > 0.0 && operandNumberFirst > 0)
							{
								// Variables to hold the result of the less than or equal to operation, and to convert(lexical_cast) the
								// operand numbers to be able to use the less than or equal to operation;
								valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
								Token::pointer_type realNumber = make<Real>(valueFirst);
								valueFirst = get_value<Real>(realNumber);

								if (operandRealNumberSecond <= valueFirst)
								{
									operands.push_back(make<Boolean>(true));
									evaluated = true;
									continue;
								}
								else
								{
									operands.push_back(make<Boolean>(false));
									evaluated = true;
									continue;
								}
							}
							else if (operandRealNumberSecond <= operandRealNumberFirst)
							{
								operands.push_back(make<Boolean>(true));
								evaluated = true;
								continue;
							}
							else
							{
								operands.push_back(make<Boolean>(false));
								evaluated = true;
								continue;
							}
						}
					}
				}
			}
			// Check if the operation is a function
			if (is<Function>(operationPtr))
			{
				// If it is Absolute value perform the operation on it.
				if (is<Abs>(operationPtr))
				{
					if (isReal == false)
					{
						if (operandNumberFirst > 0)
						{
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}
						else if (operandNumberFirst < 0)
						{
							operandNumberFirst *= -1;
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == true)
					{
						if (operandRealNumberFirst > 0)
						{
							operands.push_back(make<Real>(operandRealNumberFirst));
							evaluated = true;
							continue;
						}
						else if (operandRealNumberFirst < 0)
						{
							operandRealNumberFirst *= -1.0;
							operands.push_back(make<Real>(operandRealNumberFirst));
							evaluated = true;
							continue;
						}
					}
				}
				else if (is<Fib>(operationPtr))
				{
					if (isReal == false)
					{
						// Variable to hold the returned value from the ifibonacci function
						Integer::value_type result = ifibonacci(operandNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Integer>(result));
						evaluated = true;
						continue;
					}
					else if (isReal == true)
					{
						// Variable to hold the returned value from the ifibonacci function
						Real::value_type resultReal = ifibonacci(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(resultReal));
						evaluated = true;
						continue;
					}
				}
				else if (is<Pel>(operationPtr))
				{
					if (isReal == false)
					{
						// Variable to hold the returned value from the pell function
						Integer::value_type result = pell(operandNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Integer>(result));
						evaluated = true;
						continue;
					}
					else if (isReal == true)
					{
						// Variable to hold the returned value from the pell function
						Real::value_type resultReal = pell(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(resultReal));
						evaluated = true;
						continue;
					}
				}
				else if (is<Syl>(operationPtr))
				{
					if (isReal == false)
					{
						// Variable to hold the returned value from the sylvester function
						Integer::value_type result = sylvester(operandNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Integer>(result));
						evaluated = true;
						continue;
					}
					else if (isReal == true)
					{
						// Variable to hold the returned value from the pell function
						Real::value_type resultReal = sylvester(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(resultReal));
						evaluated = true;
						continue;
					}
				}
				else if (is<Arccos>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = acos(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Arcsin>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = asin(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Arctan>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = atan(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Ceil>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = ceil(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Cos>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = cos(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Exp>(operationPtr))
				{
					 if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = exp(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Floor>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = floor(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Lb>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = log2(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Ln>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = log10(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Sin>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = sin(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Sqrt>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = sqrt(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Sin>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						//double tempFirstNum = boost::lexical_cast<double>(operandRealNumberFirst);
						Real::value_type result = sin(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Tan>(operationPtr))
				{
					if (isReal == true)
					{
						// Variable to hold the cast value and the returned value from the acos function
						Real::value_type result = tan(operandRealNumberFirst);

						// Push result onto the operands stack(vector)
						operands.push_back(make<Real>(result));
						evaluated = true;
						continue;
					}
				}
				else if (is<Arctan2>(operationPtr))
				{
				if (isReal == true)
				{
					// Variable to hold the cast value and the returned value from the acos function
					Real::value_type result = atan2(operandRealNumberSecond,operandRealNumberFirst);

					// Push result onto the operands stack(vector)
					operands.push_back(make<Real>(result));
					evaluated = true;
					continue;
				}
				}
				else if (is<Max>(operationPtr))
				{
					// Variables to hold the lexical_cast numbers
					Real::value_type resultReal;
					Real::value_type valueFirst;
					Real::value_type valueSecond;
					
					if(boolNumberFirst == true)
					{
						// Convert the first number to a real to be able to compare then values
						valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
						Token::pointer_type realNumber = make<Real>(valueFirst);
						valueFirst = get_value<Real>(realNumber);
					}
					
					if (boolNumberSecond == true)
					{
						valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
						Token::pointer_type realNumber = make<Real>(valueSecond);
						valueSecond = get_value<Real>(realNumber);
					}


					if (isReal == false)
					{
						if (operandNumberSecond > operandNumberFirst)
						{
							operands.push_back(make<Integer>(operandNumberSecond));
							evaluated = true;
							continue;
						}
						else if (operandNumberFirst > operandNumberSecond)
						{
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}
						else if (operandNumberSecond == operandNumberFirst)
						{
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}

					}
					else if (isReal == true)
					{
						if(boolRealNumberFirst == true && boolRealNumberSecond == true)
						{
							if (operandRealNumberSecond > operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberSecond < operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberFirst));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberSecond == operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
						}
						
						if(boolRealNumberSecond == true && boolNumberFirst == true)
						{
							if (operandRealNumberSecond > valueFirst)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberSecond < valueFirst)
							{
								operands.push_back(make<Real>(valueFirst));
								evaluated = true;
								continue;
							}
							else if (valueFirst == operandRealNumberSecond)
							{
								operands.push_back(make<Real>(valueFirst));
								evaluated = true;
								continue;
							}
						}
						
						if (boolNumberSecond == true && boolRealNumberFirst == true)
						{
							if (valueSecond > operandRealNumberFirst)
							{
								operands.push_back(make<Real>(valueSecond));
								evaluated = true;
								continue;
							}
							else if (valueSecond < operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberFirst));
								evaluated = true;
								continue;
							}
							else if ((valueSecond == operandRealNumberFirst))
							{
								operands.push_back(make<Real>(valueSecond));
								evaluated = true;
								continue;
							}
						}	
					}
				}
				else if (is<Min>(operationPtr))
				{
					// Variables to hold the lexical_cast numbers
					Real::value_type resultReal;
					Real::value_type valueFirst;
					Real::value_type valueSecond;

					if (boolNumberFirst == true)
					{
						// Convert the first number to a real to be able to compare then values
						valueFirst = boost::lexical_cast<Real::value_type>(operandNumberFirst);
						Token::pointer_type realNumber = make<Real>(valueFirst);
						valueFirst = get_value<Real>(realNumber);
					}

					if (boolNumberSecond == true)
					{
						// Convert the first number to a real to be able to compare then values
						valueSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
						Token::pointer_type realNumber = make<Real>(valueSecond);
						valueSecond = get_value<Real>(realNumber);
					}

					if (isReal == false)
					{
						if (operandNumberSecond < operandNumberFirst)
						{
							operands.push_back(make<Integer>(operandNumberSecond));
							evaluated = true;
							continue;
						}
						else if (operandNumberFirst < operandNumberSecond)
						{
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}
						else if (operandNumberSecond == operandNumberFirst)
						{
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}

					}
					else if (isReal == true)
					{
						if (boolRealNumberFirst == true && boolRealNumberSecond == true)
						{
							if (operandRealNumberSecond < operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberSecond > operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberFirst));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberSecond == operandRealNumberFirst)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
						}

						if (boolRealNumberFirst == true && boolNumberSecond == true)
						{
							if (operandRealNumberFirst < valueSecond)
							{
								operands.push_back(make<Real>(operandRealNumberFirst));
								evaluated = true;
								continue;
							}
							else if (operandRealNumberFirst > valueSecond)
							{
								operands.push_back(make<Real>(valueSecond));
								evaluated = true;
								continue;
							}
							else if (valueSecond == operandRealNumberFirst)
							{
								operands.push_back(make<Real>(valueSecond));
								evaluated = true;
								continue;
							}
						}
						if (boolNumberFirst == true && boolRealNumberSecond == true)
						{
							if (valueFirst < operandRealNumberSecond)
							{
								operands.push_back(make<Real>(valueFirst));
								evaluated = true;
								continue;
							}
							else if (valueFirst > operandRealNumberSecond)
							{
								operands.push_back(make<Real>(operandRealNumberSecond));
								evaluated = true;
								continue;
							}
							else if ((valueFirst == operandRealNumberSecond))
							{
								operands.push_back(make<Real>(valueFirst));
								evaluated = true;
								continue;
							}
						}
					}
				}
				// Check if the operator is the Power operation (2^4) = 16
				if (is<Pow>(operationPtr))
				{
					// Variables to hold the lexical cast numbers
					Real::value_type resultRealPower;
					Real::value_type valueRealFirst;
					Real::value_type valueRealSecond;

					Integer::value_type resultIntegerPower;
					Integer::value_type valueIntegerFirst;
					Integer::value_type valueIntegerSecond;

					// Check to see if the operands are only Integers
					if (isReal == false)
					{
						// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
						// operand numbers to be able to use the pow() function;					
						int powerNumber = boost::lexical_cast<int>(operandNumberFirst);

						if (powerNumber < 0)
						{
							// Cast the base number to a Real number because if the exponent number(powerNum) is
							// a negative number then the result will be a Real number
							valueRealSecond = boost::lexical_cast<Real::value_type>(operandNumberSecond);
							Token::pointer_type realNumber = make<Real>(valueRealSecond);
							valueRealSecond = get_value<Real>(realNumber);

							resultRealPower = boost::multiprecision::pow(valueRealSecond, powerNumber);

							// Push the real number result onto the operand stack(vector)
							operands.push_back(make<Real>(resultRealPower));
							evaluated = true;
							continue;
						}
						else if (powerNumber > 0)
						{
							// Calculate the power
							resultIntegerPower = boost::multiprecision::pow(operandNumberSecond, powerNumber);

							// Push the integer Result onto the operand stack(vector)
							operands.push_back(make<Integer>(resultIntegerPower));
							evaluated = true;
							continue;
						}
						else if (powerNumber == 0)
						{
							// Push the integer Result onto the operand stack(vector)
							operands.push_back(make<Integer>(1));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == true)
					{

						if (boolRealNumberFirst == true && boolNumberSecond == true)
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandRealNumberFirst);

							valueRealFirst = boost::lexical_cast<Real::value_type>(operandNumberSecond);
							Token::pointer_type realNumber = make<Real>(valueRealFirst);
							valueRealFirst = get_value<Real>(realNumber);

							Real::value_type realResult = boost::multiprecision::pow(valueRealFirst, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else if (boolNumberFirst == true && boolRealNumberSecond == true)
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandNumberFirst);

							Real::value_type realResult = pow(operandRealNumberSecond, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else if (boolRealNumberSecond == true && boolRealNumberFirst == true)
						{
							// Get the result of the power of 2 real numbers
							Real::value_type realResult = boost::multiprecision::pow(operandRealNumberSecond, operandRealNumberFirst);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
						else
						{
							// Variables to hold the result of the pow operations, and to convert(lexical_cast) the
							// operand numbers to be able to use the pow() function;
							int powerNumber = boost::lexical_cast<int>(operandRealNumberFirst);

							Real::value_type realResult = pow(operandRealNumberSecond, powerNumber);

							// Push the result onto the operands stack(vector)
							operands.push_back(make<Real>(realResult));
							evaluated = true;
							continue;
						}
					}
				}
				else if (is<Result>(operationPtr))
				{
				// Variables to hold the lexical cast numbers
					Real::value_type resultRealPower;
					Real::value_type valueRealFirst;
					Real::value_type valueRealSecond;

					Integer::value_type resultIntegerPower;
					Integer::value_type valueIntegerFirst;
					Integer::value_type valueIntegerSecond;

					if (isReal == false)
					{
						if (boolNumberFirst == true)
						{
							operandNumberFirst *= 2;
							operands.push_back(make<Integer>(operandNumberFirst));
							evaluated = true;
							continue;
						}
						else if (boolNumberSecond == true)
						{
							operandNumberSecond *= 2;
							operands.push_back(make<Integer>(operandNumberSecond));
							evaluated = true;
							continue;
						}
					}
					else if (isReal == true)
					{
						if (boolRealNumberFirst == true)
						{
							operandRealNumberFirst *= 2.0;
							operands.push_back(make<Real>(operandRealNumberFirst));
							evaluated = true;
							continue;
						}
						else if (boolRealNumberSecond == true)
						{
							operandRealNumberSecond *= 2.0;
							operands.push_back(make<Real>(operandRealNumberSecond));
							evaluated = true;
							continue;
						}
					}
				}
			}
		}
	}

	// Throw exception if more than one operand is left after all of the evaluations
	if (operands.size() > 1)
	{
		throw exception("Error: too many operands");
	}


	Operand::pointer_type finalResult = dynamic_pointer_cast<Operand>(operands.back());

	return  finalResult; // final evaluated result
}


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
