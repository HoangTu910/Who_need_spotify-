#ifndef _WSN_UTILS_H_
#define _WSN_UTILS_H_

#include <iostream>

// Helper function template for streaming multiple arguments
namespace wns_utils {
    template<typename T>
    void log_stream(std::ostream& os, T&& t) {
        os << std::forward<T>(t);
    }

    template<typename T, typename... Args>
    void log_stream(std::ostream& os, T&& t, Args&&... args) {
        os << std::forward<T>(t);
        log_stream(os, std::forward<Args>(args)...);
    }
}

// Variadic macros for logging
#define WNS_LOG(...)      do { std::cout << "[WSN_LOG] ";     wns_utils::log_stream(std::cout, __VA_ARGS__);     std::cout << std::endl; } while(0)
#define WNS_ERROR(...)    do { std::cerr << "[WSN_ERROR] ";   wns_utils::log_stream(std::cerr, __VA_ARGS__);    std::cerr << std::endl; } while(0)
#define WNS_WARNING(...)  do { std::cerr << "[WSN_WARNING] "; wns_utils::log_stream(std::cerr, __VA_ARGS__);    std::cerr << std::endl; } while(0)
#define WNS_DEBUG(...)    do { std::cout << "[WSN_DEBUG] ";   wns_utils::log_stream(std::cout, __VA_ARGS__);    std::cout << std::endl; } while(0)

#endif