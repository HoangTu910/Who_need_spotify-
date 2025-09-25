#ifndef _WSN_UTILS_H_
#define _WSN_UTILS_H_

#include <iostream>

#define WNS_LOG(msg) std::cout << msg << std::endl;
#define WNS_ERROR(msg) std::cerr << "Error: " << msg << std::endl;
#define WNS_WARNING(msg) std::cerr << "Warning: " << msg << std::endl;
#define WNS_DEBUG(msg) std::cout << "Debug: " << msg << std::endl;

#endif