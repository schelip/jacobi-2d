cmd_tests/vmlinux-kallsyms.o := gcc -Wp,-MD,tests/.vmlinux-kallsyms.o.d -Wp,-MT,tests/vmlinux-kallsyms.o -Wbad-function-cast -Wdeclaration-after-statement -Wformat-security -Wformat-y2k -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wno-system-headers -Wold-style-definition -Wpacked -Wredundant-decls -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wwrite-strings -Wformat -Wno-type-limits -Wstrict-aliasing=3 -Wshadow -DHAVE_SYSCALL_TABLE_SUPPORT -DHAVE_ARCH_X86_64_SUPPORT -Iarch/x86/include/generated -DHAVE_PERF_REGS_SUPPORT -DHAVE_ARCH_REGS_QUERY_REGISTER_OFFSET -Werror -O6 -fno-omit-frame-pointer -ggdb3 -funwind-tables -Wall -Wextra -std=gnu99 -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -I/home/exati/linux-hwe-5.15/tools/lib/perf/include -I/home/exati/linux-hwe-5.15/tools/perf/util/include -I/home/exati/linux-hwe-5.15/tools/perf/arch/x86/include -I/home/exati/linux-hwe-5.15/tools/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi -I/home/exati/linux-hwe-5.15/tools/include/uapi -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/ -I/home/exati/linux-hwe-5.15/tools/perf/util -I/home/exati/linux-hwe-5.15/tools/perf -I/home/exati/linux-hwe-5.15/tools/lib/ -DHAVE_PTHREAD_ATTR_SETAFFINITY_NP -DHAVE_PTHREAD_BARRIER -DHAVE_EVENTFD_SUPPORT -DHAVE_GET_CURRENT_DIR_NAME -DHAVE_GETTID -DHAVE_FILE_HANDLE -DHAVE_GLIBC_SUPPORT -DHAVE_AIO_SUPPORT -DHAVE_SCHED_GETCPU_SUPPORT -DHAVE_SETNS_SUPPORT -DNO_LIBPERL -DHAVE_TIMERFD_SUPPORT -DNO_LIBPYTHON -DNO_DEMANGLE -DHAVE_ZLIB_SUPPORT -DHAVE_BACKTRACE_SUPPORT -DHAVE_LIBNUMA_SUPPORT -DHAVE_KVM_STAT_SUPPORT -DHAVE_AUXTRACE_SUPPORT -D"BUILD_STR(s)=$(pound)s" -c -o tests/vmlinux-kallsyms.o tests/vmlinux-kallsyms.c

source_tests/vmlinux-kallsyms.o := tests/vmlinux-kallsyms.c

