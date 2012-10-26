#ifndef SYNTAX_ERROR_EXCEPTION_H
#define SYNTAX_ERROR_EXCEPTION_H

#include <exception>

class SyntaxErrorException: public std::exception
{
	public:
	virtual const char* what() const throw()
	{
		return "A Syntax Error has occured.";
	}

};

#endif //SYNTAX_ERROR_EXCEPTION_H
