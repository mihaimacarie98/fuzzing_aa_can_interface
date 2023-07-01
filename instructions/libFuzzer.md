# libFuzzer AOSP setup

Instructions for compiling and fuzzing CAN AOSP harnesses with libFuzzer


## Compilation

The following steps are for compiling harness:

```
export SANITIZE_TARGET="address"
export FUZZ_FRAMEWORK=""
cd /mnt/sdb1/aosp
source build/envsetup.sh
lunch sdk_car_portrait_x86_64-userdebug
cd $ANDROID_BUILD_TOP/hardware/intefaces/automotive/can/
mm -j30
adb sync
```



## Running the fuzzer

The following steps are for running the fuzzer on the device

### set ASAN_OPTIONS

```
export ASAN_OPTIONS="halt_on_error=0:abort_on_error=0:detect_container_overflow=0:coverage=1:coverage_counters=1:verbosity=1"
```

### Create initial corpus

```
cd /data/fuzz/...
mkdir inputs
echo -n A > inputs/A
```

### Run
```
LD_LIBRARY_PATH=/vendor/lib64/ ./automotiveCanV1.0_fuzzer -max_total_time=86400 -detect_leaks=0 -print_pcs=1 -print_final_stats=1 -fork=1 -ignore_crashes=1 -print_coverage=1 -print_full_coverage=1 inputs
```

### Merge corpora
```
mkdir corpora_minimized
./fuzzer -merge=1 ./corpora_minimized ./inputs
./fuzzer -runs=0 ./corpora_minimized
./fuzzer -runs=0 -print_pcs=1 -print_final_stats=1 -print_coverage=1 -print_full_coverage=1 ./corpora_minimized
```

## Car manufacturer notes
Root emulator, copy fuzzer and apex libraries, push to /sdcard, move to /data/fuzz, chmod everything
```
LD_LIBRARY_PATH=/data/fuzz/x86_64/lib64 ./automotiveCanV1.0_fuzzer
```

## Documentation
https://llvm.org/docs/LibFuzzer.html

https://github.com/theopolis/simple-libfuzzer/blob/master/simple/simple.cpp

https://llvm.org/docs/LibFuzzer.html#id36

https://github.com/google/fuzzing/blob/HEAD/docs/split-inputs.md#fuzzed-data-provider

https://clang.llvm.org/docs/LeakSanitizer.html

https://stackoverflow.com/questions/47021422/
how-to-enable-address-sanitizer-for-multiple-c-binaries

https://android.googlesource.com/toolchain/llvm_android/+/refs/heads/llvm-r353983/AddressSanitizer.md

https://github.com/google/fuzzing/blob/master/tutorial/libFuzzerTutorial.md

https://source.android.com/docs/security/test/libfuzzer

