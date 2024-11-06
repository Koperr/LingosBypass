#include <curl/curl.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class CurlHandler
{
public:

    void SetUserAgent(const std::string& user_agent);
    void SetAutoLogin(const std::string& autologin);
    void SetCookieConsent(const std::string& cookie_consent);
    void SetLingosSid(const std::string& lingos_sid);
    void SetTargetQuizID(const std::string& quiz_id);
    void SetQuiztoGetDataFrom(const std::string& file_name);
    void SetFileToSendAnswerTo(const std::string& file_name);

    json GetData();
    std::string GetQuestion();
    void SendAnswer();

private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* s);

    json ParseData(const std::string& input);

    std::string user_agent;
    std::string autologin;
    std::string cookie_consent;
    std::string lingos_sid;

    std::string quiz_id;
    std::string file_to_get_answer_from;
    std::string file_to_send_answer_to;

    std::vector<std::string> quizes;
};