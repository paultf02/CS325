CXX=clang++ -std=c++17
CFLAGS= -g -O3 `llvm-config --cppflags --ldflags --system-libs --libs all` \
-Wno-unused-function -Wno-unknown-warning-option -fno-rtti

mccomp: mccomp.cpp
	$(CXX) mccomp.cpp $(CFLAGS) -o mccomp

clean:
	rm -rf mccomp

firstfollow: firstfollow.cpp
	$(CXX) firstfollow.cpp $(CFLAGS) -o firstfollow

