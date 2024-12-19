"""
Program: 2,4,1,2,7,5,1,3,4,4,5,5,0,3,3,0

Observations:

0. What do you notice about the program?

=> What about its control structure?
=> What operations are being performed?
=> are registers B and C actually relevant?

1. Using octal numbers, what is the smallest value of register A such that:

1a. produces the last digit of the program? => base 8 0

1b. that produces the two last digits of the program? => base 8 10

1c. that produces the three last digits of the program? => base 8 103

1d. that produce the four last digits of the program? => base 8 1035

1e. that produce the last 6 digits of the program?
=> significantly harder to find, because it requires backtracking!

1.5 What is the relationship between the input digits and the output digits?
=> many to one
=> notice that a few different digits can produce 0 as the first output!

2. What do you notice about the pattern of the numbers?

=> Each new digit produces an extra number.
=> Different values of the digit produce different numbers.
"""

correct_output = [2, 4, 1, 2, 7, 5, 1, 3, 4, 4, 5, 5, 0, 3, 3, 0]


def prog(a):
    
    out = []
    b = 0
    c = 0

    while (a != 0):
        b = (a % 8) ^ 2
        c = a >> b
        b = b ^ 3 ^ c
        out.append(b % 8)
        a = a >> 3

    return out

a = 0o30351700
b = 0
c = 0
out = []

print(f"a : {oct(a)}, b: {oct(b)}, c: {oct(c)}")
print(f"=> {prog(a)}")


# iterative does not work, there are multiple numbers that can produce the same digit
# solution: use recursion
def sol_wrong_iter(prog, expected_output):
    curr_a = 0
    digits_found = 0
    expected_output.reverse()
    
    out = []
    while (digits_found < len(expected_output)):
        for i in range(8):
            out = prog(curr_a * 8 + i)
            print(out)
            if not out:
                continue
            if (out[0] == expected_output[digits_found]):
                digits_found += 1
                curr_a = curr_a * 8 + i
                break

    return curr_a

# try recursive solution
def sol(curr_a, prog, digits_found, expected):
    
    if (digits_found == len(expected)):
        return curr_a
    
    for i in range(8):
        out = prog(curr_a * 8 + i)
        if not out:
            continue
        if (out[0] == expected[digits_found]):
            result = sol(curr_a * 8 + i, prog, digits_found + 1, expected)
            if result is not None:
                return result
    
    return None

correct_output.reverse()
print(sol(0, prog, 0, correct_output))
