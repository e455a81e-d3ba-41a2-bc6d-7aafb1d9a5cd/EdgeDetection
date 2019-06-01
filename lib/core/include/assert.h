#pragma once
#include <exception>
#include <string>
#include <sstream>
#include <iostream>

namespace lib {
    class AssertionFailedException : std::exception
    {
    public:
        class StreamFormatter
        {
        private:

            std::ostringstream stream;

        public:

            operator std::string() const
            {
                return stream.str();
            }

            template<typename T>
            StreamFormatter& operator << (const T& value)
            {
                stream << value;
                return *this;
            }
        };
    private:

        const char* expression_;
        const char* file_;
        int line_;
        std::string message_;
        std::string report_;
    public:
        AssertionFailedException(const char* expression, const char* file, int line, const std::string& message) :
            expression_(expression),
            file_(file),
            line_(line),
            message_(message)
        {
            auto outputStream = std::ostringstream{};

            if (!message_.empty()) {
                outputStream << message_ << ": ";
            }

            outputStream << "Assertion '" << expression << "'";
            outputStream << " failed in file '" << file << "' line " << line;
            report_ = outputStream.str();
        }

        const char* what() const override
        {
            return report_.c_str();
        }

        const char* file() const {
            return file_;
        }

        const int line() const {
            return line_;
        }

        const char* message() const {
            return message_.c_str();
        }

        virtual ~AssertionFailedException()
        {
        }
    };


#ifdef _DEBUG

#define throw_assert(EXPRESSION, MESSAGE) \
if(!(EXPRESSION)) { throw AssertionFailedException(#EXPRESSION, __FILE__, __LINE__, (AssertionFailedException::StreamFormatter() << MESSAGE)); }

#else

#define throw_assert(EXPRESSION, MESSAGE)

#endif // _DEBUG
}
