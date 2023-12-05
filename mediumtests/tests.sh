#!/bin/bash
set -e
#export LLVM_INSTALL_PATH=/home/gihan/LLVM/install-10.0.1
#export LLVM_INSTALL_PATH=/modules/cs325/llvm-8/
#export LLVM_INSTALL_PATH=/modules/cs325/llvm-10.0.1
#export LLVM_INSTALL_PATH=/modules/cs325/llvm-12.0.1
#export LLVM_INSTALL_PATH=/tmp/LLVM/llvm-14.0.6
#export LLVM_INSTALL_PATH=/modules/cs325/llvm-15.0.0
#export LLVM_INSTALL_PATH=/modules/cs325/llvm-16.0.0
export LLVM_INSTALL_PATH=/modules/cs325/llvm-17.0.1
export PATH=$LLVM_INSTALL_PATH/bin:$PATH
export LD_LIBRARY_PATH=$LLVM_INSTALL_PATH/lib:$LD_LIBRARY_PATH
CLANG=$LLVM_INSTALL_PATH/bin/clang++

module load GCC/12.2.0

DIR="$(pwd)"

### Build mccomp compiler
echo "Cleanup *****"
rm -rf ./mccomp

echo "Compile *****"

make clean
make -j mccomp

COMP=$DIR/mccomp
echo $COMP

function validate {
  $1 > perf_out
  echo
  echo $1
  grep "Result" perf_out;grep "PASSED" perf_out
  rc=$?; if [[ $rc != 0 ]]; then echo "TEST FAILED *****";exit $rc; fi;rm perf_out
#  rc=$?; if [[ $rc != 0 ]]; then echo "TEST FAILED *****"; $rc; fi;rm perf_out
}

echo "Test *****"

cd mediumtests/example_scope/
pwd
rm -rf output.ll example_scope
"$COMP" ./example_scope.c
$CLANG driver.cpp output.ll -o example_scope
validate "./example_scope"
rm ./example_scope

# cd ../long/
# pwd
# rm -rf output.ll long
# "$COMP" ./long.c
# $CLANG driver.cpp output.ll -o long
# validate "./long"
# rm ./long

cd ../truthiness/
pwd
rm -rf output.ll truthiness
"$COMP" ./truthiness.c
$CLANG driver.cpp output.ll -o truthiness
validate "./truthiness"
rm ./truthiness

# cd ../widening/
# pwd
# rm -rf output.ll widening
# "$COMP" ./widening.c
# $CLANG driver.cpp output.ll -o widening
# validate "./widening"
# rm ./widening



echo "==================================="
echo "=== Running negative test cases ==="
echo "==================================="


cd ../fail
pwd

echo "./call_undefined.c"
if "$COMP" ./call_undefined.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'call_undefined.c'! for example clang:"
    clang ./call_undefined.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./call_undefined.c || true
fi

echo "./double_func_mismatched.c"
if "$COMP" ./double_func_mismatched.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'double_func_mismatched.c'! for example clang:"
    clang ./double_func_mismatched.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./double_func_mismatched.c || true
fi

echo "./double_func.c"
if "$COMP" ./double_func.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'double_func.c'! for example clang:"
    clang ./double_func.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./double_func.c || true
fi

echo "./double_global.c"
if "$COMP" ./double_global.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'double_global.c'!"
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error!"
fi

echo "./double_local.c"
if "$COMP" ./double_local.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'double_local'! for example clang:"
    clang ./double_local.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./double_local.c || true
fi

echo "./extern_fun_decl_mismatch.c"
if "$COMP" ./extern_fun_decl_mismatch.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'extern_fun_decl_mismatch.c'! for example clang:"
    clang ./extern_fun_decl_mismatch.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./extern_fun_decl_mismatch.c || true
fi

echo "./no_init_assign.c"
if "$COMP" ./no_init_assign.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'no_init_assign.c'! for example clang:"
    clang ./no_init_assign.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./no_init_assign.c || true
fi

echo "./scope_bleed.c"
if "$COMP" ./scope_bleed.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'scope_bleed.c'! for example clang:"
    clang ./scope_bleed.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./scope_bleed.c || true
fi

echo "./undefined_var.c"
if "$COMP" ./undefined_var.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'undefined_var.c'! for example clang:"
    clang ./undefined_var.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./undefined_var.c || true
fi

echo "./unexpected_ret.c"
if "$COMP" ./unexpected_ret.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'unexpected_ret.c'! for example clang:"
    clang ./unexpected_ret.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./unexpected_ret.c || true
fi

echo "./wrong_num_args.c"
if "$COMP" ./wrong_num_args.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'wrong_num_args.c'! for example clang:"
    clang ./wrong_num_args.c
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error! Compare with clang:"
    clang ./wrong_num_args.c || true
fi

echo "./wrong_type_args.c"
if "$COMP" ./wrong_type_args.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'wrong_type_args.c'!"
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error!"
fi

echo "./wrong_type_ret.c"
if "$COMP" ./wrong_type_ret.c; then
    echo "TEST FAILED *****"
    echo "Expected semantic error in 'wrong_type_ret.c'!"
    exit 1;
else
    echo "TEST PASSED *****"
    echo "Got semantic error!"
fi



rm -f a.out output.ll
echo "***** ALL (MEDIUM) TESTS PASSED *****"
