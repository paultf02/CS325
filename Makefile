CXX=clang++ -std=c++17
CFLAGS= -g -O3 `llvm-config --cppflags --ldflags --system-libs --libs all` \
-Wno-unused-function -Wno-unknown-warning-option -fno-rtti

mccomp: mccomp.cpp
	$(CXX) mccomp.cpp parser.cpp loaddata.cpp lexer.cpp $(CFLAGS) -o mccomp

clean:
	rm -rf mccomp