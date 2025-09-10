#pragma
#include <exception>
#include <string>
#include <iostream>

class FileException : public std::exception
{
public:

    FileException(const std::string& message)
    {
        this->m_message = "File Error: " + message + "\n";
    }

    virtual char const* what() const throw()
    {
        return m_message.c_str();
    }
private:
    std::string m_message;
};