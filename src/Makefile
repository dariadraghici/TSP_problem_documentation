CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99
TARGET_EXACT = TSP_exact
TARGET_APPROX = TSP_approx
SOURCE_EXACT = TSP_exact.c
SOURCE_APPROX = TSP_approx.c

all: $(TARGET_EXACT) $(TARGET_APPROX)

$(TARGET_EXACT): $(SOURCE_EXACT)
	$(CC) $(CFLAGS) -o $(TARGET_EXACT) $(SOURCE_EXACT) -lm

$(TARGET_APPROX): $(SOURCE_APPROX)
	$(CC) $(CFLAGS) -o $(TARGET_APPROX) $(SOURCE_APPROX) -lm

generate-tests:
	cd ../tests && python3 generate_tests.py

clean-tests:
	rm -rf ../tests/input/* ../tests/output_exact/* ../tests/output_approx/*

clean:
	rm -f $(TARGET_EXACT) $(TARGET_APPROX)
	rm -f ../tests/output_exact/* ../tests/output_approx/*

test: all
	cd ../tests && ./run_tests.sh

.PHONY: all clean clean-tests generate-tests test