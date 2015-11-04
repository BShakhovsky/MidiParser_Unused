# pragma once

# include "targetver.h"

# include <cassert>
# include <fstream>
# include <iostream>
# include <map>
# include <memory>
# include <vector>

# pragma warning(push, 3)
#	include <chrono>
#	include <iomanip>	// std::setfill, std::setw
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <boost\noncopyable.hpp>
# pragma warning(pop)

# include "..\gTests\stdafx.h"
# ifdef ADD_FAILURE
#	define WARNING(MESSG) { ADD_FAILURE()				<< MESSG;				system("Pause"); }
# else
#	define WARNING(MESSG) { std::cout << "\nWARNING: "	<< MESSG << std::endl;	system("Pause"); }
# endif

# pragma warning(disable:4625 4626)		// boost::noncopyable
# pragma warning(disable:4514 4710)	// inline functions

# ifdef _DEBUG
#	define BORIS_ASSERT(BORIS_MSG) { std::cerr << "Assertion failed: " << BORIS_MSG << std::endl; assert(false); }
# elif defined NDEBUG
#	define BORIS_ASSERT(BORIS_MSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif
# define PURE_VIRTUAL BORIS_ASSERT("PURE VIRTUAL FUNCTION " __FUNCTION__ " MUST NEVER BE CALLED");