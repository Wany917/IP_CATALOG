//
// Created by Yvann on 21/02/2024.
//
#include "declarefunc.h"
int init_db(const char *db_path) {
    sqlite3 *db;
    char *err_msg = NULL;
    int rc;

    char *full_path = realpath(db_path, NULL);
    printf("Full database path: %s\n", full_path);
    free(full_path);

    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Ips ("
                      "Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "Address TEXT NOT NULL, "
                      "Mask TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return rc;
    }

    sqlite3_close(db);
    return SQLITE_OK;
}
