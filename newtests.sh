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

# ### Build mccomp compiler
# echo "Cleanup *****"
# rm -rf ./mccomp

# echo "Compile *****"

# make clean
# make -j mccomp

COMP=$DIR/mccomp
echo $COMP

function validate {
  $1 > perf_out
  echo
  echo $1
  grep "Result" perf_out;
#   grep "Result" perf_out; grep "PASSED" perf_out;
#   grep "Result" perf_out; grep "PASSED" perf_out; grep "Failed" perf_out;
#   grep "PASSED" perf_out; grep "Failed" perf_out;
#   rc=$?; if [[ $rc != 0 ]]; then echo "TEST FAILED *****";exit $rc; fi;rm perf_out
#  rc=$?; if [[ $rc != 0 ]]; then echo "TEST FAILED *****"; $rc; fi;rm perf_out
}

echo "Test *****"

addition=1
factorial=1
fibonacci=1
pi=1
while=1
void=1
cosine=0
unary=1
palindrome=1
recurse=1
rfact=1

# cd tests/addition

if [ $addition == 1 ]
then	
	# cd ../addition/
	# pwd
	rm -rf output.ll add
	"$COMP" ./tests/addition/addition.c
	$CLANG ./tests/addition/driver.cpp output.ll  -o add
	validate "./add"
fi


if [ $factorial == 1 ];
then	
	# cd ../factorial
	# pwd
	rm -rf output.ll fact
	"$COMP" ./tests/factorial/factorial.c
	$CLANG tests/factorial/driver.cpp output.ll -o fact
	validate "./fact"
fi

if [ $fibonacci == 1 ];
then	
	# cd ../fibonacci
	# pwd
	rm -rf output.ll fib
	"$COMP" ./tests/fibonacci/fibonacci.c
	$CLANG ./tests/fibonacci/driver.cpp output.ll -o fib
	validate "./fib"
fi

if [ $pi == 1 ];
then	
	# cd ../pi
	# pwd
	rm -rf output.ll pi
	"$COMP" ./tests/pi/pi.c
	$CLANG ./tests/pi/driver.cpp output.ll -o pi
	validate "./pi"
fi

if [ $while == 1 ];
then	
	# cd ../while
	# pwd
	rm -rf output.ll while
	"$COMP" ./tests/while/while.c
	$CLANG ./tests/while/driver.cpp output.ll -o while
	validate "./while"
fi

if [ $void == 1 ];
then	
	# cd ../void
	# pwd
	rm -rf output.ll void
	"$COMP" ./tests/void/void.c 
	$CLANG ./tests/void/driver.cpp output.ll -o void
	validate "./void"
fi

if [ $cosine == 1 ];
then	
	# cd ../cosine
	# pwd
	rm -rf output.ll cosine
	"$COMP" ./tests/cosine/cosine.c
	$CLANG ./tests/cosine/driver.cpp output.ll -o cosine
	validate "./cosine"
fi

if [ $unary == 1 ];
then	
	# cd ../unary
	# pwd
	rm -rf output.ll unary
	"$COMP" ./tests/unary/unary.c
	$CLANG ./tests/unary/driver.cpp output.ll -o unary
	validate "./unary"
fi

if [ $recurse == 1 ];
then	
	# cd ../recurse
	# pwd
	rm -rf output.ll recurse
	"$COMP" ./tests/recurse/recurse.c
	$CLANG ./tests/recurse/driver.cpp output.ll -o recurse
	validate "./recurse"
fi

if [ $rfact == 1 ];
then	
	# cd ../rfact
	# pwd
	rm -rf output.ll rfact
	"$COMP" ./tests/rfact/rfact.c
	$CLANG ./tests/rfact/driver.cpp output.ll -o rfact
	validate "./rfact"
fi

if [ $palindrome == 1 ];
then	
	# cd ../palindrome
	# pwd
	rm -rf output.ll palindrome
	"$COMP" ./tests/palindrome/palindrome.c
	$CLANG ./tests/palindrome/driver.cpp output.ll -o palindrome
	validate "./palindrome"
fi

echo "***** ALL TESTS PASSED *****"
