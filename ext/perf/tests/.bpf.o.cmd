cmd_tests/bpf.o := gcc -Wp,-MD,tests/.bpf.o.d -Wp,-MT,tests/bpf.o -Wbad-function-cast -Wdeclaration-after-statement -Wformat-security -Wformat-y2k -Winit-self -Wmissing-declarations -Wmissing-prototypes -Wno-system-headers -Wold-style-definition -Wpacked -Wredundant-decls -Wstrict-prototypes -Wswitch-default -Wswitch-enum -Wundef -Wwrite-strings -Wformat -Wno-type-limits -Wstrict-aliasing=3 -Wshadow -DHAVE_SYSCALL_TABLE_SUPPORT -DHAVE_ARCH_X86_64_SUPPORT -Iarch/x86/include/generated -DHAVE_PERF_REGS_SUPPORT -DHAVE_ARCH_REGS_QUERY_REGISTER_OFFSET -Werror -O6 -fno-omit-frame-pointer -ggdb3 -funwind-tables -Wall -Wextra -std=gnu99 -fstack-protector-all -D_FORTIFY_SOURCE=2 -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D_GNU_SOURCE -I/home/exati/linux-hwe-5.15/tools/lib/perf/include -I/home/exati/linux-hwe-5.15/tools/perf/util/include -I/home/exati/linux-hwe-5.15/tools/perf/arch/x86/include -I/home/exati/linux-hwe-5.15/tools/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi -I/home/exati/linux-hwe-5.15/tools/include/uapi -I/home/exati/linux-hwe-5.15/tools/arch/x86/include/ -I/home/exati/linux-hwe-5.15/tools/arch/x86/ -I/home/exati/linux-hwe-5.15/tools/perf/util -I/home/exati/linux-hwe-5.15/tools/perf -I/home/exati/linux-hwe-5.15/tools/lib/ -DHAVE_PTHREAD_ATTR_SETAFFINITY_NP -DHAVE_PTHREAD_BARRIER -DHAVE_EVENTFD_SUPPORT -DHAVE_GET_CURRENT_DIR_NAME -DHAVE_GETTID -DHAVE_FILE_HANDLE -DHAVE_GLIBC_SUPPORT -DHAVE_AIO_SUPPORT -DHAVE_SCHED_GETCPU_SUPPORT -DHAVE_SETNS_SUPPORT -DNO_LIBPERL -DHAVE_TIMERFD_SUPPORT -DNO_LIBPYTHON -DNO_DEMANGLE -DHAVE_ZLIB_SUPPORT -DHAVE_BACKTRACE_SUPPORT -DHAVE_LIBNUMA_SUPPORT -DHAVE_KVM_STAT_SUPPORT -DHAVE_AUXTRACE_SUPPORT -D"BUILD_STR(s)=$(pound)s" -c -o tests/bpf.o tests/bpf.c

source_tests/bpf.o := tests/bpf.c

