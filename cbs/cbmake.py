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
        print (cmd)
    if bDry :
        return
    else    :
        return os.system(cmd)


class	Build:
    #	*****************
    #	***	Constants ***
    #	*****************
    m_GeneratorNamesDict	 = {
                                'qtclin'       	: 'NO Generator',
                                'vc11_64'      	: 'Visual Studio 11 Win64',
                                'vc11'       	: 'Visual Studio 11',
                                'vc10_64'     	: 'Visual Studio 10 Win64',
                                'vc10'       	: 'Visual Studio 10',
                                'vc9'       	: 'Visual Studio 9 2008',
                                'vc8'       	: 'Visual Studio 8 2005',
                                'vc8_64'    	: 'Visual Studio 8 2005 Win64',
                                'vc7'       	: 'Visual Studio 7',
                                'vc71'      	: 'Visual Studio 7 .NET 2003',
                                'nmake'     	: 'NMake Makefiles',
                                'nmake8'     	: 'NMake Makefiles',
                                'nmake9'     	: 'NMake Makefiles',
                                'nmake10'     	: 'NMake Makefiles',
                                'nmake10_64'   	: 'NMake Makefiles',
                                'nmake11'     	: 'NMake Makefiles',
                                'nmake11_64'  	: 'NMake Makefiles',
                                'nmake12'     	: 'NMake Makefiles',
                                'nmake12_64'  	: 'NMake Makefiles',
                                'jom10'     	: 'NMake Makefiles JOM',
                                'jom12'     	: 'NMake Makefiles JOM',
                                'jom12_64'     	: 'NMake Makefiles JOM',
                                'make'      	: 'Unix Makefiles',
                                'ninja'         : 'Ninja',
                                'kdev'      	: 'KDevelop3',
                                'kdevmake'    	: 'KDevelop3 - Unix Makefiles',
                                'xcode'     	: 'xcode',
                                'cb'			: 'CodeBlocks - Unix Makefiles',
                                'eclip'     	: 'Eclipse CDT4 - Unix Makefiles'
                            }

                            # ""C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"" amd64
    m_GeneratorBatCmdDict	 = {
                                'qtclin'    : '',
                                'vc12_64'   : 'call "%VS120COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'vc12'      : 'call "%VS120COMNTOOLS%/vsvars32.bat"',
                                'vc11_64'   : 'call "%VS110COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'vc11'      : 'call "%VS110COMNTOOLS%/vsvars32.bat"',
                                'vc10_64'   : 'call "%VS100COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'vc10'      : 'call "%VS100COMNTOOLS%/vsvars32.bat"',
                                'vc9'       : 'call "%VS90COMNTOOLS%/vsvars32.bat"',
                                'vc8'       : 'call "%VS80COMNTOOLS%/vsvars32.bat"',
                                'vc8_64'    : 'call "%VS80COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'vc7'       : 'call "%VS70COMNTOOLS%/vsvars32.bat"',
                                'vc71'      : 'call "%VS71COMNTOOLS%/vsvars32.bat"',
                                'nmake'     : 'call "%VS90COMNTOOLS%/vsvars32.bat"',
                                'nmake8'    : 'call "%VS80COMNTOOLS%/vsvars32.bat"',
                                'nmake9'    : 'call "%VS90COMNTOOLS%/vsvars32.bat"',
                                'nmake10'   : 'call "%VS100COMNTOOLS%/vsvars32.bat"',
                                'nmake10_64': 'call "%VS100COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'nmake11'   : 'call "%VS110COMNTOOLS%/vsvars32.bat"',
                                'nmake11_64': 'call "%VS110COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'nmake12'   : 'call "%VS120COMNTOOLS%/vsvars32.bat"',
                                'nmake12_64': 'call "%VS120COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'jom10'   	: 'call "%VS100COMNTOOLS%/vsvars32.bat"',
                                'jom12'   	: 'call "%VS120COMNTOOLS%/vsvars32.bat"',
                                'jom12_64'  : 'call "%VS120COMNTOOLS%\\..\\..\\VC\\vcvarsall.bat" amd64',
                                'ninja'     : '',
                                'make'      : '',
                                'kdev'      : '',
                                'kdevmake'  : '',
                                'xcode'     : '',
                                'cb'      	: '',
                                'eclip'     : ''
                            }
    m_GeneratorTgtCmdDict	 = {
                                'qtclin'    : 'qtclin',
                                'vc11_64'   : 'vc11_64',
                                'vc11'      : 'vc11',
                                'vc10_64'   : 'vc10_64',
                                'vc10'      : 'vc10',
                                'vc9'       : 'vc9',
                                'vc8'       : 'vc8',
                                'vc8_64'    : 'vc8_64',
                                'vc7'       : 'vc7',
                                'vc71'      : 'vc71',
                                'nmake'     : 'nmake',
                                'nmake8'    : 'nmake',
                                'nmake9'    : 'nmake',
                                'nmake10'   : 'nmake',
                                'nmake10_64': 'nmake',
                                'nmake11'   : 'nmake',
                                'nmake11_64': 'nmake',
                                'nmake12'   : 'nmake',
                                'nmake12_64': 'nmake',
                                'jom10'   	: 'jom',
                                'jom12'   	: 'jom',
                                'jom12_64'  : 'jom',
                                'ninja'     : 'ninja',
                                'make'      : 'make',
                                'kdev'      : 'kdev',
                                'kdevmake'  : 'kdevmake',
                                'xcode'     : 'xcode',
                                'cb'      	: 'cb',
                                'eclip'     : 'eclip'
                            }

    m_GeneratorJobsSwitchDict    = {
                                'qtclin'    : '-j',
                                'vc11_64'   : '',
                                'vc11'      : '',
                                'vc10_64'   : '',
                                'vc10'      : '',
                                'vc9'       : '',
                                'vc8'       : '',
                                'vc8_64'    : '',
                                'vc7'       : '',
                                'vc71'      : '',
                                'nmake'     : '',
                                'nmake8'    : '',
                                'nmake9'    : '',
                                'nmake10'   : '',
                                'nmake10_64': '',
                                'nmake11'   : '',
                                'nmake11_64': '',
                                'nmake12'   : '',
                                'nmake12_64': '',
                                'jom10'     : '/J',
                                'jom12'     : '/J',
                                'jom12_64'  : '/J',
                                'make'      : '-j',
                                'ninja'     : '-j',
                                'kdev'      : '-j',
                                'kdevmake'  : '-j',
                                'xcode'     : '-j',
                                'cb'        : '-j',
                                'eclip'     : '-j'
                            }


    m_DefaultGeneratorDict	= { 'nt'        : 'nmake12',
                                'posix'     : 'ninja',
                                'osx'       : 'make'
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
    m_cmd_args			= []
    m_sCMakeGenerator	= ""
    m_StartUpPath       = path("")
    m_BuildCbsPath      = path("")
    m_BuildPath         = path("")
    m_LibsPath          = path("")
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
                            help="Select which generator you want use. Not all available on all systems. GENERATOR: qtclin(QT Creator Linux),vc12, vc12_64, vc11, vc11_64, vc10, vc10_64, vc9, vc8, vc8_64, vc71, vc7, nmake, nmake8, nmake9, nmake10, nmake10_64, nmake11, nmake11_64, nmake12, nmake12_64, jom10, jom12, jom12_64, make, kdev, kdevmake, xcode, cb(codeblocks), eclip(se)",
                            metavar="GENERATOR")
        parser.add_option("-b", "--buildtype",  dest="sBuildTypeAlias", default="release",
                            help="Select build type like release or debug. BUILDTYPE: release, debug",
                            metavar="BUILDTYPE" )
        parser.add_option("-d", "--cachevar",  dest="sCacheVar", default="",
                            help="Cache variable to add to cmake.",
                            metavar="CACHEVAR" )
        parser.add_option("-p", "--package-mode",  dest="sPackageMode", default="",
                            help="Package mode string. Can be anything you like. For example 'Full', 'Demo', etc. You need to test for these names in your .cbs files and add files to CPack accordingly. ",
                            metavar="PACKAGE_MODE" )
        parser.add_option("-l", "--libtype",    dest="sLibTypeAlias",   default="static",
                            help="Default library type to build. LIBTYPE: static, shared",
                            metavar="LIBTYPE" )
        parser.add_option("-t", "--target",     dest="sBuildTarget",    default="all",
                            help="Target to build. Only for makefiles. BUILDTARGET: 'all, clean, distclean, package' ... and all named cmake 'projects'. Use '-t help' to see all auto generated targets. ",
                            metavar="BUILDTARGET")
        parser.add_option("-x", "--cross-compile", dest="sXCompileAlias", default="no",
                            help="Select cross compiling: 'yes' or 'no'. Default 'no'",
                            metavar="CROSSCOMPILE")
        parser.add_option("-j", "--jobs", dest="sJobsCount", default="",
                            help="JOBS: Number of jobs to use when compiling",
                            metavar="JOBS")
        parser.add_option("--configfile", dest="sConfigFile", default="",
                            help="Explicit buildconfiguration file to use",
                            metavar="CONFIGFILE")
        parser.add_option("-v", "--verbose",  dest="sVerbose", default="0",
                            help="Print verbose status messages to stdout. VERBOSE: 0, 1",
                            metavar="VERBOSE")
        parser.add_option("-c", "--clean_cache",  dest="sCleanCache", default="0",
                            help="Clear CMake cache. CLEANCACHE: 0, 1",
                            metavar="CLEANCACHE")
        parser.add_option("-C", "--clean",  dest="sClean", default="0",
                            help="Clean CLEAN: 0 (no clean), c (normal clean) d (distclean. Remove build dir entirely) l (remove libs) o (remove object files) ",
                            metavar="CLEAN")

        (self.m_Options, args)  = parser.parse_args()


        # - Set build path -
        s = "%s_%s_%s" % (self.m_Options.sGeneratorAlias, self.m_Options.sBuildTypeAlias, self.m_Options.sLibTypeAlias )
        self.m_BuildCbsPath = self.m_StartUpPath / "build_cbs"
        self.m_BuildPath    = self.m_BuildCbsPath / s
        self.m_LibsPath     = self.m_BuildCbsPath / "libs"



