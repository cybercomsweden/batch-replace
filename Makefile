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
	g++ -o batchr -std=c++11 $(SRC_CPP)

batchr_test: $(TEST_CPP) $(TEST_HEADERS)
	g++ -Ivendor/doctest -Isrc/ -o batchr_test $(TEST_CPP)

.PHONY : compile test clean
