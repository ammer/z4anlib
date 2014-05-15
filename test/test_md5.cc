#include <stdlib.h>
#include "gtest.h"
#include "log.h"
#include "z4_md5.h"

TEST(md5, buff) {
  char md5_str[MD5_HEX_BUFF_LEN];
  z4_md5_buff("", 0, md5_str);
  EXPECT_STREQ(md5_str, "d41d8cd98f00b204e9800998ecf8427e");
  z4_md5_buff("d41d8cd98f00b204e9800998ecf8427e\n", 33, md5_str);
  EXPECT_STREQ(md5_str, "227bc609651f929e367c3b2b79e09d5b");
}

TEST(md5, file) {
  char md5_str[MD5_HEX_BUFF_LEN]="";
  EXPECT_EQ(z4_md5_file("/data/local/tmp/empty_file.txt", md5_str), 0);
  EXPECT_STREQ(md5_str, "d41d8cd98f00b204e9800998ecf8427e");
  EXPECT_EQ(z4_md5_file("/data/local/tmp/file1.txt", md5_str), 0);
  EXPECT_STREQ(md5_str, "227bc609651f929e367c3b2b79e09d5b");
}
