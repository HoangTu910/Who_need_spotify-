#ifndef _WSN_UTILS_H_
#define _WSN_UTILS_H_

#include <iostream>

#define WNS_LOG(msg) std::cout << "[WSN_LOG] " << msg << std::endl;
#define WNS_ERROR(msg) std::cerr << "[WSN_ERROR] " << msg << std::endl;
#define WNS_WARNING(msg) std::cerr << "[WSN_WARNING] " << msg << std::endl;
#define WNS_DEBUG(msg) std::cout << "[WSN_DEBUG] " << msg << std::endl;

#endif