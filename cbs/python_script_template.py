#!/usr/bin/python

# ----------------------------------------------
# --- CPAF build / generate IDE files script ---
# ----------------------------------------------
# Script that make building project or generating IDE project files easy.
# Run build.py --help for options. 
# IMPORTANT: When building project libraries/executables you must follow the 
# convention the the project name in your cmake-file is identical to the 
# directory (leaf) name.

#import utils

import os
import sys
import re
import copy
import shutil
from path import path

def RunOsSystem(cmd, bDry, bPrintCmd):
    """ Runs the command using 'os.system()' but only if bDry is not set. """

    if bPrintCmd:
        print cmd
    if bDry :   
        return
    else    :   
        return os.system(cmd)


class	NewProject:
    #	*****************
    #	***	Constants ***
    #	*****************
       
    #	************
    #	***	Vars ***
    #	************
    m_CmdArgs			= []
    m_StartUpPath       = path("")
    m_sProjectName	    = ""
    m_Options           = []

    #	*******************
    #	***	Constructor	***
    #	*******************
    def	__init__(self, cmd_args):
        self.m_CmdArgs	            = copy.copy(cmd_args)
        self.m_StartUpPath          = path(os.getcwd())
        self.m_sProjectName         = self.m_StartUpPath.name


        #print "StartUpPath : %s \n" % ( self.m_StartUpPath )
        #print "ProjectName : %s \n" % ( self.m_sProjectName )
        #print "OS name     : %s \n" % ( os.name )
        

        # -----------------
        # - Parse options -
        # -----------------
       
        from optparse import OptionParser
        parser = OptionParser()
#        parser.add_option("-b", "--buildtype", dest="sBuildTypeAlias", default="release", 
#                        help="BUILDTYPE: release, debug", metavar="BUILDTYPE")

        (self.m_Options, args)  = parser.parse_args()
        
        
        
        

        self.notImplementedYet()
        sys.exit(0)
                
       #files.forceRemoveTree( self.m_sDocumentationDstDir )


    #	***********************
    #	***	build Functions ***
    #	***********************
    def    notImplementedYet(self):
        print "This script is not implemented yet ...\n"
   



#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
    NewProject(sys.argv)



