import re
import sys

def includePlugs(s, plugType, typeName):
    if (re.match(".*" + plugType + ".*", s, re.DOTALL) != None):
        s = re.sub("(\#include .*).*\n", "\\1\n#include \"" + typeName + ".h\"\n",s,1)
    return s

def convertOldStylePlugs(s, oldPlugType, newPlugType):
    s = re.sub(oldPlugType, newPlugType ,s)
    return s

def convertAddPlugs(s, oldPlugType, newPlug, newType):
    newPlugType = newPlug + "<" + newType + ">"
    s = re.sub("(\s*)(.*)=\s*add" + oldPlugType + "\((.*),\s*(.*)\);", "\\1addPlug(\\2= new " + newPlugType + "(this, \\3, new " + newType + "(\\4)));", s);
    s = re.sub("(\s*)(.*)=\s*add" + oldPlugType + "\((.*)\);", "\\1addPlug(\\2= new " + newPlugType + "(this, \\3));", s);
    return s

def convertHeaderTypes(s):
    s = re.sub("(?!Q)Canvas", "MatrixType<RGBA>",s)
    s = re.sub("Video_T", "MatrixType<RGBA>",s)
    s = re.sub("Signal_T\s*\*", "MatrixType<float> ",s)
    s = re.sub("Signal_T", "float ",s)
    return s

def removeHeaderUseless(s):
    s = re.sub("\s*Plug.*\(\)\s*\{\s*return.*\}\s*;\s*\n","",s)
    return s

def convertImplTypes(s):
    s = re.sub("->canvas\(\)", "->type()->image()",s)
    s = re.sub("->_data", "->data()",s)
    s = re.sub("->sizeX\(\)", "->width()",s)
    s = re.sub("->sizeY\(\)", "->height()",s)
    s = re.sub("->allocate", "->resize",s)
    s = re.sub("->buffer\(\)\[(\d*)\]", "->type()->buffer()[\\1]",s)
    s = re.sub("->buffer\(\)", "->type()->buffer()",s)
    s = re.sub("Video_T", "MatrixType<RGBA>",s)
    s = re.sub("Signal_T\s\*", "MatrixType<float> ",s)
    s = re.sub("Signal_T", "float ",s)
    return s
    
def convertHeaderFile(s):
    s = includePlugs(s, "PlugVideo", "VideoType")
    s = includePlugs(s, "PlugSignal", "SignalType")
    s = removeHeaderUseless(s)
    s = convertOldStylePlugs(s, "PlugVideoIn", "PlugIn<VideoTypeRGBA>");
    s = convertOldStylePlugs(s, "PlugVideoOut", "PlugOut<VideoTypeRGBA>");
    s = convertOldStylePlugs(s, "PlugSignalIn", "PlugIn<SignalType>");
    s = convertOldStylePlugs(s, "PlugSignalOut", "PlugOut<SignalType>");
    s = convertHeaderTypes(s)
    return s

def convertImplFile(s):
    s = convertAddPlugs(s, "PlugVideoIn", "PlugIn", "VideoTypeRGBA");
    s = convertAddPlugs(s, "PlugVideoOut", "PlugOut", "VideoTypeRGBA");
    s = convertAddPlugs(s, "PlugSignalIn", "PlugIn", "SignalType");
    s = convertAddPlugs(s, "PlugSignalOut", "PlugOut", "SignalType");
    s = convertImplTypes(s)
    return s

filename = sys.argv[1]
f = open(filename, "r")
fsave = open("/tmp/" + filename + ".sav", "w")

mystr = f.read()
f.close()

fsave.truncate(0)
fsave.write(mystr)

if (re.match(".*\.h", filename)):
    mystr = convertHeaderFile(mystr)
else:
    if (re.match(".*\.cpp", filename)):
        mystr = convertImplFile(mystr)
    else:
        print "Unknown file format"

f = open(filename, "w")
f.write(mystr)
f.close()
fsave.close()

## class MergeDef:
##     def __init__(self, series1, series2, factor, increment, invert):
##         self.series1 = series1
##         self.series2 = series2
##         self.factor = factor
##         self.increment = increment
##         self.invert = invert

##     def printIt(self):
##         print "("+self.series1+","+self.series2+","+str(self.factor)+","+str(self.increment)+","+str(self.invert)+")"

## filename = sys.argv[1]

## f = open (filename)
## mystr = f.read()

## p = re.compile("if .*?endif;", re.DOTALL)
## list_str = p.findall(mystr)

## list_merge = list()

## for s in list_str:
##     print s
    
##     # Get first series
##     p = re.compile("if .* == (\d*);")
##     res = p.search(s)
##     series1 = res.group(1)

##     # Get second series
##     p = re.compile("yy=selif\(toto',\(toto\[1,.\]' \.== (\d*)\)\)';")
##     res = p.search(s)
##     series2 = res.group(1)

##     # Get factor
##     p = re.compile("z\s*=([^;]*);")
##     res = p.search(s)
##     if res:
##         ss = res.group(1)
##         factor = eval(re.sub("(?!.)(\d*)/", "\\1.0/", ss))
##     else:
##         factor = 1.0
    
##     # Get increment
##     p = re.compile("yy\s*=\s*yy\s*\+\s*(.*);")
##     res = p.search(s)
##     if res:
##         increment = eval(res.group(1))
##     else:
##         increment = 0.0
    
##     # Get invert
##     p = re.compile("yy\s*=\s*1\s*\./\s*yy;")
##     res = p.search(s)
##     if res:
##         invert = True
##     else:
##         invert = False

##     x = MergeDef(series1, series2, factor, increment, invert)
##     x.printIt()
##     list_merge.append(x)

## for x in list_merge:
##     x.printIt()
