ARCH=armeabi
PREFIX=arm-linux-androideabi-
CC=$(PREFIX)gcc
CXX=$(PREFIX)g++
LD=$(PREFIX)ld

INCLUDES=-I. -Iinc/ -Ivendor/sqlite-amalgamation-3080403 -Itest/ -Itest/gtest -Ikvcache
LDFLAGS=-Lvendor/sqlite-amalgamation-3080403/$(ARCH) -lsqlite

CFILES=kvcache/kvcache.c
OBJS=$(CFILES:.c=.o)

TESTBIN=out/test_z4lib
TEST_CXXFILES=test/gtest/gtest-all.cc test/gtest/gtest_main.cc test/test_kvcache.cc
TEST_OBJS=$(TEST_CXXFILES:.cc=.o)

test : $(TESTBIN)

$(TESTBIN) : $(TEST_OBJS) $(OBJS)
	$(CXX) -o $@ $(LDFLAGS) $?

.cc.o:
	$(CXX) -c -o $@ $(INCLUDES) $(CFLAGS) $?

.c.o:
	$(CC) -c -o $@ $(INCLUDES) $(CFLAGS) $?

clean :
	rm -fv $(OBJS) $(TEST_OBJS) $(TESTBIN) `find . -name "*~"`