deps_tests/vmlinux-kallsyms.o := \
  /usr/include/stdc-predef.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler_types.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler-gcc.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/types.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdbool.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h \
  /usr/include/stdint.h \
  /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
  /usr/include/features.h \
  /usr/include/features-time64.h \
  /usr/include/x86_64-linux-gnu/bits/wordsize.h \
  /usr/include/x86_64-linux-gnu/bits/timesize.h \
  /usr/include/x86_64-linux-gnu/sys/cdefs.h \
  /usr/include/x86_64-linux-gnu/bits/long-double.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
  /usr/include/x86_64-linux-gnu/bits/types.h \
  /usr/include/x86_64-linux-gnu/bits/typesizes.h \
  /usr/include/x86_64-linux-gnu/bits/time64.h \
  /usr/include/x86_64-linux-gnu/bits/wchar.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-intn.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
  /usr/include/x86_64-linux-gnu/asm/types.h \
  /usr/include/asm-generic/types.h \
  /usr/include/asm-generic/int-ll64.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi/asm/bitsperlong.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitsperlong.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/asm-generic/bitsperlong.h \
  /usr/include/x86_64-linux-gnu/asm/posix_types.h \
  /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
  /usr/include/asm-generic/posix_types.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/rbtree.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/kernel.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdarg.h \
  /usr/include/assert.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/build_bug.h \
  /usr/include/endian.h \
  /usr/include/x86_64-linux-gnu/bits/endian.h \
  /usr/include/x86_64-linux-gnu/bits/endianness.h \
  /usr/include/x86_64-linux-gnu/bits/byteswap.h \
  /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
  /usr/include/byteswap.h \
  /usr/include/linux/stddef.h \
  /usr/include/inttypes.h \
  /usr/include/string.h \
  /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
  /usr/include/strings.h \
  /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
  /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
  /usr/include/stdlib.h \
  /usr/include/x86_64-linux-gnu/bits/waitflags.h \
  /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
  /usr/include/x86_64-linux-gnu/bits/floatn.h \
  /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
  /usr/include/x86_64-linux-gnu/sys/types.h \
  /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
  /usr/include/x86_64-linux-gnu/sys/select.h \
  /usr/include/x86_64-linux-gnu/bits/select.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigset_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__sigset_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_timeval.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_timespec.h \
  /usr/include/x86_64-linux-gnu/bits/select2.h \
  /usr/include/x86_64-linux-gnu/bits/pthreadtypes.h \
  /usr/include/x86_64-linux-gnu/bits/thread-shared-types.h \
  /usr/include/x86_64-linux-gnu/bits/pthreadtypes-arch.h \
  /usr/include/x86_64-linux-gnu/bits/atomic_wide_counter.h \
  /usr/include/x86_64-linux-gnu/bits/struct_mutex.h \
  /usr/include/x86_64-linux-gnu/bits/struct_rwlock.h \
  /usr/include/alloca.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib-bsearch.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib-float.h \
  /usr/include/x86_64-linux-gnu/bits/stdlib.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/dso.h \
  /usr/include/pthread.h \
  /usr/include/sched.h \
  /usr/include/x86_64-linux-gnu/bits/sched.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h \
  /usr/include/x86_64-linux-gnu/bits/cpu-set.h \
  /usr/include/time.h \
  /usr/include/x86_64-linux-gnu/bits/time.h \
  /usr/include/x86_64-linux-gnu/bits/timex.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
  /usr/include/x86_64-linux-gnu/bits/setjmp.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct___jmp_buf_tag.h \
  /usr/include/x86_64-linux-gnu/bits/pthread_stack_min-dynamic.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/refcount.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/../../arch/x86/include/asm/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/../../arch/x86/include/asm/rmwcc.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/asm/cmpxchg.h \
  /usr/include/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/cookie_io_functions_t.h \
  /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
  /usr/include/x86_64-linux-gnu/bits/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/stdio2.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/bitops.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/limits.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/syslimits.h \
  /usr/include/limits.h \
  /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
  /usr/include/x86_64-linux-gnu/bits/local_lim.h \
  /usr/include/linux/limits.h \
  /usr/include/x86_64-linux-gnu/bits/posix2_lim.h \
  /usr/include/x86_64-linux-gnu/bits/xopen_lim.h \
  /usr/include/x86_64-linux-gnu/bits/uio_lim.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/bits.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/const.h \
  /home/exati/linux-hwe-5.15/tools/include/vdso/const.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/linux/const.h \
  /home/exati/linux-hwe-5.15/tools/include/vdso/bits.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/__ffs.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/__ffz.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/fls.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/__fls.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/fls64.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/find.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/hweight.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/arch_hweight.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/const_hweight.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitops/non-atomic.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/build-id.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/machine.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/maps.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/rwsem.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/dsos.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/list.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/poison.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/tool.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/map.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/symbol.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/path.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/symbol_conf.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/spark.h \
  /usr/include/elf.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/internal/lib.h \
  tests/tests.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/debug.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/machine.h \

tests/vmlinux-kallsyms.o: $(deps_tests/vmlinux-kallsyms.o)

$(deps_tests/vmlinux-kallsyms.o):
