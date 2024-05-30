#include <stdio.h>
#include "database.h"

sqlite3 *db = NULL;

int init_db(const char *db_name)
{
    int rc = sqlite3_open(db_name, &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return rc;
    }
    return SQLITE_OK;
}

void close_db()
{
    if (db)
    {
        sqlite3_close(db);
        db = NULL;
    }
}

void execute_query(const char *sql)
{
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    else
    {
        printf("Query executed successfully\n");
    }
}
