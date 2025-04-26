#include "CurlHandler.h"

// Funkcja callback do zapisywania odpowiedzi serwera

int main() 
{
    CurlHandler curl("Cookie:	lingos_sid=apq11l35ncc7kuenaksn8tbtic; autologin=gWq2ipDOm2RbZopvz8fu5ZFaWPwk%2FH22sTKgGOorOxE%3D%3AieVM60bQBFMyroZsgXySFpk5MK%2Fs2F%2FDn9F5O5HyJW8%3D; CookieConsent={stamp:%27f+7ywpRuvGmFIujhI+oEbPugblwcsBmNcpD/7RUUNZKavIMY97rzhQ==%27%2Cnecessary:true%2Cpreferences:false%2Cstatistics:false%2Cmarketing:false%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1745620490943%2Cregion:%27pl%27}");

    curl.SetExercise("25503");

    curl.AddWordSet("89624"); // employment Impulse 3
    curl.GetQnA();

    //std::cout << curl.QnA.dump();
    for(int i = 0; i < 100; i++)
    {
        curl.GetCurrentQnAID();
        curl.SendAnswer();
    }
}