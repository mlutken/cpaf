#!/usr/bin/python

#
# TODO: "%VS80COMNTOOLS%"\vsvars32.bat && cbmake.py
#

# ----------------------------------------------
# --- CPAF build / generate IDE files script ---
# ----------------------------------------------
# Script that make building project or generating IDE project files easy.
# Run build.py --help for options. 
# IMPORTANT: When building project libraries/executables you please follow the 
# convention that the project name in your cmake-file is identical to the 
# directory (leaf) name if possible.

#import utils

import os
import sys
import re
import copy
import shutil
from path import path

def runOsSystem(cmd, bDry, bPrintCmd):
	""" Runs the command using 'os.system()' but only if bDry is not set. """

	if bPrintCmd:
		print cmd
	if bDry :   
		return
	else    :   
		return os.system(cmd)


class	CbsNew:
	#	************
	#	***	Vars ***
	#	************
	m_cmd_args				= []
	m_StartUpPath       	= path("")
	m_sScriptsPath     		= path("")
	m_TemplatesPath         = path("")
	m_sVerboseCmd       	= ""
	m_Options           	= []

	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		self.m_cmd_args	            = copy.copy(cmd_args)
		self.m_StartUpPath          = path(os.getcwd())
		self.m_sCbsPath 			= path(sys.argv[0]).parent
#		self.m_sCbsPath 			= self.m_sCbsPath.parent
		
		self.m_TemplatesPath		= self.m_sCbsPath / path("templates")
		
#		self.m_sDefaultProjectName  = self.m_StartUpPath.name

		print "m_StartUpPath : %s \n" % 	( self.m_StartUpPath 	)
		print "m_sCbsPath    : %s \n" % 	( self.m_sCbsPath 		)
		print "m_TemplatesPath : %s \n" % 	( self.m_TemplatesPath 	)
		

		# -----------------
		# - Parse options -
		# -----------------
	
		from optparse import OptionParser
		parser = OptionParser()
		parser.add_option("-t", "--type",     dest="sType",    default="library",
							help="Type to create: prj(project), lib(brary), mod(ule), exe(cutable). DEFAULT: 'library'.", 
							metavar="TYPE")
		parser.add_option("-n", "--name",     dest="sName",    default="auto",
							help="Name of target/project. DEFAULT: 'auto' which means that current leaf directory name is used.", 
							metavar="NAME")
		parser.add_option("-c", "--config",     dest="sConfig",    default="0",
							help="Create a config file: 'prj/tgt_config.cbs'. DEFAULT: '0'.", 
							metavar="CONFIG")
		parser.add_option("-s", "--svn",     dest="sSvn",    default="0",
							help="Set to 1 to automatically add the files to svn. DEFAULT: '0'.", 
							metavar="SVN")
		parser.add_option("-d", "--dry",     dest="sDry",    default="0",
							help="Run dry, no changes. DEFAULT: '0'.", 
							metavar="DRY")
		
		(self.m_Options, args)  = parser.parse_args()
		
		if self.m_Options.sName == "auto":	self.m_Options.sName = self.m_StartUpPath.name
		
		
		if self.m_Options.sType == "prj":	self.m_Options.sType = "project"
		if self.m_Options.sType == "lib":	self.m_Options.sType = "library"
		if self.m_Options.sType == "mod":	self.m_Options.sType = "module"
		if self.m_Options.sType == "exe":	self.m_Options.sType = "executable"
		
		print "sType   : %s" %        ( self.m_Options.sType 	)
		print "sName   : %s" %        ( self.m_Options.sName 	)
		print "sConfig : %s" %        ( self.m_Options.sConfig 	)
		print "sSvn    : %s" %        ( self.m_Options.sSvn 	)
		print "sDry    : %s" %        ( self.m_Options.sDry 	)
		
			
		self.m_bDry = int(self.m_Options.sDry)	
		
		self.createCmakeListsFile()
		self.createCbsMainFile()
		self.createCbsConfigFile()

		sys.exit(0)
				

	#   *****************
	#   *** Functions ***
	#   *****************

	def createCmakeListsFile(self):
		sFile = open(self.m_TemplatesPath / path("template_CMakeLists.txt"), "r").read()
		sFile = sFile.replace("@PRJTGT_NAME@", self.m_Options.sName)
		sFile = sFile.replace("@TYPE_TO_BUILD@", self.m_Options.sType.upper())
		if self.m_Options.sDry == "1"	: print "\n*** CMakeLists.txt ***\n%s\n" % (sFile)
		else							: open( self.m_StartUpPath / path("CMakeLists.txt"), "w").write(sFile)
		if self.m_Options.sSvn == "1": 
			sCmd = "svn add CMakeLists.txt"
			runOsSystem(sCmd, self.m_bDry, 1)

	
	def createCbsMainFile(self):
		sCbsMainTemplate = "template_target.cbs"
		if self.m_Options.sType == "project"	: sCbsMainTemplate = "template_project.cbs"
		sFile = open(self.m_TemplatesPath / path(sCbsMainTemplate), "r").read()
		sFile = sFile.replace("@PRJTGT_NAME@", self.m_Options.sName)
		sFile = sFile.replace("@TYPE_TO_BUILD@", self.m_Options.sType.upper())
		sWriteFileName = self.m_Options.sName + ".cbs"
		if self.m_Options.sDry == "1"	: print "\n*** %s ***\n%s\n" % (sWriteFileName, sFile)
		else							: open( self.m_StartUpPath / path(sWriteFileName), "w").write(sFile)
		if self.m_Options.sSvn == "1": 
			sCmd = "svn add %s" % (sWriteFileName)
			runOsSystem(sCmd, self.m_bDry, 1)
	
	
	def createCbsConfigFile(self):
		if self.m_Options.sConfig == "0": return
		sFile = open(self.m_TemplatesPath / path("template_config.cbs"), "r").read()
		sFile = sFile.replace("@PRJTGT_NAME@", self.m_Options.sName)
		sFile = sFile.replace("@TYPE_TO_BUILD@", self.m_Options.sType.upper())
		sWriteFileName = self.m_Options.sName + "_config.cbs"
		if self.m_Options.sDry == "1"	: print "\n*** %s ***\n%s\n" % (sWriteFileName, sFile)
		else							: open( self.m_StartUpPath / path(sWriteFileName), "w").write(sFile)
		if self.m_Options.sSvn == "1": 
			sCmd = "svn add %s" % (sWriteFileName)
			runOsSystem(sCmd, self.m_bDry, 1)


	
	def   deleteCbsNewTargetCmd(self):
		pass
	

	#   ************************
	#   *** Helper Functions ***
	#   ************************



#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	CbsNew(sys.argv)



