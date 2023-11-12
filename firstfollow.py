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

is_terminal = {}
for nonterminal in lhslist:
    is_terminal[nonterminal] = False
for terminal in terminals:
    is_terminal[terminal] = True
#print(nonterminals)

# for i in range(len(rhslist)):
#     print(f"{lhslist[i]} is the production, rhs is a {type(rhslist[i])} it has {len(rhslist[i])} options")

#print(rhslist[29][1])

##################NULLABLE CODE

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

print("started computing which elements are nullable")
for term in nullable.keys():
    if nullable[term] == None:
        findnullable(term)
print("finished computing which elements are nullable")
    

# print("The nullable elements are:")
# [print(production) for production in nullable.keys() if nullable[production]]
# print("The non nullable elements are:")
# [print(production) for production in nullable.keys() if nullable[production] == False]
# print("The elements whose nullability has not been calculated are:")
# [print(production) for production in nullable.keys() if nullable[production] == None]

##################FIRST SET CODE

first = {term: None for term in lhslist}
first["epsilon"] = set()
for t in terminals:
    first[t] = set([t])

def sentencenull(or_statement):
    for atom in or_statement:
        if nullable[atom] == False:
            return False
    return True
    
def findfirst(term):
    if first[term] != None:
        #epsilon and terminals should exit here
        return first[term]
    else:
        #print(term)
        #only non terminals go further than here
        thisfirstset = set()
        i = index[term]
        rhs = rhslist[i]
        
        for or_statement in rhs:
            if or_statement == ["epsilon"]:
                #we need this because epsilon has empty first set
                thisfirstset.add("epsilon")
            
            #in this loop we find orfirstset
            orfirstset = set()
            allnull = True
            for atom in or_statement:
                if nullable[atom] == False:
                    orfirstset = orfirstset.union(findfirst(atom))
                    allnull = False
                    break
                elif nullable[atom] == True:
                    orfirstset = orfirstset.union(findfirst(atom) - set(["epsilon"]))
            if allnull == True:
                orfirstset.add("epsilon")
            
            thisfirstset = thisfirstset.union(orfirstset)
        first[term] = thisfirstset
        return first[term]


print("starting computing first sets")
for term in first.keys():
    if first[term] == None:
        findfirst(term)
    #print(f"{term} has first set:\n{first[term]}")
print("finished computing first sets")

##################FOLLOW SET CODE

# we say follow set of A is a union of other follow sets and first sets
# first sets already computed.
# if follow(A) is a subset of follow(B) but follow(A)
# has not been established what do we do
# the only thing that effects follow(A) is the presence of
# A on the RHS
# after the last occurence on the RHS we can then compute follow(A)
# go through code from top to bottom
# for a fixed A
# follow set dependencies is a list
# first set dependencies is a list
# now order the terms by their last appearance

print("starting computing follow sets")

last_appearance = {nonterminal: -1 for nonterminal in lhslist}
for linenum in range(len(lhslist)):
    rhs = rhslist[linenum]
    for orsequence in rhs:
        for atom in orsequence:
            if atom != "epsilon" and is_terminal[atom] == False:
                last_appearance[atom] = linenum
#below tells us order to compute follow sets
lhssorted = sorted(last_appearance.items(), key=(lambda x: x[1]))
lhssorted = list(map(lambda x: x[0], lhssorted))
#[print(f"{key}: {value}") for (key, value) in last_appearance.items()]
dependencies = {lhs: {"follow":[], "first":[]} for lhs in lhslist} # follow_dependencies, first_dependencies
follow = {lhs: None for lhs in lhslist}
for linenum in range(len(rhslist)):
    lhs = lhslist[linenum]
    rhs = rhslist[linenum]
    for orsequence in rhs:
        l = len(orsequence)
        for i in range(l):
            thisatom = orsequence[i]
            if (thisatom == "epsilon") or (is_terminal[thisatom] == True):
                continue
            if (i < l-1):
                nextatom = orsequence[i+1]                
                for j in range(i+1, l):
                    dependencies[thisatom]["first"].append(nextatom)
                    if nullable[orsequence[j]] == False:
                        break
                if sentencenull(orsequence[i+1:]) == True:
                    dependencies[thisatom]["follow"].append(lhs)
            elif i == l-1:
                dependencies[thisatom]["follow"].append(lhs)

