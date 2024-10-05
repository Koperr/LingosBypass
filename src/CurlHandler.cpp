#include "CurlHandler.h"

size_t CurlHandler::WriteCallback(void *contents, size_t size, size_t nmemb, std::string *s)
{
    s->append((char*)contents, size * nmemb);
    return size * nmemb;
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
        curl_easy_setopt(curl, CURLOPT_URL, "https://lingos.pl/s/exercise/0,0,25503");

        // Ustawienie metody na POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

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

    return question;
}

void CurlHandler::SendAnswer()
{
    CURL *curl;
    CURLcode res;

    // Ustawienia URL i dane odpowiedzi
    const std::string url = "https://lingos.pl/s/answer/200265519";
    json j;
    j["answer"] = m[GetQuestion()];

    // Inicjalizacja cURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if(curl) {
        // Ustawienia cURL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Ustawienie nagłówków
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Accept: application/json");
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Cookie: lingos_sid=apq11l35ncc7kuenaksn8tbtic; CookieConsent={stamp:%27TsO3AumTL4vbA6x7SFNWH8xCGl/iufXUKlYDFRvUNa23DnIPEwDiAQ==%27%2Cnecessary:true%2Cpreferences:false%2Cstatistics:false%2Cmarketing:false%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1727820297301%2Cregion:%27pl%27}; autologin=gWq2ipDOm2RbZopvz8fu5ZFaWPwk%2FH22sTKgGOorOxE%3D%3AieVM60bQBFMyroZsgXySFpk5MK%2Fs2F%2FDn9F5O5HyJW8%3D");
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
