
#ifndef CAFFEINE_UTILITIES_LOGGING_INCLUDED
#define CAFFEINE_UTILITIES_LOGGING_INCLUDED

#include <iostream>


// Only for use during loading. this is not subject to
// settings in the application.xml
#define LOADING_LOG(msg)								\
		std::cout << "Loading: " << msg << std::endl;	\


// Stuff to help logs look less balls
#define LOG_LINE_SEPERATOR								\
		std::cout << "-------" << std::endl;			\

		
#define LOG_BREAK_SEPERATOR								\
		std::cout << std::endl;							\


#define DEBUG_LOG(msg)									\
		std::cout << msg << std::endl;					\



#endif // include guard