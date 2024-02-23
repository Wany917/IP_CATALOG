//
// Created by Yvann on 21/02/2024.
//

#include "declarefunc.h"
const char pattern[] = "^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$";
char *masks[] = {
    "128.0.0.0",
    "192.0.0.0",
    "224.0.0.0",
    "240.0.0.0",
    "248.0.0.0",
    "252.0.0.0",
    "254.0.0.0",
    "255.0.0.0",
    "255.128.0.0",
    "255.192.0.0",
    "255.224.0.0",
    "255.240.0.0",
    "255.248.0.0",
    "255.252.0.0",
    "255.254.0.0",
    "255.255.0.0",
    "255.255.128.0",
    "255.255.192.0",
    "255.255.224.0",
    "255.255.240.0",
    "255.255.248.0",
    "255.255.252.0",
    "255.255.254.0",
    "255.255.255.0",
    "255.255.255.128",
    "255.255.255.192",
    "255.255.255.224",
    "255.255.255.240",
    "255.255.255.248",
    "255.255.255.252",
    "255.255.255.254",
    "255.255.255.255"
};

void add_ip_to_database(sqlite3 *db, const char *addr, const char *mask) {
    char *err_msg = 0;
    char sql[200];

    sprintf(sql, "INSERT INTO Ips(Address, Mask) VALUES('%s', '%s');", addr, mask);

    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to insert data: %s\n", err_msg);
        sqlite3_free(err_msg);
    } else {
        fprintf(stdout, "Address and Mask added successfully\n");
    }
}
void add_ip_callback(GtkWidget *widget, gpointer data)
{
    IpInputData *input_data = (IpInputData *)data;

    GtkEntryBuffer *buffer_ip = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_ip));
    GtkEntryBuffer *buffer_mask = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_mask));

    const gchar *ip = gtk_entry_buffer_get_text(buffer_ip);
    const gchar *mask = gtk_entry_buffer_get_text(buffer_mask);

    if (!ip || !*ip || !mask || !*mask) {
        fprintf(stderr, "IP or Mask is empty\n");
        return;
    }

    regex_t regex;
    int regi;

    regi = regcomp(&regex, pattern, REG_EXTENDED);
    if (regi)
    {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    regi = regexec(&regex, ip, 0, NULL, 0);
    if (regi == REG_NOMATCH)
    {
        printf("Bad IP Address\n");
        regfree(&regex);
        return;
    }

    gboolean mask_is_valid = FALSE;
    for (unsigned long i = 0; i < (sizeof(masks) / sizeof(masks[0])); i++)
    {
        if (strcmp(mask, masks[i]) == 0)
        {
            mask_is_valid = TRUE;
            break;
        }
    }

    if (!mask_is_valid)
    {
        printf("Bad Mask\n");
        regfree(&regex);
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("ip_catalogue.db", &db);
    add_ip_to_database(db, ip, mask);
    regfree(&regex);
    printf("Done : %s/%s\n", ip, mask);
}
void delete_ip_callback(GtkWidget *widget, gpointer data)
{
    IpInputData *input_data = (IpInputData *)data;
    GtkEntryBuffer *buffer_ip = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_ip));
    GtkEntryBuffer *buffer_mask = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_mask));

    const gchar *ip = gtk_entry_buffer_get_text(buffer_ip);
    const gchar *mask = gtk_entry_buffer_get_text(buffer_mask);

    if (!ip || !*ip || !mask || !*mask) {
        fprintf(stderr, "IP or Mask is empty\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("ip_catalogue.db", &db);
    char *err_msg = 0;
    char sql[200];

    sprintf(sql, "DELETE FROM Ips WHERE Address='%s' AND Mask='%s';", ip, mask);

    int rc2 = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc2 != SQLITE_OK)
    {
        fprintf(stderr, "Failed to delete data: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        fprintf(stdout, "Address and Mask deleted successfully\n");
    }
    g_free(input_data);
}
void search_ip_callback(GtkWidget *widget, gpointer entry)
{
    IpInputData *input_data = (IpInputData *)entry;

    GtkEntryBuffer *buffer_ip = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_ip));
    GtkEntryBuffer *buffer_mask = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_mask));

    const gchar *ip = gtk_entry_buffer_get_text(buffer_ip);
    const gchar *mask = gtk_entry_buffer_get_text(buffer_mask);

    if (!ip || !*ip || !mask || !*mask) {
        fprintf(stderr, "IP or Mask is empty\n");
        return;
    }

    sqlite3 *db;
    int rc = sqlite3_open("ip_catalogue.db", &db);
    char *err_msg = 0;
    char sql[200];

    sprintf(sql, "SELECT * FROM Ips WHERE Address='%s' AND Mask='%s';", ip, mask);

    sqlite3_stmt *res;
    rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        while (sqlite3_step(res) == SQLITE_ROW)
        {
            printf("%s| %s\n", sqlite3_column_text(res, 0), sqlite3_column_text(res, 1));
        }
    }
}
/*void convert_ip_to_binary_callback(GtkWidget *widget, gpointer entry)
{
    IpInputData *input_data = (IpInputData *)entry;

    GtkEntryBuffer *buffer_ip = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_ip));
    GtkEntryBuffer *buffer_mask = gtk_entry_get_buffer(GTK_ENTRY(input_data->entry_mask));

    const gchar *ip = gtk_entry_buffer_get_text(buffer_ip);
    const gchar *mask = gtk_entry_buffer_get_text(buffer_mask);

    if (!ip || !*ip || !mask || !*mask) {
        fprintf(stderr, "IP or Mask is empty\n");
        return;
    }

    regex_t regex;
    int regi;

    regi = regcomp(&regex, pattern, REG_EXTENDED);
    if (regi)
    {
        fprintf(stderr, "Could not compile regex\n");
        return;
    }

    regi = regexec(&regex, ip, 0, NULL, 0);
    if (regi == REG_NOMATCH)
    {
        printf("Bad IP Address\n");
        regfree(&regex);
        return;
    }

    gboolean mask_is_valid = FALSE;
    for (unsigned long i = 0; i < (sizeof(masks) / sizeof(masks[0])); i++)
    {
        if (strcmp(mask, masks[i]) == 0)
        {
            mask_is_valid = TRUE;
            break;
        }
    }

    if (!mask_is_valid)
    {
        printf("Bad Mask\n");
        regfree(&regex);
        return;
    }

    char *ip_binary = g_new0(char, 35);
    char *mask_binary = g_new0(char, 35);

    char *ip_copy = g_strdup(ip);
    char *mask_copy = g_strdup(mask);

    char *token = strtok(ip_copy, ".");
    while (token != NULL)
    {
        int num = atoi(token);
        char *binary = g_new0(char, 9);
        for (int i = 7; i >= 0; i--)
        {
            binary[i] = (num % 2) + '0';
            num /= 2;
        }
        strcat(ip_binary, binary);
        strcat(ip_binary, ".");
        token = strtok(NULL, ".");
    }
    ip_binary[strlen(ip_binary) - 1] = '\0';

    token = strtok(mask_copy, ".");
    while (token != NULL){
        int num = atoi(token);
        char *binary = g_new0(char, 9);
        for (int i = 7; i >= 0; i--)
        {
            binary[i] = (num % 2) + '0';
            num /= 2;
        }
        strcat(mask_binary, binary);
        strcat(mask_binary, ".");
        token = strtok(NULL, ".");
    }
    mask_binary[strlen(mask_binary) - 1] = '\0';
    printf("IP: %s\n", ip_binary);
    printf("Mask: %s\n", mask_binary);

    g_free(ip_binary);
    g_free(mask_binary);
    g_free(ip_copy);
    g_free(mask_copy);
    regfree(&regex);
}
*/