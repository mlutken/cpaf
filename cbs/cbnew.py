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

def runOsSystem(cmd, bDry, bPrintCmd):
    """ Runs the command using 'os.system()' but only if bDry is not set. """

    if bPrintCmd:
        print cmd
    if bDry :   
        return
    else    :   
        return os.system(cmd)


class	Build:
    #	*****************
    #	***	Constants ***
    #	*****************
    m_GeneratorNamesDict	 = {'vc8'       : 'Visual Studio 8 2005', 
                                'vc8_64'    : 'Visual Studio 8 2005 Win64', 
                                'vc7'       : 'Visual Studio 7', 
                                'vc71'      : 'Visual Studio 7 .NET 2003', 
                                'nmake'     : 'NMake Makefiles',
                                'make'      : 'Unix Makefiles',
                                'kdev'      : 'KDevelop3',
                                'xcode'     : 'xcode' 
                              }

    m_DefaultGeneratorDict	= { 'nt'        : 'nmake', 
                                'posix'     : 'make', 
                                'osx'       : 'xcode'
                              }
    	
    m_BuildTypeDict         = { 'release'   : 'Release', 
                                'debug'     : 'Debug' 
                              }

    m_LibTypeDict           = { 'static'    : 'STATIC', 
                                'shared'    : 'SHARED' 
                              }
    
    m_XCompileDict          = { 'no'        : 'NO', 
                                'yes'       : 'YES' 
                              }
       
    #	************
    #	***	Vars ***
    #	************
    m_cmd_args			    = []
    m_sCMakeGenerator		= ""
    m_StartUpPath       = path("")
    m_BuildPath         = path("")
    m_sProjectName      = ""
    m_sVerboseCmd       = ""
    m_Options           = []

    #	*******************
    #	***	Constructor	***
    #	*******************
    def	__init__(self, cmd_args):
        self.m_cmd_args	            = copy.copy(cmd_args)
        self.m_StartUpPath          = path(os.getcwd())
        self.m_sProjectName         = self.m_StartUpPath.name

        #utils.CheckAndRemoveArg(self.m_cmd_args, sys.argv[0])	#	Remove script	name from	cmd_args


#        print "StartUpPath : %s \n" % ( self.m_StartUpPath )
#        print "ProjectName : %s \n" % ( self.m_sProjectName )
#        print "OS name     : %s \n" % ( os.name )
        

        # -----------------
        # - Parse options -
        # -----------------
       
        from optparse import OptionParser
        parser = OptionParser()
        parser.add_option("-g", "--generator", dest="sGeneratorAlias", default=self.m_DefaultGeneratorDict[os.name], 
                            help="Select which generator you want use. Not all available on all systems. GENERATOR: vc8, vc8_64, vc71, vc7, nmake, make, kdev, xcode", 
                            metavar="GENERATOR")
        parser.add_option("-b", "--buildtype",  dest="sBuildTypeAlias", default="release", 
                            help="Select build type like release or debug. BUILDTYPE: release, debug",           
                            metavar="BUILDTYPE" )
        parser.add_option("-l", "--libtype",    dest="sLibTypeAlias",   default="static", 
                            help="Default library type to build. LIBTYPE: static, shared", 
                            metavar="LIBTYPE" )
        parser.add_option("-t", "--target",     dest="sBuildTarget",    default="all",
                            help="Target to build. Only for makefiles. BUILDTARGET: 'all, clean, distclean, help' ... and all named cmake 'projects'. Use '-t help' to see all auto generated targets. ", 
                            metavar="BUILDTARGET")
        parser.add_option("-x", "--cross-compile", dest="sXCompileAlias", default="no",
                            help="Select cross compiling: 'yes' or 'no'. Default 'no'", 
                            metavar="CROSSCOMPILE")
        parser.add_option("--configfile", dest="sConfigFile", default="",
                            help="Explicit buildconfiguration file to use", 
                            metavar="CONFIGFILE")
        parser.add_option("-v", "--verbose",  dest="sVerbose", default="0",
                            help="Print verbose status messages to stdout. VERBOSE: 0, 1", 
                            metavar="VERBOSE")
        parser.add_option("-c", "--clean_cache",  dest="sCleanCache", default="0",
                            help="NOT implemented yet! Clear CMake cache. CLEANCACHE: 0, 1",
                            metavar="CLEANCACHE")
        parser.add_option("-C", "--clean",  dest="sClean", default="0",
                            help="Clean CLEAN: 0 (no clean), c (normal clean) d (distclean. Remove build dir entirely) l (remove libs) o (remove object files) ",
                            metavar="CLEAN")

        (self.m_Options, args)  = parser.parse_args()
        
        
        # - Set build path -
        s = "%s_%s" % (self.m_Options.sGeneratorAlias, self.m_Options.sBuildTypeAlias )
        self.m_BuildPath = self.m_StartUpPath / "build" / s
        
        
        
