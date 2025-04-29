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
    CurlHandler(const std::string &cookies)
        : cookies(cookies) {}

    void SetExercise(const std::string& id);
    void AddWordSet(const std::string& id);

    void GetQnA();
    void GetCurrentQnAID();
    void SendAnswer();

    json QnA;
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

    json ParseData(const std::string& input);

    std::string cookies;
    std::string exercise_id;
    std::string current_question;
    std::string current_question_id;
    std::string question_type;
    std::string new_teacher_id;
    std::string answer_end_point;


    std::vector<std::string> v_wordsets;
};