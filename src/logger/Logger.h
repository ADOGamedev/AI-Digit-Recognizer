#pragma once

#include <iostream>

namespace Logger {

class Logger {
public:
    virtual ~Logger() = default;    
    virtual void log(const std::string& message) {
        std::cout << message;
    }
    virtual void log(const char message) {
        std::cout << message;
    }

    virtual void logerr(const std::string& message) {
        std::cerr << message;
    }

};

void set_logger(Logger* logger);
void log(const std::string& message);
void log(const char ch);
void logerr(const std::string& message);

}