#        print "sGeneratorAlias: %s" %    ( self.m_Options.sGeneratorAlias )
#        print "sBuildTypeAlias: %s" %    ( self.m_Options.sBuildTypeAlias )
#        print "sGenerator: %s" %         ( self.m_GeneratorNamesDict[self.m_Options.sGeneratorAlias] )
#        print "sBuildType: %s" %         ( self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias] )
#        print "sVerbose: '%s'" %           ( self.m_Options.sVerbose )
#        print "sCleanCache: '%s'" %        ( self.m_Options.sCleanCache )
#        print "sBuildTarget: %s" %       ( self.m_Options.sBuildTarget )
        
        print "sConfigFile: %s" %       ( self.m_Options.sConfigFile )
        print "Build Path: %s \n" %        ( self.m_BuildPath )
        
        
        self.prepareBuild()
        self.buildTarget()

        sys.exit(0)
                

    #   *******************************
    #   *** Prepare Build Functions ***
    #   *******************************
    def    prepareBuild(self):
        # --------------------------------
        # --- Clean cache if requested ---
        # --------------------------------
        if self.m_Options.sCleanCache != "0":
            sCacheFilePath =  "%s/CMakeCache.txt" % (self.m_BuildPath)  
            if os.path.exists(sCacheFilePath):
                print "Cleaning cache, buildpath: %s" % (sCacheFilePath) 
                os.remove( sCacheFilePath )
        
        # -------------------------------------
        # --- Clean /distclean if requested ---
        # -------------------------------------
        if self.m_Options.sClean == "c":    # Execute 'make clean'
            print "Running make clean "
            self.m_Options.sBuildTarget = "clean"
        elif self.m_Options.sClean == "d":  # Distclean. I.e. remove ./build library entirely
            self.m_Options.sBuildTarget = "distclean"
        elif self.m_Options.sClean == "l":  # Libraries remove
            sLibsDir = "./build/libs"
            if os.path.exists(sLibsDir):
                print "Deleting libs directory: '%s' ..." % (sLibsDir) 
                shutil.rmtree(sLibsDir)  
        elif self.m_Options.sClean == "o":  # Object files remove
            if os.path.exists(self.m_BuildPath):
                print "Deleting object files directory: '%s' ..." % (self.m_BuildPath) 
                shutil.rmtree(self.m_BuildPath)  


    #	***********************
    #	***	build Functions ***
    #	***********************
    def    createBuildFiles(self):
        print "Creating buildfiles ...\n"
        saveCWD = os.getcwd()       # Save working directory
        self.m_BuildPath.makedirs(1)
        os.chdir( self.m_BuildPath )
        cmd = 'cmake -D CMAKE_BUILD_TYPE:STRING=%s -D CBS_LIB_TYPE_TO_BUILD:STRING=%s -D CBS_BUILD_CONFIG_FILE:STRING=%s -D CBS_CROSS_COMPILING:STRING=%s -G "%s" ../..' % (self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias], self.m_LibTypeDict[self.m_Options.sLibTypeAlias], self.m_Options.sConfigFile, self.m_XCompileDict[self.m_Options.sXCompileAlias], self.m_GeneratorNamesDict[self.m_Options.sGeneratorAlias] )
        runOsSystem(cmd, 0, 1)
        os.chdir(saveCWD)           # Restore working directory
   
    def    buildTarget(self):
        # Set build options
        if self.m_Options.sVerbose == "1":  self.m_sVerboseCmd       = "VERBOSE=1 "
        
        if self.m_Options.sBuildTarget == "distclean":
            print "Distclean: Removing './build' directory entirely ...\n"
            if os.path.exists("./build"):
                shutil.rmtree("./build")  
        elif self.m_Options.sBuildTarget != "none":
            self.createBuildFiles()
            print "Building target ...\n"
            saveCWD = os.getcwd()       # Save working directory
            os.chdir( self.m_BuildPath )
            cmd = '%s%s %s' % (self.m_sVerboseCmd, self.m_Options.sGeneratorAlias, self.m_Options.sBuildTarget )
            runOsSystem(cmd, 0, 1)
            os.chdir(saveCWD)           # Restore working directory
    

    #   ************************
    #   *** Helper Functions ***
    #   ************************

    # --- Clean cache  ---
    def    cleanCache(self):
        sCacheFilePath =  "%s/CMakeCache.txt" % (self.m_BuildPath)  
        if os.path.exists(sCacheFilePath):
            os.remove( sCacheFilePath )


#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
    Build(sys.argv)



