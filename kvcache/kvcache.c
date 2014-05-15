#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"
#include "log.h"
#include "kvcache.h"

typedef struct _kvcache_t {
    sqlite3 *db;
} kvcache_t;

static char *kvcdb_sql_buff = NULL;
static int kvcdb_sql_buff_len = 0;

/* TODO: dlopen + dlsym, instead of link to libsqlite3.so */
/* static int *sqlite3_open_fptr; */
/* static int *sqlite3_close_fptr; */
/* static int *sqlite3_exec_fptr; */
/* static int *sqlite3_free_fptr; */

kvcache kvc_create(void) {
  sqlite3 *db = NULL;

  if(SQLITE_OK != sqlite3_open(":memory:", &db)) {
    return NULL;
  }

  const char *sql = "CREATE TABLE kvc (key TEXT NOT NULL UNIQUE PRIMARY KEY, value TEXT)";
  char *err = NULL;
  int ret = sqlite3_exec(db, sql, NULL, NULL, &err);
  if( ret != SQLITE_OK) {
    LOGE("ELF| Error excuting sql [%s]: %s", sql, err);
    if(err) { sqlite3_free(err); }
    sqlite3_close(db);
    return NULL;
  }

  LOGD("ELF| Open&Init key-value cache database successfully!");
  kvcache_t *rst = malloc(sizeof(kvcache_t));
  if(!rst) {
    LOGE("malloc(%d) failed!", sizeof(kvcache_t));
    sqlite3_close(db);
    return NULL;
  }
  rst->db = db;

  return (kvcache)rst;
}

void kvc_destroy(kvcache kvc) {
  if(!kvc) return;
  sqlite3_close(((kvcache_t *)kvc)->db);
  free(kvc);

  if(kvcdb_sql_buff && kvcdb_sql_buff_len > 0) {
    free(kvcdb_sql_buff);
    kvcdb_sql_buff = NULL;
    kvcdb_sql_buff_len = 0;
  }
}

int kvc_set(kvcache kvc, const char *key, const char *value) {
  if(!kvc || !((kvcache_t *)kvc)->db || !key || !value) {
    LOGE("bad argument: %s(%p, %p, %p)", __FUNCTION__, kvc, key, value);
    return -1;
  }

  const static char *sql_temp = "INSERT OR REPLACE INTO kvc ( key, value ) VALUES ( '%s', '%s' )";
  int len = strlen(sql_temp)+strlen(key) + strlen(value);
  if(len > kvcdb_sql_buff_len) {
    kvcdb_sql_buff = realloc(kvcdb_sql_buff, len);
    if( !kvcdb_sql_buff ) {
      return -1;
    }
    kvcdb_sql_buff_len = len;
  }
  snprintf(kvcdb_sql_buff, len, sql_temp, key, value);
  
  char *err = NULL;
  if( SQLITE_OK != sqlite3_exec(((kvcache_t *)kvc)->db, kvcdb_sql_buff, NULL, NULL, &err)) {
    LOGE("ELF| execute sql [%s] failed: %s", kvcdb_sql_buff, err);
    if(err) sqlite3_free(err);
    return -1;
  }

  return 0;
}

static int get_value(void *rst, int colnum, char **values, char **names) {
  *(int  *)rst = strdup(values[0]);
  return 0;
}

char *kvc_get(kvcache kvc, const char *key) {
  if(!kvc || !((kvcache_t *)kvc)->db || !key) {
    LOGE("bad argument: %s(%p, %p)", __FUNCTION__, kvc, key);
    return -1;
  }

  const static char *sql_temp = "SELECT value FROM kvc WHERE key='%s'";
  int len = strlen(sql_temp)+strlen(key);
  if(len > kvcdb_sql_buff_len) {
    kvcdb_sql_buff = realloc(kvcdb_sql_buff, len);
    if( !kvcdb_sql_buff ) {
      return (char *)-1;
    }
    kvcdb_sql_buff_len = len;
  }
  snprintf(kvcdb_sql_buff, len, sql_temp, key);
  
  char *err = NULL;
  char *value = NULL;
  if( SQLITE_OK != sqlite3_exec(((kvcache_t *)kvc)->db, kvcdb_sql_buff, get_value, &value, &err)) {
    LOGE("ELF| execute sql [%s] failed: %s", kvcdb_sql_buff, err);
    if(err) sqlite3_free(err);
    return (char *)-1;
  }
  
  return value;
}
