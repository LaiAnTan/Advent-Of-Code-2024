import sys

def checkFullAdderStructure(x, y, cin, sout, gates):
    """
    Check the structure of a full adder and identify any swapped outputs.

    :param x: Input x
    :param y: Input y
    :param cin: Carry-in input
    :param sout: Sum output
    :param gates: List of gates in the form of (inputs, operation, output)
    :return: The correct Carry-out output, and the two swapped outputs
    """

    # assume that the parameters are guaranteed to be correct
    # try to deduce 2 wrong outputs and swap them back.

    # after the execution of this function, we can gurantee that
    # the next call will be with correct parameters as we have found the
    # two wrong parameters, and determined the correct carry out

    # this function will not work if the outputs swapped with other full adder outputs,
    # because it is testing full adders individually.

    # at this point, we do not know if a or b is wrong yet
    a = next(g[2] for g in gates if (x in g[0] and y in g[0] and g[1] == "XOR"))
    b = next(g[2] for g in gates if (x in g[0] and y in g[0] and g[1] == "AND"))
    c = next((g[2] for g in gates if (cin in g[0] and a in g[0] and g[1] == "AND")), None)

    # if a is wrong, we will not find a rule where [a AND cin -> c]
    if (c is None):

        # we can look for the correct a by looking for rule [cin XOR A -> sout]
        operands = next(g for g in gates if (
            cin in g[0] and g[1] == "XOR" and g[2] == sout))[0]
        correct_a = next(elem for elem in operands if elem != cin)

        if (correct_a == b or correct_a == c):
            cout = next((g[2] for g in gates if (a in g[0] and g[1] == "OR")), None)
        else:  # both b and c are right, a swapped with cout / sout
            cout = next((g[2] for g in gates if (b in g[0] and c in g[0] and g[1] == "OR")), None)

        print(f"wrong a: {a} | correct a: {correct_a} | cout : {cout}")

        return [cout, (a, correct_a)]

    # at this point we know a is correct, by assumption of cin and existance of c
    # at this point, we do not know if C is correct.

    # we can check sout (sum) since we know a and cin are both correct.
    found_sout = next((g[2] for g in gates if (cin in g[0] and a in g[0] and g[1] == "XOR")), None)
    # after this, we know if sout is correct or wrong.

    # if sout is not same then we know that sout swapped with found_sout.
    if (sout != found_sout):

        if (b == sout):  # sout swapped with b
            cout = next((g[2] for g in gates if (c in g[0] and g[1] == "OR")), None)
        elif (c == sout):  # sout swapped with c
            cout = next((g[2] for g in gates if (b in g[0] and g[1] == "OR")), None)
        else:  # sout swapped with cout
            cout = found_sout

        print(
            f"wrong sum: {found_sout} | correct sum: {sout} | cout : {cout}")

        return [cout, (sout, found_sout)]

    cout = next((g[2] for g in gates if (
        b in g[0] and c in g[0] and g[1] == "OR")), None)

    # if we reach here, means the full adder is all correct
    return [cout]


def findFirstCarryIn(gates):

    return next(g[2] for g in gates if ("x00" in g[0] and "y00" in g[0] and g[1] == "AND"))


def findSwapped(variables, gates):

    xs = [n for n in variables.keys() if n[0] == 'x']
    ys = [n for n in variables.keys() if n[0] == 'y']
    souts = ["z" + s.split("x")[1] for s in xs]

    swapped = set()

    cin = findFirstCarryIn(gates)

    full_adders = list(zip(xs, ys, souts))[1:]

    for fa in full_adders:

        x = fa[0]
        y = fa[1]
        sout = fa[2]

        print(f"Checking: {x} {y} {cin} {sout}")
        # rationale of assuming that swaps only happen within full adders:
        # if swaps happened between two full adders, the whole thing would inf loop / not work (not sure if this is 100% correct)
        res = checkFullAdderStructure(x, y, cin, sout, gates)
        cin = res[0]
        if len(res) == 2:
            swapped.update(set(res[1]))


    return (swapped)


def main():

    filename = sys.argv[1]

    with open(filename, 'r') as file:
        lines = file.readlines()

    vs = {}
    gs = []

    i = 0
    while (line := lines[i].strip()) != "":

        splitted = line.split(": ")
        vs[splitted[0]] = int(splitted[1])
        i += 1

    for line in lines[i + 1:]:

        parts = line.strip().split()
        gs.append(([parts[0], parts[2]], parts[1], parts[4]))

    print(",".join(sorted(list(findSwapped(vs, gs)))))


if __name__ == "__main__":
    main()