#        print "sGeneratorAlias: %s" %    ( self.m_Options.sGeneratorAlias )
#        print "sBuildTypeAlias: %s" %    ( self.m_Options.sBuildTypeAlias )
#        print "sGenerator: %s" %         ( self.m_GeneratorNamesDict[self.m_Options.sGeneratorAlias] )
#        print "sBuildType: %s" %         ( self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias] )
#        print "sVerbose: '%s'" %           ( self.m_Options.sVerbose )
#        print "sCleanCache: '%s'" %        ( self.m_Options.sCleanCache )
#        print "sBuildTarget: %s" %       ( self.m_Options.sBuildTarget )

#        print "sConfigFile: %s" %       ( self.m_Options.sConfigFile )
#        print "Build Path: %s \n" %     ( self.m_BuildPath )
#        print "Libs Path: %s \n" %      ( self.m_LibsPath )


        self.prepareBuild()
        self.buildTarget()
        self.printQTCreatorCMakeCmdArgs()

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
                print ("Cleaning cache, buildpath: %s" % (sCacheFilePath))
                os.remove( sCacheFilePath )

        # -------------------------------------
        # --- Clean /distclean if requested ---
        # -------------------------------------
        if self.m_Options.sClean == "c":    # Execute 'make clean'
            print ("Running make clean ")
            self.m_Options.sBuildTarget = "clean"
        elif self.m_Options.sClean == "d":  # Distclean. I.e. remove ./build_cbs library entirely
            self.m_Options.sBuildTarget = "distclean"
        elif self.m_Options.sClean == "l":  # Libraries remove
            if os.path.exists(self.m_LibsPath):
                print ("Deleting libs directory: '%s' ..." % (self.m_LibsPath))
                shutil.rmtree(self.m_LibsPath)
        elif self.m_Options.sClean == "o":  # Object files remove
            if os.path.exists(self.m_BuildPath):
                print ("Deleting object files directory: '%s' ..." % (self.m_BuildPath))
                shutil.rmtree(self.m_BuildPath)


    #	***********************
    #	***	build Functions ***
    #	***********************
    def    createBuildFiles(self):
        print ("Creating buildfiles ...\n")
        saveCWD = os.getcwd()       # Save working directory
        self.m_BuildPath.makedirs(1)
        os.chdir( self.m_BuildPath )
        cmd = self.makeCreateBuildFilesCmd()
        runOsSystem(cmd, 0, 1)
        self.deleteCreateBuildFilesCmd()
        os.chdir(saveCWD)           # Restore working directory


    def    makeCreateBuildFilesCmd(self):
        sCacheVars = self.getCacheVars()
        sPackageMode = self.getPackageMode()
        cmd = 'cmake %s %s -D CMAKE_BUILD_TYPE:STRING=%s -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=%s -D CBS_BUILD_CONFIG_FILE:STRING=%s -D CBS_CROSS_COMPILING:STRING=%s -G "%s" ../..' % (sCacheVars, sPackageMode, self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias], self.m_LibTypeDict[self.m_Options.sLibTypeAlias], self.m_Options.sConfigFile, self.m_XCompileDict[self.m_Options.sXCompileAlias], self.m_GeneratorNamesDict[self.m_Options.sGeneratorAlias] )
        if self.m_GeneratorBatCmdDict[self.m_Options.sGeneratorAlias] != '':
            outFile = open('CreateBuildFilesCmd.bat', 'w')
            outFile.write( "%s\n" % (self.m_GeneratorBatCmdDict[self.m_Options.sGeneratorAlias]) )
            outFile.write( "%s\n" % (cmd) )
            cmd = "CreateBuildFilesCmd.bat"
        print ("makeCreateBuildFilesCmd CMD: %s" % (cmd))
        return  cmd

