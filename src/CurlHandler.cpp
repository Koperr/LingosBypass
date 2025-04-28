#include "CurlHandler.h"

void CurlHandler::SetExercise(const std::string &id){this->exercise_id = id;}
void CurlHandler::AddWordSet(const std::string &id){v_wordsets.push_back(id);}

size_t CurlHandler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    if (response == nullptr) {
        return 0;
    }
    try {
        response->append(static_cast<char*>(contents), totalSize);
    } catch (...) {
        return 0;
    }
    return totalSize;
}


json CurlHandler::ParseData(const std::string& input)
{
    json json_QnA;

    size_t question_start_pos = 0;
    size_t question_end_pos;
    std::string question_start = "<div class=\"d-sm-flex pe-3 w-100 w-sm-50 flashcard-border-end\">";
    std::string first_arg_end = "</div>";

    size_t trans_start_pos = 0;
    size_t trans_end_pos;
    std::string trans_start = "<div class=\"d-sm-flex mt-2 mt-sm-0 ps-0 ps-sm-3 pe-3 pe-sm-0 w-100 w-sm-50 flashcard-border-start\">";
    std::string trans_end = "</div>";

    while((question_start_pos = input.find(question_start, question_start_pos)) != std::string::npos)
    {
        // Finding the question
        question_start_pos += question_start.length();
        question_end_pos = input.find(first_arg_end, question_start_pos);
        if(question_end_pos == std::string::npos) {  break;  }

        std::string question = input.substr(question_start_pos, question_end_pos - question_start_pos);
        
        // Finding answer to the question
        trans_start_pos = input.find(trans_start, trans_start_pos);
        trans_start_pos += trans_start.length();
        
        trans_end_pos = input.find(trans_end, trans_start_pos);
        if(trans_end_pos == std::string::npos) {  break;  }

        std::string answer = input.substr(trans_start_pos, trans_end_pos - trans_start_pos);
        
        json_QnA[answer] = question;
    }

    return json_QnA;
}


void CurlHandler::GetQnA()
{
    for(auto wordset_id : v_wordsets)
    {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) 
        {

            curl_easy_setopt(curl, CURLOPT_URL, ("https://lingos.pl/student-confirmed/wordset/" + wordset_id).c_str());

            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, cookies.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                std::cerr << "(GetData()) cURL error: " << curl_easy_strerror(res) << std::endl;
            } else {
                //std::cout << "Server response: " << readBuffer << std::endl;
            }

            ParseData(readBuffer);

            curl_slist_free_all(headers);

            curl_easy_cleanup(curl);
            QnA.merge_patch(ParseData(readBuffer));
     
            curl_global_cleanup();
            }
    }
}

void CurlHandler::GetCurrentQnAID()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    json json_response;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, this->cookies.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, ("https://lingos.pl/s/exercise/0,0," + exercise_id).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 0L); 

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            //std::cout << "Server response: " << readBuffer << std::endl;
        }
        json_response = json::parse(readBuffer);
        current_question_id = json_response.value("exercise", nlohmann::json::object())
                                         .value("identifier", nlohmann::json(""))
                                         .dump();

        current_question = json_response.value("exercise", nlohmann::json::object())
                                      .value("translation", nlohmann::json(""));

        curl_slist_free_all(headers);

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}

void CurlHandler::SendAnswer()
{
    CURL *curl = curl_easy_init();
    CURLcode res;

    std::string request_raw_json = "{\"answer\":" + QnA[current_question].dump() + "}";

    if(curl) {
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Cookie: " + this->cookies).c_str());

        curl_easy_setopt(curl, CURLOPT_URL, ("https://lingos.pl/s/answer/" + current_question_id).c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request_raw_json.c_str());
        
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            //std::cout << "Response: " << response << std::endl;
        } else {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }


        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    
}
