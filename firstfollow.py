# we have 3 categories of term
# nonterminal, terminal, epsilon

with open("terminals.txt", "r") as fin:
    terminals = [line.rstrip() for line in fin.readlines()]
with open("transformedgrammar3.txt", "r") as fin:
    lines = fin.readlines()

lhslist = [] # list of nonterminals in order of production
rhslist = []
index = {}
for i in range(len(lines)):
    a = lines[i].rstrip().split(" -> ")
    production, fullrhs = a
    rhs = [term.split(" ") for term in fullrhs.split(" | ")]
    # rhslist = [list of rhs]
    # rhs = [list of or sections]
    # or_sections = [list of words]
    lhslist.append(production)
    rhslist.append(rhs)
    index[production] = i
#print(nonterminals)

# for i in range(len(rhslist)):
#     print(f"{lhslist[i]} is the production, rhs is a {type(rhslist[i])} it has {len(rhslist[i])} options")

#print(rhslist[29][1])

nullable = {term: None for term in lhslist}
nullable["epsilon"] = True
for t in terminals:
    nullable[t] = False
#print(nullable)


def findnullable(term):
    if nullable[term] != None:
        return nullable[term]
    else:
        #print(term)
        i = index[term]
        rhs = rhslist[i]
        term_nullable = False
        for or_statement in rhs:
            or_statement_nullable = True
            for atom in or_statement:
                or_statement_nullable = or_statement_nullable & findnullable(atom)
                if or_statement_nullable == False:
                    break
            term_nullable = term_nullable | or_statement_nullable
        nullable[term] = term_nullable
        return nullable[term]
#print(nullable['","'])
for term in nullable.keys():
    if nullable[term] == None:
        findnullable(term)

#print(nullable)

print("The nullable elements are:")
[print(production) for production in nullable.keys() if nullable[production]]
print("The non nullable elements are:")
[print(production) for production in nullable.keys() if nullable[production] == False]
print("The elements whose nullability has not been calculated are:")
[print(production) for production in nullable.keys() if nullable[production] == None]

first = {term: None for term in lhslist}
first["epsilon"] = set()
for t in terminals:
    first[t] = set([t])

def findfirst(term):
    if first[term] != None:
        return first[term]
    else:
        #print(term)
        i = index[term]
        rhs = rhslist[i]
        term_nullable = False
        for or_statement in rhs:
            or_statement_nullable = True
            for atom in or_statement:
                or_statement_nullable = or_statement_nullable & findnullable(atom)
                if or_statement_nullable == False:
                    break
            term_nullable = term_nullable | or_statement_nullable
        
        first[term] = term_nullable
        return first[term]
#print(nullable['","'])
for term in nullable.keys():
    if nullable[term] == None:
        findnullable(term)




            
