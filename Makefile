ARCH=armeabi
PREFIX=arm-linux-androideabi-
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld

INCLUDES=-I. -Iinc/ -Ivendor/sqlite-amalgamation-3080403 -Itest/ -Itest/gtest -Ikvcache -Imd5
LDFLAGS=-Lvendor/sqlite-amalgamation-3080403/$(ARCH) -lsqlite -llog

CFILES=kvcache/kvcache.c md5/md5.c
OBJS=$(CFILES:.c=.o)

TESTBIN=out/test_z4lib
TEST_CXXFILES=test/gtest/gtest-all.cc test/gtest/gtest_main.cc test/test_kvcache.cc test/test_md5.cc
TEST_OBJS=$(TEST_CXXFILES:.cc=.o)

test : $(TESTBIN)
	adb push $(TESTBIN) /data/local/tmp/
	adb push test/empty_file.txt /data/local/tmp/
	adb push test/file1.txt /data/local/tmp/
	adb shell /data/local/tmp/`basename $(TESTBIN)`

$(TESTBIN) : $(TEST_OBJS) $(OBJS)
	$(CXX) -o $@ $(LDFLAGS) $?

.cc.o:
	$(CXX) -c -o $@ $(INCLUDES) $(CFLAGS) $?

.c.o:
	$(CC) -c -o $@ $(INCLUDES) $(CFLAGS) $?

clean :
	rm -fv $(OBJS) $(TEST_OBJS) $(TESTBIN) `find . -name "*~"`



