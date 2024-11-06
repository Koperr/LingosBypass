#include "CurlHandler.h"

size_t CurlHandler::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json CurlHandler::ParseData(const std::string& input)
{
    json j;

    size_t first_arg_start_pos = 0;
    size_t first_arg_end_pos;
    std::string first_arg_start = "<div class=\"d-sm-flex pe-3 w-100 w-sm-50 flashcard-border-end\">";
    std::string first_arg_end = "</div>";

    size_t second_arg_start_pos = 0;
    size_t second_arg_end_pos;
    std::string second_arg_start = "<div class=\"d-sm-flex mt-2 mt-sm-0 ps-0 ps-sm-3 pe-3 pe-sm-0 w-100 w-sm-50 flashcard-border-start\">";
    std::string second_arg_end = "</div>";

    while((first_arg_start_pos = input.find(first_arg_start, first_arg_start_pos)) != std::string::npos)
    {
        // Finding the question
        first_arg_start_pos += first_arg_start.length();

        first_arg_end_pos = input.find(first_arg_end, first_arg_start_pos);
        if(first_arg_end_pos == std::string::npos) {  break;  }

        std::string question = input.substr(first_arg_start_pos, first_arg_end_pos - first_arg_start_pos);
        //std::cout << question << std::endl;
        
        // Finding answer to the question
        second_arg_start_pos = input.find(second_arg_start, second_arg_start_pos);
        second_arg_start_pos += second_arg_start.length();
        
        second_arg_end_pos = input.find(second_arg_end, second_arg_start_pos);
        if(second_arg_end_pos == std::string::npos) {  break;  }

        std::string answer = input.substr(second_arg_start_pos, second_arg_end_pos - second_arg_start_pos);
        //std::cout << answer << std::endl;
        
        j[answer] = question;
    }

    return j;
}

void CurlHandler::SetUserAgent(const std::string &user_agent){this->user_agent = "User-Agent: " + (std::string)"Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:131.0) Gecko/20100101 Firefox/131.0";}
void CurlHandler::SetAutoLogin(const std::string &autologin) { this->autologin = autologin; }
void CurlHandler::SetCookieConsent(const std::string &cookie_consent){this->cookie_consent = cookie_consent;}
void CurlHandler::SetLingosSid(const std::string &lingos_sid){this->lingos_sid = lingos_sid;}
void CurlHandler::SetTargetQuizID(const std::string &quiz_id){this->quiz_id = quiz_id;}
void CurlHandler::SetQuiztoGetDataFrom(const std::string &file_name){quizes.push_back(file_name);}
void CurlHandler::SetFileToSendAnswerTo(const std::string &file_name){this->file_to_send_answer_to = file_name;}

json CurlHandler::GetData()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    json j;
    
    if (curl) {
        //for(auto id : quizes)
        //{
            // Ustawienie URL
            std::string url = "https://lingos.pl/student-confirmed/wordset/";// + id;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Ustawienie metody na POST
            //curl_easy_setopt(curl, CURLOPT_OPT, 1L);

            // Ustawienie nagłówków
            struct curl_slist* headers = NULL;
            headers = curl_slist_append(headers, "Cookie: lingos_sid=apq11l35ncc7kuenaksn8tbtic; CookieConsent={stamp:%27TsO3AumTL4vbA6x7SFNWH8xCGl/iufXUKlYDFRvUNa23DnIPEwDiAQ==%27%2Cnecessary:true%2Cpreferences:false%2Cstatistics:false%2Cmarketing:false%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1727820297301%2Cregion:%27pl%27}; autologin=gWq2ipDOm2RbZopvz8fu5ZFaWPwk%2FH22sTKgGOorOxE%3D%3AieVM60bQBFMyroZsgXySFpk5MK%2Fs2F%2FDn9F5O5HyJW8%3D");
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
            headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
            
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // Ustawienie timeout
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 sekund

            // Ustawienie pustego ciała w zapytaniu POST
            //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

            // Callback do zapisywania odpowiedzi serwera
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Wysłanie żądania
            res = curl_easy_perform(curl);

            // Sprawdzenie odpowiedzi
            if (res != CURLE_OK) {
                std::cerr << "(GetData()) cURL error: " << curl_easy_strerror(res) << std::endl;
            } else {
                //std::cout << "Odpowiedź serwera: " << readBuffer << std::endl;
            }

            // Zwolnienie pamięci dla nagłówków
            curl_slist_free_all(headers);

            // Zakończenie cURL
            curl_easy_cleanup(curl);
            
            j = ParseData(readBuffer);
            //j.merge_patch(ParseData(readBuffer));  
            //j += ParseData(readBuffer);
     
            curl_global_cleanup();
            //}
    }
        


    return j;
    //return ParseData(readBuffer);
}

std::string CurlHandler::GetQuestion()
{
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        // Ustawienie URL
        std::string url = "https://lingos.pl/s/exercise/0," + this->quiz_id + "," + this->file_to_get_answer_from;
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        //std::cout << "Sending GET request to URL: " << url << std::endl;

        std::string cookies = "Cookie: lingos_sid=" + lingos_sid + "; CookieConsent=" + cookie_consent + "; autologin=" + autologin;

        // Ustawienie metody na POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        // Ustawienie nagłówków
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, cookies.c_str());
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0");
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");

        //std::cout << "Headers: " << std::endl;
        //std::cout << "  Cookie: " << cookies << std::endl;
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Ustawienie timeout
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 sekund

        // Ustawienie pustego ciała w zapytaniu POST
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "");

        // Callback do zapisywania odpowiedzi serwera
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Wysłanie żądania
        res = curl_easy_perform(curl);

        // Sprawdzenie odpowiedzi
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        } else {
            //std::cout << "Odpowiedź serwera: " << readBuffer << std::endl;
        }

        // Zwolnienie pamięci dla nagłówków
        curl_slist_free_all(headers);

        // Zakończenie cURL
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    json recieved_data = json::parse(readBuffer);
    std::string question = recieved_data["exercise"]["translation"];
    std::cout << "GetQuestion() returned " << question << std::endl;
    return question;
}

void CurlHandler::SendAnswer()
{
    CURL *curl;
    CURLcode res;

    // Ustawienia URL i dane odpowiedzi
    const std::string url = "https://lingos.pl/s/answer/" + this->file_to_send_answer_to;
    std::string cookies = "Cookie: lingos_sid=" + lingos_sid + "; CookieConsent=" + cookie_consent + "; autologin=" + autologin;

    json j = GetData();

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Ustawienia cURL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Ustawienie nagłówków
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, user_agent.c_str());
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, cookies.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Ustawienie danych do wysłania
        std::string jsonData = j.dump(); // Konwertowanie obiektu JSON na string
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

        // Wykonanie zapytania
        res = curl_easy_perform(curl);

        // Sprawdzanie błędów
        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Czyszczenie
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

}
