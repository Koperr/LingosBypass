#include <curl/curl.h>
#include <iostream>
#include <string>
#include <map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CurlHandler
{
public:
    std::map<std::string, std::string> m = 
    {
        {"Oglądałem zachód słońca", "i watched the sunset"}, //1
        {"miejsca historyczne", "historical sites"}, //2
        {"godziny otwarcia", "opening hours"}, //3
        {"przewodnik (osoba)", "guide"}, //4
        {"Jak tam wakacje?", "how was your holiday"}, //5
        {"rezerwować bilet", "book a ticket"}, //6
        {"zachód słońca", "sunset"}, //7
        {"dom wczasowy, kurort", "holiday resort"}, //8
        {"atrakcyjny", "attractive"}, //9
        {"przewodnik (książka)", "guidebook"}, //10
        {"wycieczka rowerowa", "bike trip"}, //11
        {"Jakie są godziny otwarcia?", "what are the opening hours"}, //12
        {"obniżka", "discount"}, //13
        {"atrakcja turystyczna", "tourist attraction"} //14
    };

    std::string GetQuestion();
    void SendAnswer();

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

};