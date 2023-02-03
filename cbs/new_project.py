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

from cbs_base import CbsBase


#from cbs_document import *
import cbs_document

class	Project (CbsBase) :
	#	*****************
	#	***	Constants ***
	#	*****************
		
	#	************
	#	***	Vars ***
	#	************
	
	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		CbsBase.__init__(self, cmd_args)
		#print "Project CONSTRUCTOR"		
			
		



class	NewProject (Project) :
	#	*****************
	#	***	Constants ***
	#	*****************
		
	
	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		Project.__init__(self, cmd_args)
		#print "NewProject CONSTRUCTOR"		
			
		cbs_document.testDebugDocument()
		
		sys.exit(0)
		
		
		
	#	***********************
	#	***	XX Functions ***
	#	***********************
	def    runCmdLineMode(self):
		print "NewProject: runCmdLineMode"
		self.prjFilesExists()
		self.createDefaultCMakeListsFile()
		self.writeCMakeListsFile()
		
		lSubDirs = self.getSubDirectories()
		self.m_lCbsProjectFile = self.createValuesSETList( "SUBDIRS", lSubDirs )
		self.writeCbsProjectFile()
	
	
	def    getSubDirectories(self):
		print "Subdirs %s:" % (self.m_Options.sSubDirs)
		if self.m_Options.sSubDirs == "":
			return files.subdirectoriesListRecursive(self.m_StartUpPath, False, 0, re.compile(r""), re.compile(r"(.*\.svn)|(.*CVS)"))
		else:
			return self.m_Options.sSubDirs.split()
	
	
	def    addOptions(self):
		CbsBase.addOptions(self)
		self.m_OptionParser.add_option("-s", "--subdirs", dest="sSubDirs", default="", 
							help="SUBDIRS: (Quoted) List of subdirectories. If left empty all immidiate directories are added.", metavar="SUBDIRS")

#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	NewProject(sys.argv)



