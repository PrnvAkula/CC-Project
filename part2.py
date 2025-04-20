from nltk.tree import *

with open("output.txt","r") as f:
    data=f.read()

data=Tree.fromstring(data)
data.pretty_print(unicodelines=True,nodedist=5) 