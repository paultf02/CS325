./mccomp ./t.c
clang output.ll -o exec
./exec
echo $?

# ./mccomp ./tests/addition/addition.c
# clang++ ./tests/addition/driver.cpp output.ll -o exec
# ./exec
# echo $?