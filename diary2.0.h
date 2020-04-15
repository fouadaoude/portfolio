#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <mysql.h>
#include <ctype.h>
#include <time.h>
#define INSERT_QUERY "INSERT INTO users (userID, Username, Password) VALUES (?, ?, ?)";
#define MAX_QUERY 1024
#define ESC 27
#define ENTER 13

void menu(int *);
void createUser(int *, char *, char *);
void userInfo(char *, char *);
void login(int *, char *, char *, int*);
void userInterface(int*, char*, char*, int*);
void clr();
void finish_with_error(MYSQL* conn);
void changeBackground();
void accountSettings(int*, char*, char*, int*);
int validPassword(char*, char*);
void addEntry(char* );

void finish_with_error(MYSQL* conn) {
    fprintf(stderr, "%s\n", mysql_error(conn));
    mysql_close(conn);
    exit(1);
}

void menu(int *menuOptionPtr){
    int menuOption;
    printf("\nPassword Protected Diary\n");
    printf("\n[1] Login\n");
    printf("\n[2] Register\n");
    printf("\n[0] Exit\n");
    scanf("%d", &menuOption);

    *menuOptionPtr = menuOption;
}
void userInfo(char *usernamePtr, char *passwordPtr){
    MYSQL *conn = mysql_init(NULL);
    
    char *server = "localhost";
    char *user = "root";
    char *password = "";
    char *database = "virtual_diary";

    if (conn == NULL) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    // Connects to database
    if (mysql_real_connect(conn, "localhost", "root", password, database, 0, NULL, 0) == NULL) {            
        finish_with_error(conn);
    }    
    mysql_close(conn);
}
void createUser(int *loginPtr, char *usernamePtr, char *passwordPtr){
    MYSQL* conn = mysql_init(NULL);
    char* server = "localhost";
    char* user = "root";
    char* serverP = "";
    char* database = "virtual_diary";
    char* result;
    char* res;
    char username[50], password[50];    
    char my_query[MAX_QUERY];

        clr();
        do{
            printf("\nEnter a Username\n");
            scanf("%s", username);
            if(strlen(username) < 5){
                printf("\nUsername is Too Short Try Again\n");
            }
            if(strlen(username) > 15){
                printf("\nUsername is Too Long Try Again\n");
            }
        }while(strlen(username) < 5 && strlen(username) > 15);
        do{
            printf("\nEnter a Password\n");
            scanf("%s", password);
            if(strlen(password) < 8){
                printf("\nPassword is Too Short Try Again\n");
            }
        }while(strlen(password) < 8);        
        if(strlen(username)>=5 && strlen(username)<=15 && strlen(password)>=8){                        
            printf("\nAccount Created Successfully\n");            
            mysql_real_connect(conn, server, user, serverP, database, 0, NULL, 0);
            snprintf(my_query, MAX_QUERY,
                "INSERT INTO users(userID, Username, Password) VALUES ('%d', '%s', '%s')", 0, username, password);            
            res = mysql_real_query(conn, my_query, strlen(my_query));                      
        }
        int id = mysql_insert_id(conn);     
        printf("\nThe last inserted row id is: %d\n", id);       
        mysql_close(conn);  //Disconnect from server            
}
void login(int *loginPtr, char *usernamePtr, char *passwordPtr, int *userIDPtr){
    char userA[50], passA[50];
    char username[50], password[50];
    char* userInfo;
    int size = sizeof(userA)/sizeof(userA[0]);
    int login = 0, userID, correct;
    // MySQL Stuff Below
    MYSQL* conn = mysql_init(NULL);
    MYSQL_RES* result;
    char* server = "localhost";
    char* user = "root";
    char* serverP = "";
    char* database = "virtual_diary";  
    char* res;
    // To Here
        if (conn == NULL) { // If connection is False
            fprintf(stderr, "mysql_init() failed\n");
            exit(1);
        }
        if (mysql_real_connect(conn, server, user, serverP, database, 0, NULL, 0) == NULL) { // Connect to virtual_diary Database
            finish_with_error(conn);
        }
    
        printf("\nEnter Username\n");
        scanf("%s", &userA);
        printf("\nEnter Password\n");
        scanf("%s", &passA);

        userInfo = mysql_query(conn, "SELECT Username, Password FROM users");
        result = mysql_store_result(conn);

        if (result == NULL) {
            finish_with_error(conn);
        }

        MYSQL_ROW row;
        int numFields = mysql_num_fields(result);

        while ((row = mysql_fetch_row(result)) != NULL) { // Scans Database to check for same username and password the user has entered
            for (int i = 0;i < numFields;i++) { // Username Loop
                for (int j = 0;j < numFields;j++) {    // Password Loop                          
                    if ((strcmp(row[i], userA) == 0) && strcmp(row[j], passA) == 0) {
                        strcpy(username, row[i]);
                        strcpy(password, row[j]);
                        strcpy(usernamePtr, row[i]);
                        strcpy(passwordPtr, row[j]);
                        login = 1;
                        break;
                    }
                }
            }
        }
    mysql_free_result(result);        

    if (login == 1) {
        mysql_query(conn, "SELECT * FROM users");
        result = mysql_store_result(conn);

        if (result == NULL) {
            finish_with_error(conn);
        }

        numFields = mysql_num_fields(result);

        while ((row = mysql_fetch_row(result)) != NULL) {
            for (int i = 0;i < numFields;i++) {
                for (int j = 0;j < numFields;j++) {
                    if ((strcmp(row[i], userA) == 0) && strcmp(row[j], passA) == 0) {
                        row[i] = row[i - 1];
                        userID = atoi(row[i]);
                        break;
                    }
                }
            }
        }
        fflush(stdin);
        *userIDPtr = userID;
        *loginPtr = login;
        mysql_free_result(result);
        mysql_close(conn);
    }                      
}
void userInterface(int *loginPtr, char *usernamePtr, char *passwordPtr, int *userIDPtr){
    int menuOption;
    char username[50], password[50];
    int login = 0, userID;     


    strcpy(username, usernamePtr);
    strcpy(password, passwordPtr);    
    login = *loginPtr;            
    userID = *userIDPtr;

    if (login == 1) {                
        do {
            clr();            
            printf("Welcome Back %s!\n", username);
            printf("\nPassword Protected Personal Diary\n\n");
            printf("[1] New Entry\n");
            printf("[2] View Entry\n");
            printf("[3] Edit Entry\n");
            printf("[4] Change Background Color\n");
            printf("[5] Account Settings\n");
            printf("[0] Log Out\n\n");
            scanf("%d", &menuOption);
            switch (menuOption) {
            case 0:
                clr();
                system("COLOR 07");
                printf("\nLogged Out Successfully\n");
                login = 0;
                *loginPtr = login;
                break;
            case 1:                  
                addEntry(userIDPtr);
                break;
            case 4:
                changeBackground();
                break;
            case 5:
                accountSettings(loginPtr, usernamePtr, passwordPtr, userIDPtr);
                break;
            }
        } while (menuOption != 0);
    }
}
void changeBackground() {
    int menuOption;
    
    do {
        clr();
        printf("\nSelect a Color Theme\n");
        printf("\n[1] Light\n");
        printf("\n[2] Dark\n");
        printf("\n[3] Candy\n");
        printf("\n[4] Sunshine\n");
        printf("\n[5] Hacker\n");
        printf("\n[0] Exit\n");
        scanf("%d", &menuOption);
        clr();
        switch (menuOption) {
            case 1:
                system("COLOR 70");
                break;
            case 2:
                system("COLOR 07");
                break;
            case 3:
                system("COLOR 40");
                break;
            case 4:
                system("COLOR 60");
                break;
            case 5:
                system("COLOR 0A");
                break;
            }
    } while (menuOption != 0);    
}
void accountSettings(int* loginPtr, char* usernamePtr, char* passwordPtr, int* userIDPtr) {
    int menuOption, userID;
    char username[50], password[50];
    char passChange[50], passChangeConfirm[50], query[200];
    int validPass;
    
    MYSQL* conn = mysql_init(NULL);
    char* server = "localhost";
    char* user = "root";
    char* serverP = "Gunsaredangerous";
    char* database = "virtual_diary";

    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }
    if (mysql_real_connect(conn, server, user, serverP, database, 0, NULL, 0) == NULL) { // Connect to virtual_diary Database
        finish_with_error(conn);
    }

    strcpy(username, usernamePtr);
    strcpy(password, passwordPtr);
    userID = *userIDPtr;

    do {
        clr();
        printf("\n[1] View Account Details\n");
        printf("\n[2] Change Password\n");        
        printf("\n[0] Previous Menu\n");
        scanf("%d", &menuOption);
        switch (menuOption) {
            case 1:
                clr();
                printf("\nUserID = %d\n", userID);
                printf("\nUsername = %s\n", username);       
                printf("\nPress Any Button To Exit\n");
                _getch();
                break;
            case 2:
                clr();
                printf("\nEnter Current Password\n");
                scanf("%s", &passChange);                
                if (strcmp(passChange, password) == 0) {
                    clr();
                    mysql_query(conn, "SELECT * FROM users");
                    MYSQL_RES* result = mysql_store_result(conn);
                    int num_fields = mysql_num_fields(result);
                    MYSQL_ROW row;
                    while ((row = mysql_fetch_row(result))) {
                        for (int i = 0;i < num_fields;i++) {
                            if (strcmp(username, row[i]) == 0) {
                                do {                                                                        
                                    printf("\nEnter New Password\n");
                                    scanf("%s", &passChange);
                                    printf("\nConfirm New Password\n");
                                    scanf("%s", &passChangeConfirm);
                                    validPass = validPassword(passChange, passChangeConfirm);
                                } while (validPassword == 0);
                                if (strcmp(passChange, passChangeConfirm) == 0 && validPass == 1) {
                                    char updatePassword[200];
                                    strcpy(updatePassword, "UPDATE users SET Password='%s' WHERE Username='%s'"); // Update query for user
                                    snprintf(query, MAX_QUERY, updatePassword, passChange, username);                                                                        
                                    mysql_real_query(conn, query, strlen(query));                                    
                                    
                                    int id = mysql_insert_id(conn);                                                                        
                                    if (mysql_query(conn, query)) {
                                        clr();
                                        printf("\nPassword Was Not Changed Successfully\n");
                                        mysql_close(conn);  //Disconnect from server            
                                        break;
                                    }
                                    else{
                                        clr();
                                        printf("\nPassword Changed Successfully\n");
                                        mysql_close(conn);  //Disconnect from server            
                                        break;                                        
                                    }
                                }
                            }
                        }                    
                    }
                }                
        }
    } while (menuOption != 0);    
}
int validPassword(char* passwordPtr, char* passConfirmPtr) {
    char password[50], passConfirm[50], validPass = 0;
    strcpy(password, passwordPtr);
    strcpy(passConfirm, passConfirmPtr);

    if (strlen(password) < 8) {
        printf("\nPassword is Too Short\n");
    }
    else if (strlen(password) > 48) {
        printf("\nPassword is Too Long\n");
    }
    else if (strcmp(password, passConfirm) != 0) {
        printf("\nThe Two Passwords Don't Match\n");
    }
    else {
        return validPass = 1;
    }

    return validPass;
}
void addEntry(char* userIDPtr) {
    char newEntry[500], entryName[40];
    int numOfStr = 0, userID;    
    char ch;
    char confirm;
    char my_query[MAX_QUERY];

    userID = *userIDPtr;

    MYSQL* conn = mysql_init(NULL);
    char* res;
    char* server = "localhost";
    char* user = "root";
    char* serverP = "";
    char* database = "virtual_diary";    

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    do {
        clr();       
        printf("\nToday's Date: %02d-%d-%02d\n", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
        printf("\nWhat Would You Like To Name This Entry?\n");;
        while (fgets(entryName, 40, stdin)) {
            ch = _getch();
            if (ch == ENTER) {
                break;
            }            
            if (ch == ESC) {
                break;
            }
        }
        printf("\nWrite Your Thoughts : \t\t\t\t\t\t\t\t\tTo Exit Press Enter Then ESC\n");                        
        while (fgets(newEntry, 500, stdin)) {
            ch = _getch();
            if (ch == ESC) {
                printf("\nWould You Like to Save This Entry?(Y/N)\n");
                scanf(" %c", &confirm);                
                if (strcmp(confirm, 'y')==0 || strcmp(confirm, 'Y')==0) {
                    mysql_real_connect(conn, server, user, serverP, database, 0, NULL, 0);
                    snprintf(my_query, MAX_QUERY,
                        "INSERT INTO user_entry(entry_ID, entry_name, entry_message, entry_date, userID) VALUES ('%d', '%s', '%s', '%s', '%d')", NULL, entryName, newEntry, NULL, userID);
                    res = mysql_real_query(conn, newEntry, strlen(my_query));                        
                    if (res == NULL) {
                        printf("\nError Saving Entry\n");                        
                    mysql_close(conn);
                    }
                }                
            }
        }
        //printf("newEntry = %s", newEntry);        
    } while (ch != ESC);

}
void clr(){
    system("cls");
}
