cmd_bench/mem-memset-x86-64-asm.o := gcc -Wp,-MD,bench/.mem-memset-x86-64-asm.o.d -Wp,-MT,bench/mem-memset-x86-64-asm.o -Wbad-function-cast -Wdeclaration-after-statement -Wformat-security -Wformat-y2k -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wno-system-headers -Wold-style-definition -Wpacked -Wredundant-decls -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wwrite-strings -Wformat -Wno-type-limits -Wstrict-aliasing=3 -Wshadow -DHAVE_SYSCALL_TABLE_SUPPORT -DHAVE_ARCH_X86_64_SUPPORT -Iarch/x86/include/generated -DHAVE_PERF_REGS_SUPPORT -DHAVE_ARCH_REGS_QUERY_REGISTER_OFFSET -Werror -O6 -fno-omit-frame-pointer -ggdb3 -funwind-tables -Wall -Wextra -std=gnu99 -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -I/home/exati/linux-hwe-5.15/tools/lib/perf/include -I/home/exati/linux-hwe-5.15/tools/perf/util/include -I/home/exati/linux-hwe-5.15/tools/perf/arch/x86/include -I/home/exati/linux-hwe-5.15/tools/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi -I/home/exati/linux-hwe-5.15/tools/include/uapi -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/ -I/home/exati/linux-hwe-5.15/tools/perf/util -I/home/exati/linux-hwe-5.15/tools/perf -I/home/exati/linux-hwe-5.15/tools/lib/ -DHAVE_PTHREAD_ATTR_SETAFFINITY_NP -DHAVE_PTHREAD_BARRIER -DHAVE_EVENTFD_SUPPORT -DHAVE_GET_CURRENT_DIR_NAME -DHAVE_GETTID -DHAVE_FILE_HANDLE -DHAVE_GLIBC_SUPPORT -DHAVE_AIO_SUPPORT -DHAVE_SCHED_GETCPU_SUPPORT -DHAVE_SETNS_SUPPORT -DNO_LIBPERL -DHAVE_TIMERFD_SUPPORT -DNO_LIBPYTHON -DNO_DEMANGLE -DHAVE_ZLIB_SUPPORT -DHAVE_BACKTRACE_SUPPORT -DHAVE_LIBNUMA_SUPPORT -DHAVE_KVM_STAT_SUPPORT -DHAVE_AUXTRACE_SUPPORT -D"BUILD_STR(s)=$(pound)s" -c -o bench/mem-memset-x86-64-asm.o bench/mem-memset-x86-64-asm.S

source_bench/mem-memset-x86-64-asm.o := bench/mem-memset-x86-64-asm.S

deps_bench/mem-memset-x86-64-asm.o := \
  /usr/include/stdc-predef.h \
  bench/../../arch/x86/lib/memset_64.S \
  /home/exati/linux-hwe-5.15/tools/perf/util/include/linux/linkage.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/asm/cpufeatures.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/asm/required-features.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/asm/disabled-features.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/alternative.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/export.h \

bench/mem-memset-x86-64-asm.o: $(deps_bench/mem-memset-x86-64-asm.o)

$(deps_bench/mem-memset-x86-64-asm.o):