print("Dependencies")
#[print(f"{key}: {value}") for (key, value) in dependencies.items()]
[print(f"{lhs}: {dependencies[lhs]}") for lhs in lhssorted]

# We have to deal with two problems
# If followset(A) depends on followset(B) but B is after A in lhssorted
# this means last appearance of B is after last appearance of A

# this dependency graph has nodes that are sets
# there is an arrow from the node of set X to the node of set Y if
# set X contains set Y
# there are two types of set that can be a node.
# a node could be a follow set. this can have some or no outgoing arrows
# a node could be a first set. this definitely has 0 outgoing arrows since we have already computed it
# if we have a cycle then we need to contract all edges in the cycle
# we keep contracting edges until the graph has no cycles
# note that the number of connected components never change throughout this process of contracting

# how do we do this in code

# let G = dependencies
# let A1, ..., Ak be a cycle k>=2
# Let Gprime = G
# union all the follow and first dependencies of A1,...,Ak and remove A1,...,Ak followdeps to get followdeps, firstdeps
# delete A1,...,Ak from Gprime
# add the tuple (A1,...,Ak) to Gprimes keys with followdeps and first deps
# repeat to get a final graph G with no cycles

G = dependencies.copy()
namenum = 0
numtoterms = {}

while True:
    cycle = find_cycle(G)
    if cycle == None:
        break
    Gprime = G.copy()
    newfollowdeps = set()

    




print("lhs that depend on followsets that are defined below")
for linenum, lhs in enumerate(lhssorted, start=1):
    for dependency in dependencies[lhs]["follow"]:
        if last_appearance[dependency] >= linenum:
            print(f"lhs: {lhs}, linenum: {linenum}, dependency: {dependency}, last appearance: {last_appearance[dependency]} ")
#print(last_appearance["stmt"])

#now compute follow sets
# for nonterminal in lhssorted:
#     print(f"{nonterminal} follow:")
#     thisfollowset = set()
#     followdep = dependencies[nonterminal]["follow"]
#     firstdep = dependencies[nonterminal]["first"]
#     for a in firstdep:
#         thisfollowset.update(first[a] - set("epsilon"))
#     for a in followdep:
#         if a != nonterminal:
#             thisfollowset.update(follow[a])
#     follow[nonterminal] = thisfollowset
#     print(follow[nonterminal])
# print("finished computing follow sets")





#[print(f"{key}: {value}") for (key, value) in lhssorted]







# follow = {term: None for term in lhslist}
# follow["epsilon"] = set()
# for t in terminals:
#     follow[t] = set([t])
    
# #we go production by production
# def findfollow(term):
#     if first[term] != None:
#         #epsilon and terminals should exit here
#         return first[term]
#     else:
#         #print(term)
#         #only non terminals go further than here
#         thisfirstset = set()
#         i = index[term]
#         rhs = rhslist[i]
        
#         for or_statement in rhs:
#             if or_statement == ["epsilon"]:
#                 #we need this because epsilon has empty first set
#                 thisfirstset.add("epsilon")
            
#             #in this loop we find orfirstset
#             orfirstset = set()
#             allnull = True
#             for atom in or_statement:
#                 if nullable[atom] == False:
#                     orfirstset = orfirstset.union(findfirst(atom))
#                     allnull = False
#                     break
#                 elif nullable[atom] == True:
#                     orfirstset = orfirstset.union(findfirst(atom) - set(["epsilon"]))
#             if allnull == True:
#                 orfirstset.add("epsilon")
            
#             thisfirstset = thisfirstset.union(orfirstset)
#         first[term] = thisfirstset
#         return first[term]


#save this data to file
#csv in the form
#term,nullable,firstset,followset
# ostring = ""
# for term in first.keys():
#     ostring += f"{term},{nullable[term]},{first[term]},{follow[term]}\n"
# with open("firstfollow.csv", 'w') as fout:
#     fout.write(ostring)
# print("first and follow sets saved into csv file")
 


            
