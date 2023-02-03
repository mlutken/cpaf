#!/usr/bin/python

# ----------------------------------------------
# --- CPAF build / generate IDE files script ---
# ----------------------------------------------
# Script to create a new CBS project easily
# Run new_project.py --help for options. 

#import utils

import os
import sys
import re
import copy
import shutil
from path import path

import files
import cbs_data



class	NewProject:
	#	*****************
	#	***	Constants ***
	#	*****************
		
	#	************
	#	***	Vars ***
	#	************
	m_CmdArgs				= []
	m_StartUpPath       	= path("")
	m_sProjectName	    	= ""
	m_sCbsProjectFile		= ""
	m_sCbsBuildConfigFile	= ""
	m_sCbsCustomFile		= ""
	m_Options           	= []
	
	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		self.m_CmdArgs	            = copy.copy(cmd_args)
		self.m_StartUpPath          = path(os.getcwd())
		self.m_sProjectName         = self.m_StartUpPath.name
		
		
		# -----------------
		# - Parse options -
		# -----------------
		
		from optparse import OptionParser
		parser = OptionParser()
		parser.add_option("-r", "--recursion-depth", dest="sRecDepth", default="-1", 
							help="RECURDEPTH: Recursion depth", metavar="RECURDEPTH")
		parser.add_option("-n", "--name", dest="sName", default=self.m_sProjectName, 
							help="NAME: name of project", metavar="NAME")
		parser.add_option("-s", "--subdirs", dest="sSubDirs", default="", 
							help="SUBDIRS: (Quoted) List of subdirectories. If left empty all immidiate directories are added.", metavar="SUBDIRS")
		parser.add_option("-b", "--buildconfig-create", dest="sConfigCreate", default="", 
							help="CONFIGCREATE: Create a default config file. Use -b 1 to do this.", metavar="CONFIGCREATE")
		parser.add_option("-c", "--custom-create", dest="sCustomCreate", default="", 
							help="CUSTOMCREATE: Create a default custom project file. Use -c 1 to do this.", metavar="CUSTOMCREATE")
		parser.add_option("-D", "--delete-project-files", dest="sDelPrjFiles", default="0", 
							help="DELPRJFILES: Use -D 1 to delete CMakeLists.txt, PRNAME.cbs in current directory.", metavar="DELPRJFILES")
		
		(self.m_Options, args)  	= parser.parse_args()
		
		self.m_sCbsProjectFile		= "%s.cbs" % (self.m_sProjectName)	
		self.m_sCbsBuildConfigFile	= "%s.BuildConfig.cbs" % (self.m_sProjectName)
		self.m_sCbsCustomFile		= "%s_custom.cbs" % (self.m_sProjectName)
		
		#print "StartUpPath : %s \n" % ( self.m_StartUpPath )
		print "ProjectName : %s" % ( self.m_Options.sName )
		print "CbsFileName : %s" % ( self.m_sCbsProjectFile )
		#print "OS name     : %s \n" % ( os.name )
		
		
#		self.m_iRecurDepth = int(self.m_Options.sRecDepth))
		
#		files.testPathUtils()
#		files.testRecursiveDirFilesFunctions( self.m_StartUpPath, int(self.m_Options.sRecDepth) )  
#		sys.exit(0)
		
		
		self.testAndDeleteProjectFiles()
		#self.testForExistingFiles()
		lSubDirs = self.getSubDirectories()
		
		self.createCMakeListsFile()
		
		#for sDir in lSubDirs:
		#	print "DIR: %s" % (sDir)
			
#		for sLine in self.m_lProjectFile:
#			print sLine,
		sys.exit(0)
		
		
	#	***********************
	#	***	build Functions ***
	#	***********************
	
	def    createCMakeListsFile(self):
		lOutLines = files.substInLines( cbs_data.g_lCMakeListsFile, { "PRJ_NAME" : self.m_sProjectName } ) 
		outFile = open('CMakeLists.txt', 'w')
		outFile.writelines(lOutLines)
		
	def    testAndCreateConfigFile(self):
		if self.m_Options.sConfigCreate == "1":
			print "Creating defaul project build config file '%s' file in current directory ...\n" % (self.m_sCbsBuildConfigFile)
	
	
	def    getSubDirectories(self):
		print "Subdirs %s:" % (self.m_Options.sSubDirs)
		if self.m_Options.sSubDirs == "":
			return files.subdirectoriesListRecursive(self.m_StartUpPath, False, 0, re.compile(r""), re.compile(r"(.*\.svn)|(.*CVS)"))
		else:
			return self.m_Options.sSubDirs.split()
	
	
	
	def    testForExistingFiles(self):
		if files.fileExists("CMakeLists.txt") or files.fileExists(self.m_sCbsProjectFile):
			print "You allready have a CMakeLists.txt and/or a %s file in current directory! \nPlease remove them first ...\n" % (self.m_sCbsProjectFile)
			sys.exit(1)


	def    testAndDeleteProjectFiles(self):
		if self.m_Options.sDelPrjFiles == "1":
			print "Deleting project files CMakeLists.txt and a %s file in current directory ...\n" % (self.m_sCbsProjectFile)
			files.forceCopyFile("CMakeLists.txt", "OLD-CMakeLists.txt")
			files.forceCopyFile(self.m_sCbsProjectFile, "OLD-%s" % self.m_sCbsProjectFile )
			files.safeDeleteFile("CMakeLists.txt")
			files.safeDeleteFile(self.m_sCbsProjectFile)
			sys.exit(1)

#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	NewProject(sys.argv)



