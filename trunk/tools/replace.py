## Replaces some search string by another string in given files.

import sys
import re
from os.path import basename

if (len(sys.argv) < 4):
    print "Usage: replace <search_text> <replace_text> [file(s) ...]"
    sys.exit(0)

stext = sys.argv[1]
rtext = sys.argv[2]

print len(sys.argv)

for i in range(3,len(sys.argv)):
    f = open(sys.argv[i], "r+")
    print "Opening file : " + sys.argv[i]
    if f:
        s = ""
        while True:
            line = f.readline()
            if (line == ""):
                break
            line = line.replace(stext, rtext)
            s += line
        f.seek(0)
        f.truncate(0)
        f.write(s)
        f.close()