deps_tests/bpf.o := \
  /usr/include/stdc-predef.h \
  /usr/include/errno.h \
  /usr/include/features.h \
  /usr/include/features-time64.h \
  /usr/include/x86_64-linux-gnu/bits/wordsize.h \
  /usr/include/x86_64-linux-gnu/bits/timesize.h \
  /usr/include/x86_64-linux-gnu/sys/cdefs.h \
  /usr/include/x86_64-linux-gnu/bits/long-double.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs.h \
  /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
  /usr/include/x86_64-linux-gnu/bits/errno.h \
  /usr/include/linux/errno.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi/asm/errno.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/asm-generic/errno.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/asm-generic/errno-base.h \
  /usr/include/x86_64-linux-gnu/bits/types/error_t.h \
  /usr/include/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdarg.h \
  /usr/include/x86_64-linux-gnu/bits/types.h \
  /usr/include/x86_64-linux-gnu/bits/typesizes.h \
  /usr/include/x86_64-linux-gnu/bits/time64.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
  /usr/include/x86_64-linux-gnu/bits/types/cookie_io_functions_t.h \
  /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
  /usr/include/x86_64-linux-gnu/bits/floatn.h \
  /usr/include/x86_64-linux-gnu/bits/floatn-common.h \
  /usr/include/x86_64-linux-gnu/bits/stdio.h \
  /usr/include/x86_64-linux-gnu/bits/stdio2.h \
  /usr/include/stdlib.h \
  /usr/include/x86_64-linux-gnu/bits/waitflags.h \
  /usr/include/x86_64-linux-gnu/bits/waitstatus.h \
  /usr/include/x86_64-linux-gnu/bits/types/locale_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__locale_t.h \
  /usr/include/x86_64-linux-gnu/sys/types.h \
  /usr/include/x86_64-linux-gnu/bits/types/clock_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/clockid_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/time_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/timer_t.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-intn.h \
  /usr/include/endian.h \
  /usr/include/x86_64-linux-gnu/bits/endian.h \
  /usr/include/x86_64-linux-gnu/bits/endianness.h \
  /usr/include/x86_64-linux-gnu/bits/byteswap.h \
  /usr/include/x86_64-linux-gnu/bits/uintn-identity.h \
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
  /usr/include/x86_64-linux-gnu/sys/epoll.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h \
  /usr/include/stdint.h \
  /usr/include/x86_64-linux-gnu/bits/wchar.h \
  /usr/include/x86_64-linux-gnu/bits/stdint-uintn.h \
  /usr/include/x86_64-linux-gnu/bits/epoll.h \
  /usr/include/x86_64-linux-gnu/sys/stat.h \
  /usr/include/x86_64-linux-gnu/bits/stat.h \
  /usr/include/x86_64-linux-gnu/bits/struct_stat.h \
  /usr/include/x86_64-linux-gnu/bits/statx.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/linux/stat.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/types.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/stdbool.h \
  /usr/include/x86_64-linux-gnu/asm/types.h \
  /usr/include/asm-generic/types.h \
  /usr/include/asm-generic/int-ll64.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi/asm/bitsperlong.h \
  /home/exati/linux-hwe-5.15/tools/include/asm-generic/bitsperlong.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/asm-generic/bitsperlong.h \
  /usr/include/x86_64-linux-gnu/asm/posix_types.h \
  /usr/include/x86_64-linux-gnu/asm/posix_types_64.h \
  /usr/include/asm-generic/posix_types.h \
  /usr/include/x86_64-linux-gnu/bits/statx-generic.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_statx_timestamp.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_statx.h \
  /usr/include/fcntl.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl-linux.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_iovec.h \
  /usr/include/linux/falloc.h \
  /usr/include/x86_64-linux-gnu/bits/fcntl2.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/record.h \
  /usr/include/time.h \
  /usr/include/x86_64-linux-gnu/bits/time.h \
  /usr/include/x86_64-linux-gnu/bits/timex.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_tm.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_itimerspec.h \
  /usr/include/linux/stddef.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/linux/perf_event.h \
  /usr/include/linux/ioctl.h \
  /usr/include/x86_64-linux-gnu/asm/ioctl.h \
  /usr/include/asm-generic/ioctl.h \
  /usr/include/x86_64-linux-gnu/asm/byteorder.h \
  /usr/include/linux/byteorder/little_endian.h \
  /usr/include/linux/swab.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/include/asm/swab.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/target.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/util.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler_types.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/compiler-gcc.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/bpf-loader.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/err.h \
  /usr/include/string.h \
  /usr/include/strings.h \
  /usr/include/x86_64-linux-gnu/bits/strings_fortified.h \
  /usr/include/x86_64-linux-gnu/bits/string_fortified.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/debug.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/evlist.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/kernel.h \
  /usr/include/assert.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/build_bug.h \
  /usr/include/byteswap.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/refcount.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/../../arch/x86/include/asm/atomic.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/../../arch/x86/include/asm/rmwcc.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/asm/cmpxchg.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/list.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/poison.h \
  /home/exati/linux-hwe-5.15/tools/lib/api/fd/array.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/internal/evlist.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/internal/evsel.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/events_stats.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/perf/event.h \
  /usr/include/linux/limits.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/linux/bpf.h \
  /home/exati/linux-hwe-5.15/tools/include/uapi/linux/bpf_common.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/auxtrace.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/barrier.h \
  /home/exati/linux-hwe-5.15/tools/include/asm/../../arch/x86/include/asm/barrier.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/evsel.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/perf/evsel.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/perf/core.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/symbol_conf.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/internal/cpumap.h \
  /usr/include/pthread.h \
  /usr/include/sched.h \
  /usr/include/x86_64-linux-gnu/bits/sched.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_sched_param.h \
  /usr/include/x86_64-linux-gnu/bits/cpu-set.h \
  /usr/include/x86_64-linux-gnu/bits/setjmp.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct___jmp_buf_tag.h \
  /usr/include/x86_64-linux-gnu/bits/pthread_stack_min-dynamic.h \
  /usr/include/signal.h \
  /usr/include/x86_64-linux-gnu/bits/signum-generic.h \
  /usr/include/x86_64-linux-gnu/bits/signum-arch.h \
  /usr/include/x86_64-linux-gnu/bits/types/sig_atomic_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/siginfo_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/__sigval_t.h \
  /usr/include/x86_64-linux-gnu/bits/siginfo-arch.h \
  /usr/include/x86_64-linux-gnu/bits/siginfo-consts.h \
  /usr/include/x86_64-linux-gnu/bits/siginfo-consts-arch.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigval_t.h \
  /usr/include/x86_64-linux-gnu/bits/types/sigevent_t.h \
  /usr/include/x86_64-linux-gnu/bits/sigevent-consts.h \
  /usr/include/x86_64-linux-gnu/bits/sigaction.h \
  /usr/include/x86_64-linux-gnu/bits/sigcontext.h \
  /usr/include/x86_64-linux-gnu/bits/types/stack_t.h \
  /usr/include/x86_64-linux-gnu/sys/ucontext.h \
  /usr/include/x86_64-linux-gnu/bits/sigstack.h \
  /usr/include/x86_64-linux-gnu/bits/sigstksz.h \
  /usr/include/unistd.h \
  /usr/include/x86_64-linux-gnu/bits/posix_opt.h \
  /usr/include/x86_64-linux-gnu/bits/environments.h \
  /usr/include/x86_64-linux-gnu/bits/confname.h \
  /usr/include/x86_64-linux-gnu/bits/getopt_posix.h \
  /usr/include/x86_64-linux-gnu/bits/getopt_core.h \
  /usr/include/x86_64-linux-gnu/bits/unistd.h \
  /usr/include/x86_64-linux-gnu/bits/unistd_ext.h \
  /usr/include/linux/close_range.h \
  /usr/include/x86_64-linux-gnu/bits/ss_flags.h \
  /usr/include/x86_64-linux-gnu/bits/types/struct_sigstack.h \
  /usr/include/x86_64-linux-gnu/bits/sigthread.h \
  /usr/include/x86_64-linux-gnu/bits/signal_ext.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/filter.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/string.h \
  /home/exati/linux-hwe-5.15/tools/lib/api/fs/fs.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/perf/mmap.h \
  tests/tests.h \
  tests/llvm.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/debug.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/parse-events.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/mmap.h \
  /home/exati/linux-hwe-5.15/tools/lib/perf/include/internal/mmap.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/ring_buffer.h \
  /home/exati/linux-hwe-5.15/tools/include/linux/bitops.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/limits.h \
  /usr/lib/gcc/x86_64-linux-gnu/11/include/syslimits.h \
  /usr/include/limits.h \
  /usr/include/x86_64-linux-gnu/bits/posix1_lim.h \
  /usr/include/x86_64-linux-gnu/bits/local_lim.h \
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
  /usr/include/aio.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/event.h \
  /home/exati/linux-hwe-5.15/tools/perf/util/perf_regs.h \
  /home/exati/linux-hwe-5.15/tools/perf/arch/x86/include/perf_regs.h \
  /home/exati/linux-hwe-5.15/tools/arch/x86/include/uapi/asm/perf_regs.h \

tests/bpf.o: $(deps_tests/bpf.o)

$(deps_tests/bpf.o):
