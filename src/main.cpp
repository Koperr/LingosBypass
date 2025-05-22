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
    curl.GetQnA();

    CurlHandler Marcel("cookie: lingos_sid=p4odd2bp4cni1m7vj1plqs2su5; CookieConsent={stamp:%27z1xJ1mu/uGZwXA/rBIkuCKtuUW+cJ5CvRoAjDwNSpTYMHgEbMVJztQ==%27%2Cnecessary:true%2Cpreferences:true%2Cstatistics:true%2Cmarketing:true%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1745872540821%2Cregion:%27pl%27}; _ga=GA1.1.415176137.1745872539; autologin=iYyztUENwrpdlL%2BY1n5RcZOo9l%2FUq5CZ27tQG81QqOc%3D%3AwBbYbf6K75P2GP9fuadZ6oJFPPubVAF7O4nHl57GNmc%3D; _ga_67KDMLGTNZ=GS1.1.1745872538.1.1.1745873267.0.0.0; __eoi=ID=d9d3ccc5c9d3b82b:T=1745872557:RT=1745873429:S=AA-AfjY2ZI1LdPjJJ8zDvGWxzh85");
    Marcel.SetExercise("25503");
    Marcel.AddWordSet("89624");
    Marcel.GetQnA();

    //CurlHandler Edek("");
    //Edek.SetExercise("25503");
    //Edek.AddWordSet("89624");
    //Edek.GetQnA();

    //CurlHandler Seweryn("");
    //Seweryn.SetExercise("25503");
    //Seweryn.AddWordSet("89624");
    //Seweryn.GetQnA();

    // BŁAŻEJ
    //CurlHandler Frajer("cookie: lingos_sid=mr8c6u3q20ut13v4522k1ptbgr; CookieConsent={stamp:%27k5DABeWdSLEzrtDg3A+yijIhUT4SUtMVFcRzQ3CytrSk4xLXd0Ib9g==%27%2Cnecessary:true%2Cpreferences:true%2Cstatistics:true%2Cmarketing:true%2Cmethod:%27explicit%27%2Cver:1%2Cutc:1745960036617%2Cregion:%27pl%27}; _ga=GA1.1.1051018077.1745960124; autologin=R0hfxBxd0xn2v0wctW7OKyS0oeKx%2BArcX7MxnY1bFmU%3D%3ApM9xZ1V9J%2FFzvFH3O2BW1KposWGa7Bgu1ajXxReijkQ%3D; _ga_67KDMLGTNZ=GS1.1.1745960123.1.1.1745960252.0.0.0");
    //Frajer.SetExercise("25503");
    //Frajer.AddWordSet("89624");
    //Frajer.GetQnA();

    int request_count = 100; // lessons * questions (in my case 5 lessons per day * 20 questions each) PLUS you may add some
    // if you didnt "unlock" current wordset questions (if you are doing them first time)
    ////////////////////////////////////////////


    float progress = 0.0f;
    int barWidth = 30;
    for(int i = 0; i < request_count; i++)
    {
        //curl.GetCurrentQnAID();
        //curl.SendAnswer();
        
        Marcel.GetCurrentQnAID();
        Marcel.SendAnswer();

        //Edek.GetCurrentQnAID();
        //dek.SendAnswer();

        //Seweryn.GetCurrentQnAID();
        //Seweryn.SendAnswer();

        //Frajer.GetCurrentQnAID();
        //Frajer.SendAnswer();

        //usleep(0.1 * 1000000); // for testing progress bar only

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