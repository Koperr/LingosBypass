#include <curl/curl.h>
#include <iostream>
#include <string>
#include <map>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CurlHandler
{
public:
    CurlHandler(const std::string& autologin, const std::string& cookie_consent, const std::string& lingos_sid, const std::string& quiz_id, const std::string& file_to_get_answer_from, const std::string& file_to_send_answer_to)
    {
        this->autologin = autologin;
        this->cookie_consent = cookie_consent;
        this->lingos_sid = lingos_sid;

        this->quiz_id = quiz_id;
        this->file_to_get_answer_from = file_to_get_answer_from;
        this->file_to_send_answer_to = file_to_send_answer_to;
    }

    json GetData();
    std::string GetQuestion();

    void SendAnswer();

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

    json ParseData(const std::string& input);

    std::string autologin;
    std::string cookie_consent;
    std::string lingos_sid;

    std::string quiz_id;
    std::string file_to_get_answer_from;
    std::string file_to_send_answer_to;
};