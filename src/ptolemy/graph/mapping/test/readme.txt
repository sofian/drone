
Tests for the synthesis.graph.mapping package.

@author Shahrooz Shahparnia based on a file by S. Bhattacharyya
@version $Id: readme.txt,v 1.1 2003/06/27 20:59:50 shuvra Exp $

If a test is giving errors write the content of the buffer to a file and
compare. Following code can be used for this purpose.

    set fileChannel [open temp.txt w+]
    puts $fileChannel $fileBuffer 
    close $fileChannel