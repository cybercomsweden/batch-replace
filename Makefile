SRC_CPP := $(shell find src/ -name '*.cpp')
SRC_HEADERS := $(shell find src/ -name '*.h')
TEST_CPP := $(shell find src/ tests/ -name '*.cpp' ! -wholename 'src/main.cpp')
TEST_HEADERS := $(shell find src/ tests/ -name '*.h')

compile: batchr

test: batchr_test
	./batchr_test

clean:
	rm -f batchr
	rm -f batchr_test

batchr: $(SRC_CPP) $(SRC_HEADERS)
	$(CXX) -std=c++11 -o batchr $(SRC_CPP) -lstdc++fs

batchr_test: $(TEST_CPP) $(TEST_HEADERS)
	$(CXX) -std=c++11 -o batchr_test -Ivendor/doctest -Isrc/ $(TEST_CPP) -lstdc++fs

.PHONY : compile test clean
