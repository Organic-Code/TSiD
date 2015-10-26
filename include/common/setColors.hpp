#ifndef SET_COLORS_HPP_INCLUDED
#define SET_COLORS_HPP_INCLUDED

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#	define OS_WINDOWS
#	include <windows.h>
#else
#	include <unistd.h>
#	include <iostream>
#endif

#include <string>

void setColors( std::string const& text_color );

#endif
