cmd_arch/x86/tests/arch-tests.o := gcc -Wp,-MD,arch/x86/tests/.arch-tests.o.d -Wp,-MT,arch/x86/tests/arch-tests.o -Wbad-function-cast -Wdeclaration-after-statement -Wformat-security -Wformat-y2k -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wno-system-headers -Wold-style-definition -Wpacked -Wredundant-decls -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wwrite-strings -Wformat -Wno-type-limits -Wstrict-aliasing=3 -Wshadow -DHAVE_SYSCALL_TABLE_SUPPORT -DHAVE_ARCH_X86_64_SUPPORT -Iarch/x86/include/generated -DHAVE_PERF_REGS_SUPPORT -DHAVE_ARCH_REGS_QUERY_REGISTER_OFFSET -Werror -O6 -fno-omit-frame-pointer -ggdb3 -funwind-tables -Wall -Wextra -std=gnu99 -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -I/home/exati/linux-hwe-5.15/tools/lib/perf/include -I/home/exati/linux-hwe-5.15/tools/perf/util/include -I/home/exati/linux-hwe-5.15/tools/perf/arch/x86/include -I/home/exati/linux-hwe-5.15/tools/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi -I/home/exati/linux-hwe-5.15/tools/include/uapi -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/ -I/home/exati/linux-hwe-5.15/tools/perf/util -I/home/exati/linux-hwe-5.15/tools/perf -I/home/exati/linux-hwe-5.15/tools/lib/ -DHAVE_PTHREAD_ATTR_SETAFFINITY_NP -DHAVE_PTHREAD_BARRIER -DHAVE_EVENTFD_SUPPORT -DHAVE_GET_CURRENT_DIR_NAME -DHAVE_GETTID -DHAVE_FILE_HANDLE -DHAVE_GLIBC_SUPPORT -DHAVE_AIO_SUPPORT -DHAVE_SCHED_GETCPU_SUPPORT -DHAVE_SETNS_SUPPORT -DNO_LIBPERL -DHAVE_TIMERFD_SUPPORT -DNO_LIBPYTHON -DNO_DEMANGLE -DHAVE_ZLIB_SUPPORT -DHAVE_BACKTRACE_SUPPORT -DHAVE_LIBNUMA_SUPPORT -DHAVE_KVM_STAT_SUPPORT -DHAVE_AUXTRACE_SUPPORT -D"BUILD_STR(s)=$(pound)s" -c -o arch/x86/tests/arch-tests.o arch/x86/tests/arch-tests.c

source_arch/x86/tests/arch-tests.o := arch/x86/tests/arch-tests.c

deps_arch/x86/tests/arch-tests.o := \
  /usr/include/stdc-predef.h \
  /usr/include/string.h \
  /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
  /usr/include/features.h \
  /usr/include/features-time64.h \
  /usr/include/x86_64-linux-gnu/bits/wordsize.h \
  /usr/include/x86_64-linux-gnu/bits/timesize.h \
  /usr/include/x86_64-linux-gnu/sys/cdefs.h \
  /usr/include/x86_64-linux-gnu/bits/long-double.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h \
  /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
  /usr/include/strings.h \
  /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
  /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
  /home/exati/linux-hwe-5.15/tools/perf/tests/tests.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdbool.h \
  /home/exati/linux-hwe-5.15/tools/perf/arch/x86/include/arch-tests.h \

arch/x86/tests/arch-tests.o: $(deps_arch/x86/tests/arch-tests.o)

$(deps_arch/x86/tests/arch-tests.o):
