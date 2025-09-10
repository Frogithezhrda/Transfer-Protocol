#pragma
#include <exception>
#include <string>
#include <iostream>

class InputException : public std::exception
{
public:

    InputException(const std::string& message)
    {
        this->m_message = "Input Error: " + message + "\n";
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};