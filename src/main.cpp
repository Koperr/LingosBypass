#include "CurlHandler.h"

// Funkcja callback do zapisywania odpowiedzi serwera

int main() 
{
    CurlHandler curl;
    std::string dupa = curl.GetQuestion();
    std::cout << dupa << std::endl;
}
