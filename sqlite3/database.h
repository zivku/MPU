#ifndef DATABASE_H
#define DATABASE_H
#ifdef __cplusplus
extern "C"
{
#endif
#include <sqlite3.h>

    // 初始化資料庫連線
    int init_db(const char *db_name);

    // 關閉資料庫連線
    void close_db();

    // 執行查詢
    void execute_query(const char *sql);

    extern sqlite3 *db; // 宣告全局資料庫連線

#ifdef __cplusplus
}
#endif

#endif