#    def    printQTCreatorCMakeCmdArgs(self):
#        if ( self.m_Options.sGeneratorAlias == "qtclin" ):
#            cmdArgs = '-D CMAKE_BUILD_TYPE:STRING=%s -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=%s -D CBS_BUILD_CONFIG_FILE:STRING=%s -D CBS_CROSS_COMPILING:STRING=%s ../..' % (self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias], self.m_LibTypeDict[self.m_Options.sLibTypeAlias], self.m_Options.sConfigFile, self.m_XCompileDict[self.m_Options.sXCompileAlias] )
#            print "QTCreator arguments to cmake:\n%s\n" % ( cmdArgs )

    def    printQTCreatorCMakeCmdArgs(self):
        cmdArgs = '-D CMAKE_BUILD_TYPE:STRING=%s -D CBS_BUILD=ON -D CBS_LIB_TYPE_TO_BUILD:STRING=%s -D CBS_BUILD_CONFIG_FILE:STRING=%s -D CBS_CROSS_COMPILING:STRING=%s ../..' % (self.m_BuildTypeDict[self.m_Options.sBuildTypeAlias], self.m_LibTypeDict[self.m_Options.sLibTypeAlias], self.m_Options.sConfigFile, self.m_XCompileDict[self.m_Options.sXCompileAlias] )
        print ("QTCreator arguments to cmake:\n%s\n" % ( cmdArgs ))

    def   deleteCreateBuildFilesCmd(self):
        pass

    def    buildTarget(self):
        # Set build options
        if self.m_Options.sVerbose == "1":  self.m_sVerboseCmd       = "VERBOSE=1 "

        if self.m_Options.sBuildTarget == "distclean":
            print ("Distclean: Removing './build_cbs' directory entirely ...\n")
            if os.path.exists(self.m_BuildCbsPath):
                shutil.rmtree(self.m_BuildCbsPath)
            sCMakeListsTxtUser =  "%s/CMakeLists.txt.user" % (self.m_StartUpPath)
            if os.path.exists(sCMakeListsTxtUser):
                os.remove( sCMakeListsTxtUser )

        elif self.m_Options.sBuildTarget != "none":
            self.createBuildFiles()
            print ("Building target ...\n")
            saveCWD = os.getcwd()       # Save working directory
            os.chdir( self.m_BuildPath )
            cmd = self.makeBuildTargetCmd()
            runOsSystem(cmd, 0, 1)
            self.deleteBuildTargetCmd()
            os.chdir(saveCWD)           # Restore working directory

    def    makeBuildTargetCmd(self):
        jobsSwitch = "";
        print ("Jobs count : %s" % (self.m_Options.sJobsCount))
        if self.m_Options.sJobsCount != "":
            jobsSwitch = "%s %s" % (self.m_GeneratorJobsSwitchDict[self.m_Options.sGeneratorAlias], self.m_Options.sJobsCount);


        cmd = '%s%s %s %s' % (self.m_sVerboseCmd, self.m_GeneratorTgtCmdDict[self.m_Options.sGeneratorAlias], jobsSwitch, self.m_Options.sBuildTarget )
        if self.m_GeneratorBatCmdDict[self.m_Options.sGeneratorAlias] != '':
            outFile = open('BuildTargetCmd.bat', 'w')
            outFile.write( "%s\n" % (self.m_GeneratorBatCmdDict[self.m_Options.sGeneratorAlias]) )
            outFile.write( "%s\n" % (cmd) )
            cmd = "BuildTargetCmd.bat"
        print ("makeBuildTargetCmd CMD: %s" % (cmd))
        return  cmd

    def   deleteBuildTargetCmd(self):
        pass


    #   ************************
    #   *** Helper Functions ***
    #   ************************
    def    getCacheVars(self):
        sCacheVars = ""
        if self.m_Options.sCacheVar != "":
            sCacheVars = "-D %s" % (self.m_Options.sCacheVar)
        return sCacheVars

    def    getPackageMode(self):
        sPackageMode = "-D PACKAGE_MODE:STRING=''"
        if self.m_Options.sPackageMode != "":
            sPackageMode = "-D PACKAGE_MODE:STRING=%s" % (self.m_Options.sPackageMode)
        return sPackageMode

    # --- Clean cache  ---
    def    cleanCache(self):
        sCacheFilePath =  "%s/CMakeCache.txt" % (self.m_BuildPath)
        if os.path.exists(sCacheFilePath):
            os.remove( sCacheFilePath )


#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
    Build(sys.argv)



