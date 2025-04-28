#include "CurlHandler.h"
#include <unistd.h>


int main() 
{
    ////////////////// README //////////////////

    // You have to modify these 3 lines in order to make it work
    // for you, this is just an example of how it should look like
    // You can find your login cookies by inspecting site > Network tab > reload site > click on whichever request on list > Headers > Request Headers
    CurlHandler curl("Cookie:	lingos_sid=apq11l35ncc7kuenaksn8tbtic; autologin=gWq2ipDOm2RbZopvz8fu5ZFaWPwk%2FH22sTKgGOorOxE%3D%3AieVM60bQBFMyroZsgXySFpk5MK%2Fs2F%2FDn9F5O5HyJW8%3D; CookieConsent={stamp:%27f+7ywpRuvGmFIujhI+oEbPugblwcsBmNcpD/7RUUNZKavIMY97rzhQ==%27%2Cnecessary:true%2Cpreferences:false%2Cstatistics:false%2Cmarketing:false%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1745620490943%2Cregion:%27pl%27}");
    curl.SetExercise("25503"); // Set an exercize to do (https://lingos.pl/s/lesson/0,0,[EXERCISE ID])
    curl.AddWordSet("89624"); // Add a wordset to get sentences from (you can add more than 1 set of sentences), (https://lingos.pl/student-confirmed/wordset/[WORDSET ID])

    int request_count = 100; // lessons * questions (in my case 5 lessons per day * 20 questions each)
    ////////////////////////////////////////////

    curl.GetQnA();

    float progress = 0.0f;
    int barWidth = 30;
    for(int i = 0; i < request_count; i++)
    {
        curl.GetCurrentQnAID();
        curl.SendAnswer();
        

        usleep(0.1 * 1000000); // for testing progress bar only

        std::cout << "[";
        int pos = barWidth * progress;
        for (int i = 0; i < barWidth; ++i) {
            if (i <= pos) std::cout << "#";
            else std::cout << " ";
        }
        std::cout << std::fixed << std::setprecision(1) << "] " << float(progress * 100.0) << " %\r";
        std::cout.flush();
        
        float test = (float)request_count;
        progress += 1.f / test;
    }
    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
            if (i <= barWidth) std::cout << "#";
            else std::cout << " ";
    }
    std::cout << "] 100.0 %\nTask finished successfully.\n© Jakub Świtała, 2025. All rights reserved.\n";

}