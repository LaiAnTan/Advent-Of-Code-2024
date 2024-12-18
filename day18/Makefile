DAY = 18

COMPILER = g++
FLAGS = -Wall -Wextra -Werror

DIR_NAME = day$(DAY)

INPUT = day$(DAY)_input.txt

PART1_SRCS = day$(DAY)_part1.cpp
PART2_SRCS = day$(DAY)_part2.cpp

part1:
	$(COMPILER) $(FLAGS) $(PART1_SRCS) -o $(DIR_NAME)part1
	./$(DIR_NAME)part1 $(INPUT)

part2:
	$(COMPILER) $(FLAGS) $(PART2_SRCS) -o $(DIR_NAME)part2
	./$(DIR_NAME)part2 $(INPUT)

clean:
	rm -f $(DIR_NAME)part2 $(DIR_NAME)part1

.PHONY: all clean part1 part2