
import os
import shutil
import re
import sys
import time
import text



#***************************
#*** Path Util functions ***
#***************************

def tailname ( sPath ):
	sHead, sTail = os.path.split(sPath)
	return sTail


def pathToList( path ):
	""" Convert path to a list with individual subpaths as elements"""
	lPath = []
	bDone = False
	
	if path == "": return []
	
	sHead = os.path.normpath(path) 
	while not bDone:
#		print "sHead: %s " % ( sHead )
		sHead, sTail = os.path.split( sHead )
		if sTail != "" : 
			lPath.insert(0, sTail )
		else: 
			if sHead != "": lPath.insert(0, sHead )
			bDone = True
			
	return lPath


def intersectionFromStart( path1, path2 ):                                         
	""" Get common part of the two from the start. Finds the intersection of two parts 
		from the beginning of the paths. See the example below.
		\return The intersection/common-part (from the root and forward) of the two paths.
		\example
			path1 = "z:/hello/horse/goat/pig/cow/sheep"
			path2 = "z:/hello/horse/whale/dolphin"
			intersectionFromStart(inPath, remPath) == "z:/hello/horse" 
		\endexample"""
	l1  = pathToList( path1 )
	l2  = pathToList( path2 )
	it1 = 0 
	it2 = 0 
    
	if l1[it1] != l1[it2]: 
		return ""    # If path do not have same root, return an empty path !!!
	
	intersectionPath = ""
	while (it1 != len(l1)) and (it2 != len(l2)) and ( l1[it1] == l2[it2]) :
		intersectionPath = os.path.join( intersectionPath, l1[it1] )
		it1 = it1 + 1
		it2 = it2 + 1
	return intersectionPath;


def removeFromStart( inPath, remPath ):                                         
	""" Remove part of path from beginning. See the example below. If paths does 
		not have the same root then inPath is returned.
		\return inPath with the intersection of inPath and remPath removed.
		\example
    		inPath = "z:/hello/horse/goat/pig/cow/sheep"
    		remPath = "z:/hello/horse"
    		removeFromStart(inPath, remPath) == "goat/pig/cow/sheep"
		\endexample"""
	
	lIn   = pathToList( inPath )
	lRem  = pathToList( remPath )
	itIn  = 0 
	itRem = 0 

	if inPath[itIn] != remPath[itRem]: 
		return inPath  # If path do not have same root, just return inPath !!!
 
	while (itIn != len(lIn)) and (itRem != len(lRem) and  (lIn[itIn] == lRem[itRem]) ):
		itIn  = itIn  + 1
		itRem = itRem + 1
		
	outPath = ""
	for it in range (itIn, len(lIn)):
		outPath = os.path.join( outPath, lIn[it])
	return outPath


def relativePath ( pathFrom, pathTo ):                                         
	""" Make relative path from dir to file. Use this function to make a relative 
		link to a file in one directory, from a given directory. Typically one has 
		an absolute from path (no filename only path) and an absolute path to the 
		file to link to (path and filename). See examples.
		\return Relative path from pathFrom to pathTo. 
		\example
			linkFrom = "z:/hello/horse/goat/pig/cow/sheep"
			linkTo	 = "z:/hello/horse/whale/dolphin/seal.txt"
			relativePath(linkFrom, linkTo) == "../../../../whale/dolphin/seal.txt"
		\endexample"""
	
	lFrom  = pathToList( pathFrom )
	lTo    = pathToList( pathTo   )
	itFrom = 0 
	itTo   = 0 
	
	if lFrom[itFrom] != lTo[itTo]: 
		return pathTo  # If path do not have same root, just return "to path"!!!
	
	while (itFrom != len(lFrom)) and (itTo != len(lTo)) and ( lFrom[itFrom] == lTo[itTo] ) :
		itFrom 	= itFrom + 1
		itTo	= itTo   + 1
	
	relPath = ""
	for it in range (itFrom, len(lFrom)):
		relPath = os.path.join( relPath, "..")

	for it in range (itTo, len(lTo)):
		relPath = os.path.join( relPath, lTo[it])
	return relPath;


#***************************
#*** File Util functions ***
#***************************


def fileExists(fullFileName):
	bFileExists	= 1
	try				: os.stat(fullFileName)
	except OSError	: bFileExists = 0
	return bFileExists


def deleteFile(fileName, bDry= 0):
	from path import path
	#cmd = "rm -f %s" % (fullFileName)
	#utils.RunOsSystem(cmd, bDry, 0)
	fileName = path(fileName).abspath()
	print "fileName: %s" % (fileName)
	os.remove(fileName)
	
def makeFileWritable(fileName):
	return
#	fullFileName = text.ConvertToBackwardSlashes(fullFileName)
#	fullFileName = fullFileName.replace("\\", "\\\\")
#	cmd = "attrib -R %s" % (fullFileName)
#	os.system(cmd)

def safeDeleteFile(fileName, bDry= 0):
	if fileExists(fileName):
		makeFileWritable(fileName)
		deleteFile(fileName, bDry)


def makeFileExecuteable(fullFileName):
	return # Windows version :-)
#
#	# Cygwin version
#	cmd = "chmod a=rwx %s" % (fullFileName)
#	os.system(cmd)




def safeMakeDirs(path):
	""" Makes all dirs in path."""
	if FileExists(path): return
	try:		os.makedirs(path)
	except:		print



