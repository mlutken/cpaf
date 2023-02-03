
import os
import shutil
import re
import sys
import time

#***********************
#*** Print Functions ***
#***********************
def Print(str, bInsertNewLine= 1):
	""" Prints the string to std_out and calls flush. """
	if bInsertNewLine:
		print str
	else:
		print str,
	sys.stdout.flush()


#**********************
# *** ***
#**********************
def substInLines( lLines, dSubstWordDict ):
	lOutLines = []
#	for elem in dSubstWordDict.items():
#		print "Key: %s" % elem[0]
#		print "Val: %s" % elem[1]
		
	for sLineIn in lLines:
		for elem in dSubstWordDict.items():
			sLineOut = sLineIn.replace( elem[0], elem[1] )	# key, value
		lOutLines.append(sLineOut)
#		print sLineOut,
	return lOutLines


#***********************************
#*** File/Path Name Manipulation ***
#***********************************


def convertToForwardSlashes(fileName):
	""" Slashes stuff ... '\' and '/'.
		Convert to forward slashes. """
	fileNme = fileName.replace("\\", "/")
	return fileNme


def convertToBackwardSlashes(fileName):
	""" Slashes stuff ... '\' and '/'.
		Convert to backward slashes. """
	fileNme = fileName.replace("/", "\\")
	return fileNme


def splitFileNameToPathAndName(fileName):
	""" Splits a scene name form ('Singleplayer/C03A/FF-C03A_MAIN') in to 
		path and name like this: returns tuple (Singleplayer/C03A/ , FF-C03A_MAIN)."""
	
	fileNameScan	= re.compile(r"(.*)[/\\]([^/\\]*)$", re.I)

	match			= fileNameScan.match(fileName)
	try:			path = match.group(1) + "/"
	except:			path = ""

	try:			name = match.group(2)
	except:			name = fileName

	return path, name


def getPathOfFile(fileName):
	""" Returns the path of a file. """
	path, name = splitFileNameToPathAndName(fileName)
	return path


def removePathFromFile(fileName):
	""" Removes the path section of a file name and returns just the name. """
	path, name = splitFileNameToPathAndName(fileName)
	return name


def removeLastSlashFromPath(pathName):
	""" """
	#pathNameScan	= re.compile(r"(.*)[/\\]([^/\\]*)$", re.I)
	pathNameScan	= re.compile(r"(.*)[/\\]$", re.I)

	match			= pathNameScan.match(pathName)
	try:			path = match.group(1)
	except:			path = pathName

	return path


def insertEndSlashInPathName(pathName):
	""" Inserts an end slash '/' at the end of the path name or changes 
		to a '/' if the last char in pathname is a backslash '\'. """
	
	path = pathName
	
	lenName		= len(pathName)
	lastChar	= lenName -1
	
	path = ""
	for i in range(0,lenName):
		if i == lastChar:
			if pathName[i] == '/':
				path += pathName[i]
#			elif pathName[i] == r'\'
#				path += '/'
			else:
				path += pathName[i]
				path += '/'
		else:
			path += pathName[i]

	return path



def getParentAndLeafDir(pathName):
	""" Gets the parent dir of a directory. Also gets
		the leaf dir. Eg. if orgPath = 'q:/freedomfighter/final/'.
		Then this function returns ('q:/freedomfighter/', final/'). """
	
	pathName = removeLastSlashFromPath(pathName)
	parentPath, leafPath = splitFileNameToPathAndName(pathName)

	leafPath = insertEndSlashInPathName(leafPath)
	parentPath = insertEndSlashInPathName(parentPath)

	return parentPath, leafPath
		


def convertToWindowsNetPath(netPath):
	netPath = netPath.replace("//", "\\\\")
	netPath = netPath.replace("/", "\\")
	netPath = removeLastSlashFromPath(netPath)	# Experimental XX but hopefully works
	return netPath


def convertToCygwinPath(path):
	""" Converts a windows style path with drive letter to cygwin path.
		Fx. 'z:/code/engine' -> '/cygdrive/z/code/engine/'. Needed eg. 
		when modifying PATH environment variable. """
		 
	if path == "": return path
	driveLetter = path[0]
	replaceStr	= "%s:/" % (driveLetter)
	path = path.replace(replaceStr, "/cygdrive/%s/" % (driveLetter))
	return path


