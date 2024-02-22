#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>

#define MAX_IP_LEN 20
#define MAX_LINE_LEN 50

void menu();
void addIP(FILE *catalogue);
void deleteIP(FILE *catalogue);
void displayIP(FILE *catalogue);
void searchByMask(FILE *catalogue);

const char filename[] = "catalogue.txt";
const char pattern[] = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
char *masks[] = {"128.0.0.0","192.0.0.0","224.0.0.0","240.0.0.0","248.0.0.0","252.0.0.0","254.0.0.0","255.0.0.0","255.128.0.0","255.192.0.0","255.224.0.0","255.240.0.0","255.248.0.0","255.252.0.0","255.254.0.0","255.255.0.0","255.255.128.0","255.255.192.0","255.255.224.0","255.255.240.0","255.255.248.0","255.255.252.0","255.255.254.0","255.255.255.0","255.255.255.128","255.255.255.192","255.255.255.224","255.255.255.240","255.255.255.248","255.255.255.252","255.255.255.254","255.255.255.255"};

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
                    searchByMask(catalogue);
                break;

                case 'd':
                    deleteIP(catalogue);
                break;

                case 'q':
                    printf("\n    --------\n    Exited !\n    --------\n");
                    exit(0);
                break;

                default:
                    printf("    This is not a valid option !\n\n");
            }
        }else{
            printf("    This is not a valid option !\n\n");
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

            char mask[20];
            printf("\n    Enter Mask : ");
            scanf(" %20s", mask);

            int found = 0;

            for (int i = 0; i < sizeof(masks) / sizeof(masks[0]); i++) {
                if (strcmp(masks[i], mask) == 0) {
                    found = 1;
                    break;
                }
            }

            if (found) {
                
                int len1 = strlen(addr);
                int len2 = strlen(mask);

                char addrmask[len1 + len2 + 3];

                strcpy(addrmask, addr);
                
                strcat(addrmask, " / ");
                
                strcat(addrmask, mask);

                catalogue = fopen(filename, "a+");
                fprintf(catalogue, "%s\n", addrmask);
                fclose(catalogue);
                printf("\n    IP successfully added !\n");

            } else {
                printf("\n    Mask : \"%s\" isn't a correct mask !\n", mask);
            }

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
    char mask[50];
    int isIP = 0;

    printf("    Enter Ip that you want to remove : ");
    scanf(" %s", addr);
    printf("\n    Enter Mask of the IP that you want to remove : ");
    scanf(" %s", mask);

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

    int len1 = strlen(addr);
    int len2 = strlen(mask);

    char addrmask[len1 + len2 + 3];

    strcpy(addrmask, addr);
                
    strcat(addrmask, " / ");
                
    strcat(addrmask, mask);

    while (fgets(line, sizeof(line), catalogue) != NULL) {
        if (strstr(line, addrmask) != NULL) {
            isIP = 1;
        }else{
            isIP = 0;
        }
    }

    if(isIP == 1){

        while (fgets(line, sizeof(line), catalogue) != NULL) {
            if (strstr(line, addrmask) == NULL) {
                fputs(line, check);
            }
        }

        fclose(catalogue);
        fclose(check);

        remove(filename);

        rename("check.txt", filename);

        printf("\n    Ip : \"%s\" successfully removed !\n", addrmask);

    }else if(isIP == 0){
        printf("\n    Ip : %s isn't registered !\n", addrmask);
    }
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


void searchByMask(FILE *catalogue) {
    regex_t regex;
    char inputIP[MAX_IP_LEN], inputMask[MAX_IP_LEN];
    char line[MAX_LINE_LEN];
    int count = 0;

    // Compile et valide l'expression régulière
    if (regcomp(&regex, "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$", REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    printf("Enter IP address: ");
    scanf("%19s", inputIP);
    if (regexec(&regex, inputIP, 0, NULL, 0)) {
        printf("Invalid IP address format.\n");
        regfree(&regex);
        return;
    }

    printf("Enter subnet mask: ");
    scanf("%19s", inputMask);
    if (regexec(&regex, inputMask, 0, NULL, 0)) {
        printf("Invalid subnet mask format.\n");
        regfree(&regex);
        return;
    }
    regfree(&regex);

    // Tokenise et stocke les segments de l'adresse IP et du masque saisis
    int inputIPOctets[4], inputMaskOctets[4];
    char *token = strtok(inputIP, ".");
    for (int i = 0; i < 4 && token != NULL; i++) {
        inputIPOctets[i] = atoi(token);
        token = strtok(NULL, ".");
    }
    token = strtok(inputMask, ".");
    for (int i = 0; i < 4 && token != NULL; i++) {
        inputMaskOctets[i] = atoi(token);
        token = strtok(NULL, ".");
    }

    catalogue = fopen("catalogue.txt", "r");
    if (!catalogue) {
        perror("Failed to open file");
        return;
    }

    while (fgets(line, sizeof(line), catalogue)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = 0;

        char *fileIP = strtok(line, " / ");
        char *fileMask = strtok(NULL, " / ");
        if (!fileIP || !fileMask) continue;

        int fileIPOctets[4], match = 1;
        token = strtok(fileIP, ".");
        for (int i = 0; i < 4 && token != NULL; i++) {
            fileIPOctets[i] = atoi(token);
            token = strtok(NULL, ".");
        }

        // Compare chaque segment de l'adresse IP et du masque
        for (int i = 0; i < 4; i++) {
            if ((inputIPOctets[i] & inputMaskOctets[i]) != (fileIPOctets[i] & inputMaskOctets[i])) {
                match = 0;
                break;
            }
        }

        if (match) {
            if (count == 0) {
                printf("Addresses found:\n");
            }
            count++;

            // Reconstruit l'adresse IP complète à partir des segments
            char fullIP[20]; // Assurez-vous que cette taille est suffisante pour contenir une adresse IP complète
            snprintf(fullIP, sizeof(fullIP), "%d.%d.%d.%d", fileIPOctets[0], fileIPOctets[1], fileIPOctets[2], fileIPOctets[3]);

            printf("%d- %s\n", count, fullIP); // Utilisez `fullIP` pour afficher l'adresse IP complète
        }
    }

    fclose(catalogue);

    if (count == 0) {
        printf("No addresses found.\n");
    }

    printf("\n    Enter r to return to menu !\n");
    
    char ret = ' ';

    scanf(" %s", &ret);

    while(ret != 'r'){
        scanf(" %s", &ret);
    }
}

