#include "CurlHandler.h"

// Funkcja callback do zapisywania odpowiedzi serwera

int main() 
{
    CurlHandler curl;//("gWq2ipDOm2RbZopvz8fu5ZFaWPwk/H22sTKgGOorOxE=:ieVM60bQBFMyroZsgXySFpk5MK/s2F/Dn9F5O5HyJW8=", 
    //"{stamp:'TsO3AumTL4vbA6x7SFNWH8xCGl/iufXUKlYDFRvUNa23DnIPEwDiAQ==',necessary:true,preferences:false,statistics:false,marketing:false,method:'explicit',ver:1,utc:1727820297301,region:'pl'}",
    //"apq11l35ncc7kuenaksn8tbtic",
    //"75101", // quiz_id
    //"25503", // file_to_get_answer_from
    //"203568333"); // file_to_send_answer_to

    curl.SetUserAgent("Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:131.0) Gecko/20100101 Firefox/131.0");
    curl.SetAutoLogin("gWq2ipDOm2RbZopvz8fu5ZFaWPwk/H22sTKgGOorOxE=:ieVM60bQBFMyroZsgXySFpk5MK/s2F/Dn9F5O5HyJW8=");
    curl.SetCookieConsent("{stamp:'NOyfdedc0A8xsg/+F0xfZ1zkKJZKWsUO3fRLj4JPdHKguaQlyFI19w==',necessary:true,preferences:true,statistics:true,marketing:true,method:'explicit',ver:1,utc:1730923362440,region:'pl'}");
    curl.SetLingosSid("apq11l35ncc7kuenaksn8tbtic");
    curl.SetTargetQuizID("25503");
    curl.SetQuiztoGetDataFrom("72148"); // wrzesien
    curl.SetQuiztoGetDataFrom("75101"); // paxdziernik

    json j;
    j = curl.GetData();
    //std::cout << "dupa1\n";
    std::cout << "dupa1\n";
    std::string question = curl.GetQuestion();
    std::cout << "question: " << question << std::endl;
    //std::cout << "dupa1\n";
    //std::string answer = j[curl.GetQuestion()];

    //std::cout << "dupa2\n";
    //std::cout << std::endl << "[" << question << "] ["<< answer << "]" << std::endl;
    //std::cout << j.dump();

    curl.SendAnswer();
}