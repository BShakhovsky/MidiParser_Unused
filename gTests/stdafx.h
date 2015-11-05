# pragma once

# include "targetver.h"

# pragma warning(disable:4710 4711)

# include <cassert>
# include <fstream>
# include <iostream>
# include <memory>

# pragma warning(push, 1)
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <boost\lexical_cast.hpp>
#	include <gtest\gtest.h>
#	include <gtest\gtest-spi.h>	// for checking execution of ADD_FAILURE() macro in client's code
# pragma warning(pop)

# pragma warning(disable:4514)
# pragma warning(disable:4571)	// catch semantics changed since Visual C++ 7.1
# pragma warning(disable:4625 4626 5026 5027)	// non-copyable classes

# ifdef _DEBUG
#	define BORIS_ASSERT(BORIS_MSG) { std::cerr << "Assertion failed: " << BORIS_MSG << std::endl; assert(false); }
# elif defined NDEBUG
#	define BORIS_ASSERT(BORIS_MSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif