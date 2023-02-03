#!/usr/bin/python

# Base class for 
# Run new_project.py --help for options. 

#import utils

import os
import sys
import re
import copy
import shutil
from path import path
from optparse import OptionParser

import files
import cbs_data
import cbs_utils



class	CbsBase:
	""" Base class for all project/target related classes/commands."""
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
	m_lCMakeListsFile		= []
	m_lCbsProjectFile		= []
	m_lCbsBuildConfigFile	= []
	m_lCbsCustomFile		= []
	m_Options           	= []
	m_OptionParser			= OptionParser()
	m_bCmdLineMode			= True			
	
	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		#print "CbsBase CONSTRUCTOR"		
		self.m_CmdArgs	            = copy.copy(cmd_args)
		self.m_StartUpPath          = path(os.getcwd())
		
		
		# -----------------
		# - Parse options -
		# -----------------
		
		self.addOptions()
		(self.m_Options, args)  	= self.m_OptionParser.parse_args()
		
		self.m_sProjectName         = self.m_Options.sName
		self.m_sCbsProjectFile		= "%s.cbs" % (self.m_sProjectName)	
		self.m_sCbsBuildConfigFile	= "%s.BuildConfig.cbs" % (self.m_sProjectName)
		self.m_sCbsCustomFile		= "%s_custom.cbs" % (self.m_sProjectName)
		
		#print "StartUpPath : %s \n" % ( self.m_StartUpPath )
		#print "ProjectName : %s" % ( self.m_sProjectName )
		#print "CbsFileName : %s" % ( self.m_sCbsProjectFile )
		#print "OS name     : %s \n" % ( os.name )
		
		
		
	def    run(self):
		self.testAndDeleteProjectFiles()
		self.runCmdLineMode()
			
		
	#	***********************
	#	***	build Functions ***
	#	***********************
	
	def    createDefaultCMakeListsFile(self):
		self.m_lCMakeListsFile = files.substInLines( cbs_data.g_lCMakeListsFile, { "PRJ_NAME" : self.m_sProjectName } ) 
		
		
	def    writeCMakeListsFile(self):
		print "writeCMakeListsFile"
		outFile = open('CMakeLists.txt', 'w')
		outFile.writelines(self.m_lCMakeListsFile)
			
			
	def    writeCbsProjectFile(self):
		print "writeCbsProjectFile"
		outFile = open(self.m_sCbsProjectFile, 'w')
		outFile.writelines(self.m_lCbsProjectFile)
		
		
	def    writeCbsBuildConfigFile(self):
		print "writeCbsBuildConfigFile"
		outFile = open(self.m_sCbsBuildConfigFile, 'w')
		outFile.writelines(self.m_lCbsBuildConfigFile)
		
		
	def    writeCbsCustomFile(self):
		print "writeCbsCustomFile"
		outFile = open(self.m_sCbsCustomFile, 'w')
		outFile.writelines(self.m_lCbsCustomFile)
		
		
	def    testAndCreateConfigFile(self):
		if self.m_Options.sConfigCreate == "1":
			print "Creating default project build config file '%s' file in current directory ...\n" % (self.m_sCbsBuildConfigFile)


	def    testAndDeleteProjectFiles(self):
		if self.m_Options.sDelPrjFiles == "1":
			self.deleteProjectFiles()
			if self.m_bCmdLineMode: sys.exit(1)

	
	def    deleteProjectFiles(self):
		print "Deleting project files CMakeLists.txt and a %s file in current directory ...\n" % (self.m_sCbsProjectFile)
		files.forceCopyFile("CMakeLists.txt", "OLD-CMakeLists.txt")
		files.forceCopyFile(self.m_sCbsProjectFile, "OLD-%s" % self.m_sCbsProjectFile )
		files.safeDeleteFile("CMakeLists.txt")
		files.safeDeleteFile(self.m_sCbsProjectFile)

	def    prjFilesExists(self):
		if files.fileExists("CMakeLists.txt") or files.fileExists(self.m_sCbsProjectFile):
			print "You allready have a CMakeLists.txt and/or a %s file in current directory! \nPlease remove them first ...\n" % (self.m_sCbsProjectFile)
			if self.m_bCmdLineMode: sys.exit(1)
			return True
		return False	


	def    addOptions(self):
		self.m_OptionParser.add_option("-r", "--recursion-depth", dest="sRecDepth", default="-1", 
							help="RECURDEPTH: Recursion depth", metavar="RECURDEPTH")
		self.m_OptionParser.add_option("-n", "--name", dest="sName", default=self.m_StartUpPath.name, 
							help="NAME: name of project", metavar="NAME")
		self.m_OptionParser.add_option("-b", "--buildconfig-create", dest="sConfigCreate", default="", 
							help="CONFIGCREATE: Create a default config file. Use -b 1 to do this.", metavar="CONFIGCREATE")
		self.m_OptionParser.add_option("-c", "--custom-create", dest="sCustomCreate", default="", 
							help="CUSTOMCREATE: Create a default custom project file. Use -c 1 to do this.", metavar="CUSTOMCREATE")
		self.m_OptionParser.add_option("-D", "--delete-project-files", dest="sDelPrjFiles", default="0", 
							help="DELPRJFILES: Use -D 1 to delete CMakeLists.txt, PRNAME.cbs in current directory.", metavar="DELPRJFILES")
		self.m_OptionParser.add_option("-G", "--gui", dest="sGui", default="0", 
							help="GUI: Launch Gui version of this command. Use -G 1 for this", metavar="GUI")

	#**********************
	#*** List functions ***
	#**********************
	
	def createValuesSETList( self, sValueType, lValues ):
		""" Create a list of valus with entries like e.g. 
			SET ( m_sProjectName_VALUETYPE ${m_sProjectName_VALUETYPE}    myvalue ) """
		return cbs_utils.createValuesSETList( self.m_sProjectName, sValueType, lValues )

#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	NewProject(sys.argv)



