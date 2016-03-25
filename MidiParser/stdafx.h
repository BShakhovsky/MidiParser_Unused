# pragma once

# pragma warning(disable:4710 4711)

# include "targetver.h"

# include <cassert>
# include <fstream>
# include <iostream>
# include <map>
# include <memory>
# include <vector>

# pragma warning(push, 2)
#	include <chrono>
#	include <iomanip>	// std::setfill, std::setw
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <boost\noncopyable.hpp>
#	include <boost\format.hpp>
# pragma warning(pop)

# include "gTests\stdafx.h"
# ifdef ADD_FAILURE
#	define WARNING(MESSG) { ADD_FAILURE()				<< MESSG;				system("Pause"); }
# elif defined _DEBUG
#	define WARNING(MESSG) { std::cerr << "\nWARNING: "	<< MESSG << std::endl;	system("Pause"); }
# else
#	define WARNING(MESSG) { std::cerr << "\nWARNING: "	<< MESSG << std::endl; }
# endif

# pragma warning(disable:4514)
# pragma warning(disable:4625 4626)	// boost::noncopyable

# ifdef _DEBUG
#	define BORIS_ASSERT(BORIS_MSG) { std::cerr << "Assertion failed: " << BORIS_MSG << std::endl; \
										assert(false); }
# elif defined NDEBUG
#	define BORIS_ASSERT(BORIS_MSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif