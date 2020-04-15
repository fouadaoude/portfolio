#include "diary2.0.h"


int main(){
    int menuOption, loginValue=0, userID;
    char username[50], password[50];

    do{
        // Gets UserInformation from database
        userInfo(username, password);
        menu(&menuOption);
        
        switch(menuOption){
            case 1:
                clr();
                login(&loginValue, username, password, &userID);                
                if (loginValue == 1) {
                    userInterface(&loginValue, username, password, &userID);
                }
                else {
                    clr();
                    printf("\nIncorrect Username or Password Try Again\n");
                }
                break;
            case 2:
                createUser(&loginValue, username, password);
                break;
        }
    }while(menuOption != 0);

    return 0;
}