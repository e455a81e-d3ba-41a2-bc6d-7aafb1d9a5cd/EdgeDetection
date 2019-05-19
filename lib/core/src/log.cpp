#include "../include/log.h"

namespace lib {
    logger::logger(std::ostream& ostream) : ostream_(ostream)
    {

    }

    logger::~logger()
    {
        //ostream_.flush();
    }

    void logger::log(log_level level, const std::string&& message)
    {
        //std::lock_guard<std::mutex>(mutex_);
        ostream_ << log_level_string[static_cast<int>(level)] << ": " << message << std::endl;
    }

    logstream logger::operator()() 
    {
        return logstream(*this, log_level::Info);
    }

    logstream logger::operator()(log_level level) 
    {
        return logstream(*this, level);
    }

    logstream::logstream(logger& logger, log_level level) : logger_(logger), level_(level)
    {
    }

    logstream::logstream(const logstream& logstream) : logger_(logstream.logger_), level_(logstream.level_)
    {
    }

    logstream::~logstream()
    {
        logger_.log(level_, str());
    }
}