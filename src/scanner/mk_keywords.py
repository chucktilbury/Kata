# Read the keywords.txt file and generate keywords.c and keywords.h

raw_list = []
with open("keywords.txt", "r") as inf:
    for line in inf:
        s = line.strip()
        if len(s) > 0:
            raw_list.append(s)

raw_list.sort()
print (raw_list)

with open("keywords.c", "w") as outf:
    outf.write("/* This file is generated with a script. Do not edit. */\n\n")
    outf.write("#include \"tokens.h\"\n\n")
    outf.write("KeywordList keyword_list[] = {\n")
    for item in raw_list:
        outf.write("    { \"%s\", TOK_%s },\n"%(item, item.upper()))
    outf.write("};\n\n")
    outf.write("const int num_keywords = (sizeof(keyword_list) / sizeof(KeywordList));\n\n")
