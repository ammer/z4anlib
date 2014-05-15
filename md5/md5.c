#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>
#include "log.h"
#include "md5.h"
#include "z4_md5.h"

static void hex2string(const unsigned char *hex, int hex_len, char *buff, int buf_len) {
    int i = 0;
    char HEX_DICT[0x10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    char *pc = hex+hex_len;
    while(i<buf_len-1 && hex < pc) {
        char c1 = (*hex)>>4;
        char c2 = (*hex)&0x0f;
        buff[i++] = HEX_DICT[c1];
        buff[i++] = HEX_DICT[c2];
        hex++;
    }
    buff[i] = 0;
}

int z4_md5_file(const char *file, char rst_trait[MD5_HEX_BUFF_LEN]) {
    MD5_CTX md;
    int ret;
    char *buff = NULL;
    unsigned char hex_md5[MD5_HEX_LEN];                 /* 128 bits */

    int fd = open(file, O_RDONLY);
    if(-1 == fd) {
        LOGE("Open file(%s) error: %s", file, strerror(errno));
        ret = -1;
        goto end;
    }
    
    int len = 1024*1024;	/* TODO: sizeof file */
    buff = malloc(len);
    if(!buff) {
      LOGE("malloc(%d) failed", len);
      ret = -1;
      goto end;
    }
    
    MD5_Init(&md);
    ssize_t size;
    do {
        size = read(fd, buff, len);
        if(size<=0) {
	  if(size < 0) {
	    LOGE("Read file(%s) error: %s", file, strerror(errno));
	    ret = -1;
	    goto end;
	  } /* allow empty file */
        }
        MD5_Update(&md, buff, size);
    } while(size>0);

    MD5_Final(hex_md5, &md);
    hex2string(hex_md5, MD5_HEX_LEN, rst_trait, MD5_HEX_BUFF_LEN);
    ret = 0;

 end:
    if(fd != -1)
        close(fd);
    if(buff)
        free(buff);

    return ret;
}

/* 
 * got md5 of a piece of data in DATA with length LEN, result will store to MD5STR
 */
void z4_md5_buff(const char *data, size_t len, char md5str[MD5_HEX_BUFF_LEN]) {
    MD5_CTX md;
    assert(MD5_HEX_BUFF_LEN >= 32);
    char hex_md5[MD5_HEX_LEN];

    MD5_Init(&md);
    MD5_Update(&md, data, len);
    MD5_Final(hex_md5, &md);
    hex2string(hex_md5, MD5_HEX_LEN, md5str, MD5_HEX_BUFF_LEN);
    return;
}
