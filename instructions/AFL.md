# AFL AOSP setup

Instructions for compiling and fuzzing CAN AOSP harnesses with libFuzzer


## Android.bp setup

For using AFL++, the following change is necessary in the Android.bp file of the harness:
```
ldflags: [
    "-Wl,--allow-multiple-definition",
    "-Wl,--exclude-libs=libclang_rt.fuzzer-x86_64-android.a",
],
```
## Compilation

The following steps are for compiling harness:

```
export SANITIZE_TARGET="address"
export FUZZ_FRAMEWORK="afl"
cd /mnt/sdb1/aosp
source build/envsetup.sh
lunch sdk_car_portrait_x86_64-userdebug
cd $ANDROID_BUILD_TOP/hardware/intefaces/automotive/can/
mm -j30
adb sync
```



## Running the fuzzer

The following steps are for running the fuzzer on the device

### set ASAN_OPTIONS and AFL env var
Compared to libFuzzer, AFL requires `abort_on_error=1` if custom `ASAN_OPTIONS` are used.

```
export ASAN_OPTIONS="halt_on_error=0:abort_on_error=1:detect_container_overflow=0:coverage=1:coverage_counters=1:verbosity=1"
export AFL_MAP_SIZE=10000000 
export AFL_DEBUG=1
export AFL_SKIP_CPUFREQ=1 
export AFL_EXPLORE_CRASHES=1
```

### Create initial corpus

```
cd /data/fuzz/...
mkdir inputs
echo -n A > inputs/A
```

### Run
```
LD_LIBRARY_PATH=/vendor/lib64/ afl-fuzz -D -V 86400 -t 5000 -i inputs -o output_default ./automotiveCanV1.0_fuzzer
```

### Merge corpora
```
LD_LIBRARY_PATH=/vendor/lib64/ afl-tmin -i inputs -o merged_corpora ./automotiveCanV1.0_fuzzer
```

### Coverage data generation
```
LD_LIBRARY_PATH=/vendor/lib64/ afl-showmap -i output_default/default/queue -t 5000 -o coverage_info -r -C ./automotiveCanV1.0_fuzzer
```
## Documentation
https://github.com/llvm-mirror/compiler-rt/blob/master/lib/fuzzer/afl/afl_driver.cpp

https://github.com/ele7enxxh/android-afl

https://github.com/google/AFL/issues/61

https://google.github.io/clusterfuzz/setting-up-fuzzing/libfuzzer-and-afl/