def safeDeletePath(fullPathName):
	safeMakeDirs(fullPathName)
	shutil.rmtree(fullPathName)
#	try				: shutil.rmtree(fullPathName)
#	except OSError	: print "ERROR Deleting Path: " + fullPathName


def subdirectoriesListRecursive( top, bFullPaths, iRecursionDepth, reMatch = re.compile(r""), reSkip = re.compile(r"$DUMMY")):
	top = os.path.abspath( top )
	lSubdirs = []
	for root, lDirs, lFiles in os.walk(top, topdown=True):
		relSubDir 	= removeFromStart( root, top )
		iLevel 		= len( pathToList(relSubDir) ) 
		if reSkip.match( tailname(root) ): continue
		if (iRecursionDepth != -1) and (iLevel > iRecursionDepth): continue 
		for dirName in lDirs:
			name = os.path.join( root, dirName )
			relativePath = removeFromStart( name, top )
			
			tail = tailname(relativePath)
			if reSkip.match( relativePath ): continue
			if not reMatch.match( tail ): continue
			
			if not bFullPaths: name = relativePath
			lSubdirs.append ( name )
			
	return lSubdirs


	
def filesListRecursive( top, bFullPaths, iRecursionDepth, reMatch = re.compile(r""), reSkip = re.compile(r"$DUMMY")):
	top = os.path.abspath( top )
	lFiles = []

	for root, lDirs, lSubFiles in os.walk(top, topdown=True):
		relSubDir 	= removeFromStart( root, top )
		iLevel 		= len( pathToList(relSubDir) ) 
		if reSkip.match( tailname(root) ): continue
		if (iRecursionDepth != -1) and (iLevel > iRecursionDepth): continue 
		for fileName in lSubFiles:
			name = os.path.join( root, fileName )
			relativePath = removeFromStart( name, top )
			
			tail = tailname(relativePath)
			if reSkip.match( relativePath ): continue
			if not reMatch.match( tail ): continue
			
			if not bFullPaths: name = relativePath
			lFiles.append ( name )
			
	return lFiles


#****************************
#*** File Stats Functions ***
#****************************

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


#********************************
#*** File/Dirs Copy Functions ***
#********************************

def forceCopyFile(srcFile, dstFile):
	try:
		shutil.copy(srcFile, dstFile)
	except IOError:
		print "Safe Copy!" 

def forceRemoveTree(dir):
	if os.path.exists(dir):
		shutil.rmtree(dir)	

def forceCopyTree(srcDir, dstDir):
	if os.path.exists(dstDir):
		shutil.rmtree(dstDir)	
	shutil.copytree(srcDir, dstDir)



def zipDir(dirToZip, zipFile, bDry):
	""" Zips files in dir to a zip file."""

	saveDir = os.getcwd()		# Save current directory
	os.chdir(dirToZip)
	
	try		:	os.remove(zipFile)
	except	:	print "Note: File %s does not exist!" % (zipFile)

	cmdLine = "find -iname '*.*' | zip -q -@ %s" % (zipFile)

	print cmdLine
	Print("Packing to zip file %s... " % (zipFile), 0)
	utils.RunOsSystem(cmdLine, bDry, 0)
	os.chdir(saveDir)			# restore original dir
	print "Done"



#****************************
#*** Test/Debug Functions ***
#****************************

def testRecursiveDirFilesFunctions( top, iRecursionDepth ):
#	lNames = subdirectoriesListRecursive(top, False, iRecursionDepth, re.compile(r".*process.*"), re.compile(r"\.svn"))
#	lNames = subdirectoriesListRecursive(top, False, iRecursionDepth, re.compile(r""), re.compile(r".*\.svn"))
#	lNames = subdirectoriesListRecursive(top, False, iRecursionDepth, re.compile(r".*\.svn"))
	lNames = filesListRecursive         (top, False, iRecursionDepth, re.compile(r""), re.compile(r"(.*\.hpp)|(.*\.cpp)"))
	print "TOP(%d): %s " % ( iRecursionDepth, top )
	for sName in lNames:
		print "NAME: %s" % (sName)
	


def testPathUtils():
	
	pathOne    = "one"
	pathTwo    = "one/two"
	pathFull   = "/one/two"
	
	print "pathOne:  %s , pathToList(pathOne): %s"  % (pathOne, pathToList(pathOne))
	print "pathTwo:  %s , pathToList(pathTwo): %s"  % (pathTwo, pathToList(pathTwo))
	print "pathFull: %s , pathToList(pathFull): %s" % (pathFull, pathToList(pathFull))
	
	inPath  = "/hello/horse/goat/pig/cow/sheep"
	remPath = "/hello/horse/"
	interSectPath 	= intersectionFromStart( inPath, remPath )
	resPath 		= removeFromStart(inPath, remPath)
	
	print "inPath:  %s " % inPath
	print "remPath: %s " % remPath
	print "interSectPath: 	%s " % ( interSectPath )
	print "resPath: 		%s " % ( resPath )
	
	linkFrom 	= "/hello/horse/goat/pig/cow/sheep"
	linkTo	 	= "/hello/horse/whale/dolphin/seal.txt"
	relPath 	= relativePath(linkFrom, linkTo) # == "../../../../whale/dolphin/seal.txt"
	print "linkFrom: 		%s " % ( linkFrom )
	print "linkTo: 			%s " % ( linkTo )
	print "relPath: 		%s " % ( relPath )
	
