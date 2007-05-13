#!/usr/bin/python

import os, sys
from string import Template

# Parse arguments
if (len(sys.argv) > 1):
	frei0rPath = sys.argv[1]
else:
	frei0rPath = "/usr/local/lib/frei0r-1/"

if (len(sys.argv) > 2):
	pluginExtension = sys.argv[2]
else:
	pluginExtension = ".so"

# Add trailing slash
if not frei0rPath.endswith("/"):
	frei0rPath += "/"

# Add beggining dot
if not pluginExtension.startswith("."):
	pluginExtension = "." + pluginExtension

pluginList = os.listdir(frei0rPath)
for plugin in pluginList:
	# First check the plugin's extension
	if not plugin.endswith(pluginExtension):
		continue
	# Complete name
	name = "Frei0r_" + plugin[0:plugin.find(pluginExtension)]
	# Complete path
	path = frei0rPath + plugin
	print "<entity name=\"" + name + "\" class=\"drone.frei0r.actor.Frei0rActor\">"
	print "  <property name=\"frei0rLibraryName\" value=\"" + path + "\" class=\"ptolemy.data.expr.FileParameter\" />"
	print "</entity>"
	
