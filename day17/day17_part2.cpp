#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <sstream>
#include <cmath>

enum Register {
    a,
    b,
    c
};

template <typename S>
std::ostream &operator << (std::ostream &os, const std::vector<S> &vector)
{
	os << "[";
	for (auto i = vector.begin(); i != vector.end() - 1; i++)
		os << *i << ", ";
	os << *(vector.end() - 1) << "]";
	return os;
}

template <typename T, typename U>
std::ostream &operator << (std::ostream &os, const std::map<T, U> &map)
{
	for (auto i : map)
		os << i.first << ": " << i.second << "\n";
	return os;
}

// void adv(Computer *comp);
// void bxl(Computer *comp);
// void bst(Computer *comp);
// void jnz(Computer *comp);
// void bxc(Computer *comp);
// void out(Computer *comp);
// void bdv(Computer *comp);
// void cdv(Computer *comp);

class Computer {

    public:
        int halt;
        int instruction_ptr;
        std::map<Register, int> registers;
        std::vector<int>    program;
        std::vector<int>    output;
        std::map<int, std::function<void(Computer *c)>> instructions;

        Computer(int a_initial, int b_initial, int c_initial, std::vector<int> program)
        {
            this->registers[a] = a_initial;
            this->registers[b] = b_initial;
            this->registers[c] = c_initial;
            this->program = program;
            this->instruction_ptr = 0;
            this->halt = false;
            this->output = {};
        }

        void insertInstruction(int opcode, std::function<void(Computer *c)> instruction)
        {
            this->instructions[opcode] = instruction;
        }

        int getComboValue(int operand)
        {
            if (operand >= 0 && operand <= 3)
                return (operand);
            if (operand == 4)
                return (this->registers[a]);
            if (operand == 5)
                return (this->registers[b]);
            if (operand == 6)
                return (this->registers[c]);

            throw std::invalid_argument("Invalid operand");
        }

        void runProgram()
        {
            while (!this->halt)
            {
                // this->printStatus();
                int curr_opcode = this->program[this->instruction_ptr];
                // std::cout << "Calling instruction " << curr_opcode << std::endl;
                this->instructions[curr_opcode](this);
                if (this->instruction_ptr >= (int) this->program.size())
                    this->halt = true;
            }
        }

        void printStatus()
        {
            std::cout << "Registers:\n" << this->registers << std::endl;
            std::cout << "Halted: " << this->halt << std::endl;
            if (this->halt == false)
                std::cout << "Instruction ptr at pos " << this->instruction_ptr << " with value " << this->program[this->instruction_ptr] << std::endl;
            std::cout << "Program: " << this->program << std::endl;
            if (this->output.empty() == false)
                std::cout << "Output: " << this->output << std::endl;
        }
};

void adv(Computer *comp)
{
    int numerator = comp->registers[a];
    // combo operand
    int denominator = std::pow(2, comp->getComboValue(comp->program[comp->instruction_ptr + 1]));
    comp->registers[a] = numerator / denominator;
    comp->instruction_ptr += 2;
}

void bxl(Computer *comp)
{
    comp->registers[b] = comp->registers[b] ^ comp->program[comp->instruction_ptr + 1];
    comp->instruction_ptr += 2;
}

void bst(Computer *comp)
{
    // literal operand
    comp->registers[b] = comp->getComboValue(comp->program[comp->instruction_ptr + 1]) % 8;
    comp->instruction_ptr += 2;
}

void jnz(Computer *comp)
{
    if (comp->registers[a] != 0)
        comp->instruction_ptr = comp->program[comp->instruction_ptr + 1]; // literal
    else
        comp->instruction_ptr += 2;
}

void bxc(Computer *comp)
{
    comp->registers[b] = comp->registers[b] ^ comp->registers[c];
    comp->instruction_ptr += 2;
}

void out(Computer *comp)
{
    comp->output.push_back(comp->getComboValue(comp->program[comp->instruction_ptr + 1]) % 8);
    comp->instruction_ptr += 2;
}

void bdv(Computer *comp)
{
    int numerator = comp->registers[a];
    // combo operand
    int denominator = std::pow(2, comp->getComboValue(comp->program[comp->instruction_ptr + 1]));
    comp->registers[b] = numerator / denominator;
    comp->instruction_ptr += 2;
}

void cdv(Computer *comp)
{
    int numerator = comp->registers[a];
    // combo operand
    int denominator = std::pow(2, comp->getComboValue(comp->program[comp->instruction_ptr + 1]));
    comp->registers[c] = numerator / denominator;
    comp->instruction_ptr += 2;
}

int main(int argc, char **argv)
{
	if (argc != 2)
			return (-1);
		
	std::string		filename(argv[1]);
	std::fstream	data(filename, std::fstream::in);
	std::string		line;

	if (!data.is_open())
		return (-1);

    std::vector<int>    prog;

    std::getline(data, line);
    std::getline(data, line);
    std::getline(data, line);
    std::getline(data, line);
    std::getline(data, line);

    std::stringstream ss(line.substr(line.find(":") + 2));
    std::string num;
    while (std::getline(ss, num, ','))
        prog.push_back(std::stoi(num));

    int a_value = 0;
    Computer comp(a_value, 0, 0, prog);
    comp.instructions = {
        {0, adv},
        {1, bxl},
        {2, bst},
        {3, jnz},
        {4, bxc},
        {5, out},
        {6, bdv},
        {7, cdv}
    };
    while (true)
    {
        comp.output = {};
        comp.instruction_ptr = 0;
        comp.halt = 0;
        comp.registers[a] = a_value;
        comp.registers[b] = 0;
        comp.registers[c] = 0;
        std::cout << "Running with register a = " << a_value << std::endl;
        comp.runProgram();
        // std::cout << comp.program << std::endl;
        // std::cout << comp.output << std::endl;
        if (comp.program == comp.output)
            break;
        a_value++;
    }


    std::cout << a_value << std::endl;
}

/*
plan:


recursion ??

base cases:

if same all, return a
if first difference, return

reordered program:


init:
a = ?
b = 0
c = 0

1. b = (a mod 8) ^ 2
2. c = a >> b
3. b = b ^ 3 ^ c
4. print (b mod 8)
5. a = a >> 3
6. if a != 0 go back to step 1

output:
2,4,1,2,7,5,1,3,4,4,5,5,0,3,3,0

Observations:

- step 5 is independent of the program.
- a >> 3 means a / 8
- number of iterations for a to == 0 is ceil(log8(a))
- since output is 16 long, 16 = ceil(log8(a))
- solving for a: 

16 <= log8(a) <= 17
16 <= log2(a) / 3 <= 17
48 <= log2(a) <= 51
2^48 <= a <= 2^51

- Conclusion: 1,970,324,837,974,592 possible values of a that result in an appropriate output size
- i.e. brute force is unviable
*/