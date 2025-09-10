#pragma
#include <exception>
#include <string>
#include <iostream>

class TransferException : public std::exception
{
public:

    TransferException(const std::string& message)
    {
        this->m_message = "Transfer Error: " + message + "\n";
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};