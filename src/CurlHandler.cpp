#include "CurlHandler.h"

size_t CurlHandler::WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    std::string* response = static_cast<std::string*>(userp);
    if (response == nullptr) {
        return 0; // Brak miejsca na zapis
    }
    try {
        response->append(static_cast<char*>(contents), totalSize);
    } catch (...) {
        return 0; // Zwróć 0 w przypadku błędu
    }
    return totalSize; // Ilość zapisanych bajtów
}
json CurlHandler::ParseData(const std::string& input)
{
    std::cout << "RUNNING PARSEDATA\n";
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
void CurlHandler::SetWordSet(const std::string &file_name){quizes.push_back(file_name);}
void CurlHandler::SetAnswerDestination(const std::string &file_name){this->answer_destination = file_name;}

json CurlHandler::GetData()
{
    std::cout << "RUNNING GETDATA\n";

    json j;
    for(auto id : quizes)
    {
        std::cout << "GET DATA FROM ID: " << id << std::endl;
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        // Inicjalizacja cURL
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) 
        {
            // Ustawienie URL
            std::string url = "https://lingos.pl/student-confirmed/wordset/" + id;
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
            //j = ParseData(readBuffer);
            data.merge_patch(ParseData(readBuffer));
            //j.merge_patch(ParseData(readBuffer));  
            //j += ParseData(readBuffer);
     
            curl_global_cleanup();
            }
    }
        

    // return JSON with questions and translations
    return data;
    //return ParseData(readBuffer);
}

std::string CurlHandler::GetQuestion()
{
    std::cout << "RUNNING GETQUESTION\n";
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if (curl) {
        // Ustawienie URL
        //std::string url = "https://lingos.pl/s/exercise/0," + this->quiz_id + "," + this->file_to_get_answer_from;        // TODO
        std::string url = "https://lingos.pl/s/exercise/0,0,25503";
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
    //std::cout << "GetQuestion() returned " << question << std::endl;
    return question;
}

void CurlHandler::SendAnswer()
{
    std::cout << GetQuestion() << std::endl;
    std::cout << "RUNNING SENDANSWER\n";
    CURL *curl;
    CURLcode res;
    // Ustawienia URL i dane odpowiedzi
    const std::string url = "https://lingos.pl/s/answer/" + this->answer_destination;
    std::string cookies = "Cookie: lingos_sid=" + lingos_sid + "; CookieConsent=" + cookie_consent + "; autologin=" + autologin;

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Ustawienia cURL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        json jsonToSend = data[GetQuestion()];
        std::string stringToSend = jsonToSend.dump();
        std::cout << stringToSend << stringToSend.length() << std::endl;
        json jjsonToSend = {    {"answer", stringToSend}    };
        std::string test2 = jjsonToSend.dump();
        test2 = "{\"answer\":" + stringToSend + "}";
        //std::string temp = stringToSend;
        //const char* test = temp.c_str(); + 11
        //std::cout << sizeof(test);
        // Ustawienie nagłówków
        struct curl_slist *headers = NULL;
        std::string content_lenght = "Content-Length: " + std::to_string(stringToSend.length() + 11);
        headers = curl_slist_append(headers, "Accept: application/json, text/javascript, */*; q=0.01");
        headers = curl_slist_append(headers, "Accept-Encoding: gzip, deflate, br, zstd");
        headers = curl_slist_append(headers, "Accept-Language: en-US,en;q=0.5");
        headers = curl_slist_append(headers, "Connection: keep-alive");
        //headers = curl_slist_append(headers, content_lenght.c_str()); //////////// mozliwe ze jest niepotrzebny
        headers = curl_slist_append(headers, "Content-Type: application/json");
        //headers = curl_slist_append(headers, "Cookie: lingos_sid=apq11l35ncc7kuenaksn8tbtic; autologin=gWq2ipDOm2RbZopvz8fu5ZFaWPwk%2FH22sTKgGOorOxE%3D%3AieVM60bQBFMyroZsgXySFpk5MK%2Fs2F%2FDn9F5O5HyJW8%3D; CookieConsent={stamp:%27NOyfdedc0A8xsg/+F0xfZ1zkKJZKWsUO3fRLj4JPdHKguaQlyFI19w==%27%2Cnecessary:true%2Cpreferences:true%2Cstatistics:true%2Cmarketing:true%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1730923362440%2Cregion:%27pl%27}");
        headers = curl_slist_append(headers, "Host: lingos.pl");
        headers = curl_slist_append(headers, "Origin: https://lingos.pl");
        headers = curl_slist_append(headers, "Priority: u=0");
        headers = curl_slist_append(headers, "Referer: https://lingos.pl/s/lesson/0,0,25503"); //////////////////
        headers = curl_slist_append(headers, "Sec-Fetch-Dest: empty");
        headers = curl_slist_append(headers, "Sec-Fetch-Mode: cors");
        headers = curl_slist_append(headers, "Sec-Fetch-Site: same-origin");
        headers = curl_slist_append(headers, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:132.0) Gecko/20100101 Firefox/132.0");
        headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
        headers = curl_slist_append(headers, cookies.c_str());
        //headers = curl_slist_append(headers, user_agent.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Ustawienie danych do wysłania
        //std::string jsonData = data.dump(); // Konwertowanie obiektu JSON na string
        //curl_easy_setopt(curl, CURLOPT_POSTFIELDS, stringToSend.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, test2.c_str());
        // Wykonanie zapytania
        
        std::string response;
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            std::cout << "Response: " << response << std::endl;
        } else {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }
        //else{
        //    std::cout << "SEND [" << stringToSend << "] TO SERVER\n";
        //}
        // Czyszczenie
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    
}
