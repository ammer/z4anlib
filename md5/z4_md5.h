#ifndef _Z4_MD5_H
#define  _Z4_MD5_H

#ifdef __cplusplus
extern "C" {
#endif


#define MD5_HEX_LEN 16
#define MD5_HEX_STR_LEN 32
#define MD5_HEX_BUFF_LEN (MD5_HEX_STR_LEN+4)


  extern int z4_md5_file(const char *file, char rst_trait[MD5_HEX_BUFF_LEN]);
  extern void z4_md5_buff(const char *data, size_t len, char md5str[MD5_HEX_BUFF_LEN]);


#ifdef __cplusplus
}
#endif
#endif	/*  _Z4_MD5_H */
