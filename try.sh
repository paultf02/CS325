export LLVM_INSTALL_PATH=/modules/cs325/llvm-17.0.1
export PATH=$LLVM_INSTALL_PATH/bin:$PATH
export LD_LIBRARY_PATH=$LLVM_INSTALL_PATH/lib:$LD_LIBRARY_PATH
CLANG=$LLVM_INSTALL_PATH/bin/clang++

module load GCC/12.2.0

DIR="$(pwd)"

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

cd ./tests/cosine
cd ../cosine
pwd
rm -rf output.ll cosine
"$COMP" ./cosine.c
$CLANG driver.cpp output.ll -o cosine
validate "./cosine"

# ./mccomp ./t.c
# clang output.ll -o exec
# ./exec
# echo $?

# ./mccomp ./tests/addition/addition.c
# clang++ ./tests/addition/driver.cpp output.ll -o exec
# ./exec
# echo $?