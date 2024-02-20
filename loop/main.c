#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

void menu();
void addIP(FILE *catalogue);
void deleteIP(FILE *catalogue);

const char filename[] = "catalogue.txt";

int main(){

    system("clear");

    FILE *catalogue;

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

                break;

                case 's':

                break;

                case 'd':

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
    const char pattern[] = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";

    regi = regcomp(&regex, pattern, REG_EXTENDED);
    if (regi) {
        fprintf(stderr, "\n   Impossible de compiler l'expression régulière\n");
        exit(1);
    }
    char addr[20];
    printf("\n    Enter IP address : ");
    scanf(" %20s", addr);
    regi = regexec(&regex, addr, 0, NULL, 0);
    if (!regi) {

        catalogue = fopen(filename, "a+");
        fprintf(catalogue, "%s\n", addr);
        fclose(catalogue);
        printf("\n    IP successfully added !\n");

    } else if (regi == REG_NOMATCH) {
        printf("\n    Wrong IP format !\n");
    } else {
        char msgbuf[100];
        regerror(regi, &regex, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "\n   Erreur de regex : %s\n", msgbuf);
        exit(1);
    }

    regfree(&regex);

}


void deleteIP(FILE *catalogue){

}