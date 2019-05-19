#pragma once
#include <ostream>
#include <sstream>
#include <mutex>

namespace lib {
    enum class log_level : int { Debug, Info, Warning, Severe };
    const static char* log_level_string[] = { "Debug", "Info", "Warning", "Severe" };

    class logger;

    class logstream : public std::ostringstream
    {
        logger& logger_;
        log_level level_;
    public:
        logstream(logger& logger, log_level level);
        logstream(const logstream& logstream);
        ~logstream();
    };

    class logger {
        std::mutex mutex_;
        std::ostream& ostream_;
    public:
        logger(std::ostream&);
        virtual ~logger();
        void log(log_level level, const std::string&& message);

        logstream operator()();
        logstream operator()(log_level nLevel);
    };
};
