import sys

#with open("keywords.txt", "r") as fh:
with open(sys.argv[1], "r") as fh:
    word_list = fh.readlines()

#with open("tokens.h.template", "r") as fh:
with open(sys.argv[2], "r") as fh:
    header = fh.read()

#with open("tokens.c.template", "r") as fh:
with open(sys.argv[3], "r") as fh:
    source = fh.read()

toklist = sorted(["TOK_%s"%(w.strip().upper()) for w in word_list])

hout = header%(
    "\n".join(["    %s,"%(w) for w in toklist]))

with open("tokens.h", "w") as fh:
    fh.write(hout)

sec1 = "\n".join("    { \"%s\", TOK_%s },"%(
            w.strip(), w.strip().upper()) for w in sorted(word_list))
sec2 = "\n".join("        (tok->type == TOK_%s)? \"%s\" :"%(
            w.strip().upper(), w.strip()) for w in word_list)

cout = source%(sec1, sec2)

with open("tokens.c", "w") as fh:
    fh.write(cout)
