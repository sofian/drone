## Adds a preamble to the listed files. Please make a copy before, since
## there is no reversion mechanism.

import sys
import re
from os.path import basename

if (len(sys.argv) < 3):
    print "Usage: addpreamble <preamble> [file(s) ...]"
    sys.exit(0)

fpreamble = open(sys.argv[1], "r")
if fpreamble:
    preamble = fpreamble.read()
else:
    exit

p = re.compile("%FILE%")

print len(sys.argv)

for i in range(2,len(sys.argv)):
    f = open(sys.argv[i], "r+")
    print "Opening file : " + sys.argv[i]
    if f:
        filename = basename(sys.argv[i])
        s = f.read() # read all file
        pr = p.sub(filename, preamble)
        f.seek(0)
        f.truncate(0)
        f.write(pr)
        f.write(s)
        f.close()
                       
