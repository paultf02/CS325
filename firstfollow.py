lhslist = [] # list of nonterminals in order of production
terminals = []
with open("transformedgrammar3.txt", "r") as fin:
    lines = fin.readlines()
rhslist = []
for l in lines:
    a = l.rstrip().split(" -> ")
    production, fullrhs = a
    rhs = [term.split(" ") for term in fullrhs.split(" | ")]
    # rhslist = [list of rhs]
    # rhs = [list of or sections]
    # or_sections = [list of words]
    lhslist.append(production)
    rhslist.append(rhs)
#print(nonterminals)


# for i in range(len(rhslist)):
#     print(f"{lhslist[i]} is the production, rhs is a {type(rhslist[i])} it has {len(rhslist[i])} options")

print(rhslist[29][1])