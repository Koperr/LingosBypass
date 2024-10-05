#include <curl/curl.h>
#include <iostream>
#include <string>

class CurlHandler
{
public:
    std::string GetQuestion();

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

};
