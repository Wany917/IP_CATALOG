#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

void menu();
void addIP(FILE *catalogue);
void deleteIP(FILE *catalogue);
void displayIP(FILE *catalogue);

const char filename[] = "catalogue.txt";
const char pattern[] = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";

int main(){

    system("clear");

    FILE *catalogue;

    catalogue = fopen(filename, "a+");
    fclose(catalogue);

    char option = ' ';

    while(1 == 1){
        system("clear");
        menu();

        printf("    What do you want to do ? : ");
        scanf(" %s", &option);
        getchar();
        printf("\n");

        if(strlen(&option) == 1){
            switch(option){
                case 'a':
                    addIP(catalogue);
                break;

                case 'l':
                    displayIP(catalogue);
                break;

                case 's':

                break;

                case 'd':
                    deleteIP(catalogue);
                break;

                case 'q':
                    printf("\n    --------\n    Exited !\n    --------\n");
                    exit(0);
                break;

                default:
                    printf("\n    This is not a valid option !\n\n");
            }
        }else{
            printf("\n    This is not a valid option !\n\n");
        }
        usleep(750000);
        system("clear");

    }
    return 0;
}

void menu() {

    printf("\
     ::::::::                   ::: ::::::::::: :::     :::        :::::::   ::::::::  :::   ::: ::::::::::\n\
    :+:    :+:                :+: :+:   :+:   :+: :+:   :+:       :+:   :+: :+:    :+: :+:   :+: :+:\n\
    +:+                      +:+   +:+  +:+  +:+   +:+  +:+       +:+   +:+ +:+        +:+   +:+ +:+\n\
    +#+       +#++:++#++:++ +#++:+#++:  +#+ +#+#++++#+  +#+       +#+   +:+ :#:        +#+   +:+ +#+:#++:+#\n\
    +#+                     +#+    +#+  +#+ +#+    +#+  +#+       +#+   +#+ +#+   +#+  +#+   +#+ +#+\n\
    #+#    #+#              #+#    #+#  #+# #+#    #+#  #+#       #+#   #+# #+#    #+# #+#   #+# #+#\n\
    ########               ###     ###  ### ###    ###  ########   #######   ########   #######  ##########\n\n");

    printf("    a - Add a new IP address\n");
    printf("    l - List IP addresses\n");
    printf("    s - Search similar by mask\n");
    printf("    d - Delete an IP\n");
    printf("    q - quit\n\n\n");

}

void addIP(FILE *catalogue){
    regex_t regex;
    int regi;

    regi = regcomp(&regex, pattern, REG_EXTENDED);
    if (regi) {
        fprintf(stderr, "\n   Error while compilating regex !\n");
        exit(1);
    }
    char addr[20];
    printf("    Enter IP address : ");
    scanf(" %20s", addr);
    regi = regexec(&regex, addr, 0, NULL, 0);
    if (!regi) {

        catalogue = fopen(filename, "r");
        if (catalogue == NULL) {
            printf("Error while trying to open the file %s.\n", filename);
            exit(1);
        }

        char line[50];

        int isIP = 0;

        while (fgets(line, sizeof(line), catalogue) != NULL) {
            if (strstr(line, addr) != NULL) {
                isIP = 1;
            }else{
                isIP = 0;
            }
        }

        if(isIP == 0){
            catalogue = fopen(filename, "a+");
            fprintf(catalogue, "%s\n", addr);
            fclose(catalogue);
            printf("\n    IP successfully added !\n");
        }else if(isIP == 1){
            printf("\n    Ip : %s is already registered !\n", addr);
        }

    } else if (regi == REG_NOMATCH) {
        printf("\n    Wrong IP format !\n");
    } else {
        char msgbuf[100];
        regerror(regi, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "\n   Regex error : %s\n", msgbuf);
        exit(1);
    }

    regfree(&regex);

}


void deleteIP(FILE *catalogue){

    FILE *check;
    char line[50];
    char addr[50];
    int isIP = 0;

    printf("    Enter Ip do you want to remove : ");
    scanf(" %s", addr);

    catalogue = fopen(filename, "r+");
    if (catalogue == NULL) {
        printf("    Error while trying to open : %s !\n", filename);
        exit(1);
    }

    check = fopen("check.txt", "w");
    if (check == NULL) {
        printf("    Error while trying to create check file !\n");
        fclose(check);
        exit(1);
    }

    while (fgets(line, sizeof(line), catalogue) != NULL) {
        if (strstr(line, addr) == NULL) {
            fputs(line, check);
        }
    }

    fclose(catalogue);
    fclose(check);

    remove(filename);

    rename("check.txt", filename);

    printf("\n    Ip : \"%s\" successfully removed !\n", addr);

}

void displayIP(FILE *catalogue){

    char line[50];

    catalogue = fopen(filename, "r");
    if (catalogue == NULL) {
        printf("    Impossible d'ouvrir le fichier d'ips !\n");
        exit(1);
    }

    printf("    Liste des IPS :\n\n");
    while (fgets(line, sizeof(line), catalogue) != NULL) {
        printf("    %s", line);
    }

    fclose(catalogue);

    printf("\n    Enter r to return to menu !\n");
    
    char ret = ' ';

    scanf(" %s", &ret);

    while(ret != 'r'){
        scanf(" %s", &ret);
    }

}