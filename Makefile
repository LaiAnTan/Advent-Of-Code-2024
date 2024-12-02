
clean:
	rm d1p1 d1p2

d1p1:
	g++ -Wall -Wextra -Werror day01/day01_part1.cpp -o d1p1
	./d1p1 day01/day01_input.txt

d1p2:
	g++ -Wall -Wextra -Werror day01/day01_part2.cpp -o d1p2
	./d1p2 day01/day01_input.txt

.PHONY: d1p1 d1p2