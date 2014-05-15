#include <stdlib.h>
#include "gtest.h"
#include "log.h"
#include "kvcache.h"

TEST(kvcache, create_and_close) {
  kvcache kvc = kvc_create();
  EXPECT_NE(kvc, (void *)NULL);
  kvc_destroy(kvc);
}

TEST(kvcache, set_and_get) {
  const char *key = "hello";
  const char *value = "world";
  kvcache kvc = kvc_create();
  EXPECT_EQ(kvc_get(kvc, key), (void *)NULL);
  EXPECT_EQ(kvc_set(kvc, key, value), 0);
  EXPECT_STREQ(kvc_get(kvc, key), value);
  kvc_destroy(kvc);
}
