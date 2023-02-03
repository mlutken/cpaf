#!/usr/bin/python

# ----------------------------------------------------
# --- CPAF automake configure h.in files converter ---
# ----------------------------------------------------


import os
import sys
import re
import copy
import shutil
from path import path

g_sConfigFileTop = "" \
"# ----------------------------------------------------------------------\n" \
"# --- Common settings. (Re)Move to platform specific block if needed ---\n" \
"# ----------------------------------------------------------------------\n" \
"\n" 

g_sConfigFileBottom = "\n" 		\
"# -------------------------\n" \
"# --- Platform specific ---\n" \
"# -------------------------\n" \
"\n" \
"# *** WINDOWS ***\n" \
"IF (    WIN32 )\n" \
"ENDIF ( WIN32 )\n" \
"\n" \
"# *** UNIX ***\n" \
"IF (    UNIX )\n" \
"\n" \
"	# *** LINUX ***\n" \
"   IF (    CMAKE_SYSTEM_NAME MATCHES \"Linux\" )\n" \
"    ENDIF ( CMAKE_SYSTEM_NAME MATCHES \"Linux\" )\n" \
"\n" \
"	# *** DARWIN (MAC OSX) ***\n" \
"    IF (   CMAKE_SYSTEM_NAME MATCHES \"Darwin\" )\n" \
"    ENDIF( CMAKE_SYSTEM_NAME MATCHES \"Darwin\" )\n" \
"ENDIF ( UNIX )\n" \
"\n"	\
"ADD_PACKAGE_DEFINES ()	# 1) Adds VERSION, PACKAGE, PACKAGE_NAME, PACKAGE_STRING, PACKAGE_TARNAME, PACKAGE_VERSION, PACKAGE_BUGREPORT to commandline defines.\n"	\
"\n" \
"\n" \
"# COMMENTS:\n" \
"# 1) VERSION, PACKAGE, PACKAGE_NAME, PACKAGE_STRING, PACKAGE_TARNAME, PACKAGE_VERSION\n" \
"#    are all handled by the SET_DEFAULT_VERSION macro and then added to the command \n" \
"#    line defines by the macro ADD_PACKAGE_DEFINES() in the top of this config file.\n" \
"#    ALSO: PACKAGE_BUGREPORT are set by ADD_PACKAGE_DEFINES().\n" \
"#    Since these are very general names we don't want then in the configured file which\n" \
"#    might be included indirectly by other libraries/programs using this library. Therefore\n" \
"#    the ADD_PACKAGE_DEFINES macro to put them all on the commandline.\n" \
"#    SO BOTTOM LINE: You most often just want to remove these uncommented lines !!!\n" \
"#    NOTE: Sometimes the ADD_PACKAGE_DEFINES() are added in the main .cbs file \n" \
"#    instead of in the config file. \n" \
"\n" \
"\n"
		


g_sConfigInFileBottom = "\n" 		\
"/* COMMENTS:\n" \
" 1) VERSION, PACKAGE, PACKAGE_NAME, PACKAGE_STRING, PACKAGE_TARNAME, PACKAGE_VERSION\n" \
"    are all handled by the SET_DEFAULT_VERSION macro and then added to the command \n" \
"    line defines by the macro ADD_PACKAGE_DEFINES() in the top of this config file.\n" \
"    ALSO: PACKAGE_BUGREPORT are set by ADD_PACKAGE_DEFINES().\n" 	\
"    Since these are very general names we don't want then in the configured file which\n" \
"    might be included indirectly by other libraries/programs using this library. Therefore\n" \
"    the ADD_PACKAGE_DEFINES macro to put them all on the commandline.\n" \
"    SO BOTTOM LINE: You most often just want to remove these uncommented lines !!!\n" \
"    NOTE: Sometimes the ADD_PACKAGE_DEFINES() are added in the main .cbs file \n" \
"    instead of in the config file. \n" \
"*/\n" \
"\n"

g_lPACKAGE_DEFINES	= ["VERSION", "PACKAGE", "PACKAGE_NAME", "PACKAGE_STRING", "PACKAGE_TARNAME", "PACKAGE_VERSION" ]

def runOsSystem(cmd, bDry, bPrintCmd):
	""" Runs the command using 'os.system()' but only if bDry is not set. """

	if bPrintCmd:
		print cmd
	if bDry :   
		return
	else    :   
		return os.system(cmd)

class	ConfigSect:
	m_sSect					= ""	# The whole section as one sstring
	m_sSectType				= ""	# '', 'SimpleDefine'
	m_sCommentBlock			= ""	# Comment block string e.g.: Define to 1 if the system has the type int16_t 
	m_sDefineName			= ""	# Name of SimpleDefine like 'HAVE_INT16_T'
	m_sDefineValue	= ""	# Value of SimpleDefine like '1'
	def commentBlockCStyle(self):
		return "/* " + self.m_sCommentBlock + " */"
	def commentBlockCMakeStyle(self):
		return "#" + self.m_sCommentBlock.replace( "\n", " " )
#	def	__init__(self):
#		pass


class	CbsConfigConverter:
	#	************
	#	***	Vars ***
	#	************
	m_cmd_args				= []
	m_StartUpPath       	= path("")
	m_sCbsPath       		= path("")
	m_TemplatesPath         = path("")
	m_sVerboseCmd       	= ""
	m_Options           	= []
	m_aSections 			= []	# Sections read in from the xxx.h.in file
	m_aSectionsDefault 		= []	# Section read in from the xxx.h file (i.e. the configured by 'configure' header file if it exists.
	m_sVERSION_MAJOR		= "X"
	m_sVERSION_MINOR		= "X"
	m_sVERSION_MICRO		= "X"

	#	*******************
	#	***	Constructor	***
	#	*******************
	def	__init__(self, cmd_args):
		self.m_cmd_args	            = copy.copy(cmd_args)
		self.m_StartUpPath          = path(os.getcwd())
		self.m_sCbsPath 			= path(sys.argv[0]).parent
		self.m_TemplatesPath		= self.m_sCbsPath / path("templates")
		
		print "m_StartUpPath   : %s" % 	( self.m_StartUpPath 	)
		print "m_sCbsPath      : %s" % 	( self.m_sCbsPath 		)
		print "m_TemplatesPath : %s" % 	( self.m_TemplatesPath 	)
		
		# -----------------
		# - Parse options -
		# -----------------
	
		from optparse import OptionParser
		parser = OptionParser()
		parser.add_option("-f", "--file-in",     dest="sOrigHeaderFileDotIn",    default="config.h.in",
							help="Name of prj_config.h.in file to convert.", 
							metavar="TYPE")
		parser.add_option("-n", "--name",     dest="sName",    default="auto",
							help="Name of target/project. DEFAULT: 'auto' which means that current leaf directory name is used.", 
							metavar="NAME")
		parser.add_option("-d", "--dry",     dest="sDry",    default="0",
							help="Run dry, no changes. DEFAULT: '0'.", 
							metavar="DRY")
		(self.m_Options, args)  = parser.parse_args()
		
		# Print some info about commmand input
		print "sDry    : %s" %        ( self.m_Options.sDry 	)
		
		# Process user options input further
		if self.m_Options.sName == "auto":
			sList	= self.m_StartUpPath.name.split( '-' )
			self.m_Options.sName = sList[0]
			sVersion = sList[1]
			sList	= sVersion.split( '.' )
			if len(sList) == 3:
				self.m_sVERSION_MAJOR	= sList[0]
				self.m_sVERSION_MINOR	= sList[1]
				self.m_sVERSION_MICRO	= sList[2]
				##print "VERSION (%s %s %s)" % (self.m_sVERSION_MAJOR, self.m_sVERSION_MINOR, self.m_sVERSION_MICRO)  
						
			##print "NAME %s" % (self.m_Options.sName)  
			##print "VERSION %s" % (sVersion)  
			
		
		self.m_bDry = int(self.m_Options.sDry)	
		if self.m_Options.sOrigHeaderFileDotIn == "":	
			print "Error. Must specify file with -f option!"
			sys.exit(0)
		
		self.convertConfigFile( self.m_Options.sOrigHeaderFileDotIn, self.m_Options.sName )

		sys.exit(0)
				

	#   *****************
	#   *** Functions ***
	#   *****************

	
	def convertConfigFile(self, sOrigHeaderFileDotIn, sPrjTgtName ):
		self.setConfigFileNames ( sOrigHeaderFileDotIn, sPrjTgtName  )
		self.readOrigHeaderFileDotIn()
		self.readOrigHeaderFile()
		self.readAmCommonDefines()
#		for sct in self.m_aSections: print sct.m_sSect
		
		#for sct in self.m_aSections:
			#if sct.m_sSectType == "SimpleDefine":
				#print "Simple Define: '%s' " % ( sct.m_sDefineName )
		#print "\n"
		self.removeAmDefinesFromSectionsList()
		self.removeAmDefinesFromSectionsDefaultList()
		self.writeCbsHeaderDotInFile()
		self.writeCbsConfigFile()
		
#		for sct in self.m_aSections: print sct.m_sSect
		
		#for sVar in self.m_aAmVars:
			#print "AM VAR: '%s'" % ( sVar )
		#for sct in self.m_aSections:
			#if sct.m_sSectType == "SimpleDefine":
				#print "Simple Define: '%s' == '%s' " % ( sct.m_sDefineName, sct.m_sDefineValue )

	#   ***********************
	#   *** Write Functions ***
	#   ***********************
	def   writeCbsHeaderDotInFile(self):
		pass
		of = open(self.m_sCbsHeaderDotInFile , "w")
		bWrittenAmCommonDefinesInclude = False
		for sct in self.m_aSections:
			if sct.m_sSectType == "SimpleDefine" or sct.m_sSectType == "StringDefine":
				sct = self.getSectFromDefaultList( sct )
			
			if sct.m_sSectType == "SimpleDefine":
				if not bWrittenAmCommonDefinesInclude:
					bWrittenAmCommonDefinesInclude = True
					of.write( "#include <am_common_defines.h>\n\n" )
				of.write( sct.commentBlockCStyle() + "\n")
				of.write( "#cmakedefine %s @%s@\n\n" % (sct.m_sDefineName, sct.m_sDefineName) )
			elif sct.m_sSectType == "StringDefine":
				if not bWrittenAmCommonDefinesInclude:
					bWrittenAmCommonDefinesInclude = True
					of.write( "#include <am_common_defines.h>\n\n" )
				sCommentOut1 = ""
				if sct.m_sDefineName in g_lPACKAGE_DEFINES or sct.m_sDefineName == "PACKAGE_BUGREPORT":	
					sCommentOut1 = "// 1) "
				of.write( "%s%s\n" % (sCommentOut1, sct.commentBlockCStyle()) )
				of.write( '%s#cmakedefine %s "@%s@"\n\n' % (sCommentOut1, sct.m_sDefineName, sct.m_sDefineName) )
			else:
				of.write(sct.m_sSect + "\n")
		of.write(g_sConfigInFileBottom)	# Make sure we end with a newline to avoid compiler warnings
		
	def   writeCbsConfigFile(self):
		pass
		of = open(self.m_sCbsConfigFile , "w")
		of.write(g_sConfigFileTop)
		of.write ( "SET_DEFAULT_VERSION ( %s %s %s )\n" % (self.m_sVERSION_MAJOR, self.m_sVERSION_MINOR, self.m_sVERSION_MICRO) )
		of.write ( "ASSIGN_DEFAULT ( _INSTALL_PREFIX ${CBS_DEFAULT_INSTALL_PREFIX} )\n\n" )
		
		for sct in self.m_aSectionsDefault:
			if sct.m_sSectType == "SimpleDefine":
				if sct.m_sDefineValue == ''	:	sValue = '""'
				else						:	sValue = sct.m_sDefineValue
				of.write( "SET_DEFAULT ( %s\t\t%s\t) %s\n" % (sct.m_sDefineName, sValue, sct.commentBlockCMakeStyle()) )
			elif sct.m_sSectType == "StringDefine":
				sCommentOut1 = ""
				if sct.m_sDefineName in g_lPACKAGE_DEFINES:	sCommentOut1 = "# 1) "
				of.write( '%sSET_DEFAULT ( %s\t\t"%s"\t) %s\n' % (sCommentOut1, sct.m_sDefineName, sct.m_sDefineValue, sct.commentBlockCMakeStyle()) )
			else:
				pass
				#of.write(sct.m_sSect + "\n")
		of.write( g_sConfigFileBottom )	
	
	#   **********************
	#   *** Read Functions ***
	#   **********************
	def   readOrigHeaderFile(self):
		""" Find default values for the defines from original header 
			file created from configure"""
		if not os.path.exists(self.m_sOrigHeaderFile): return
		#commentBlockScan	= re.compile( r"\A/\*(.*)\*/" )	# Matches comment block       : /* ...ANYTHING_HERE... */
		undefScan		= re.compile( r"/\*\s*#undef\s*(\w+)\s*\*/" )	# Matches undef               : /* #undef DEFINE_NAME */   
		defScanVal		= re.compile( r"#define\s+(\w+)\s+(\w+)"	)	# Matches define with value   : #define DEFINE_NAME VALUE
		defScanStrVal	= re.compile( r'#define\s+(\w+)\s+"(.*)"$'	)	# Matches define with value   : #define DEFINE_NAME "VALUE"
		defScan			= re.compile( r"#define\s+(\w+)\s+" 		)	# Matches define with no value: #define DEFINE_NAME
		ifNDefScan		= re.compile( r"#ifndef\s+(\w+)\s+" 		)	# Matches define with no value: #define DEFINE_NAME
		self.m_aSectionsDefault = []
		asLinesIn = open(self.m_sOrigHeaderFile, "r").readlines()
		i = 0
		while i < len(asLinesIn): 
			while i < len(asLinesIn) and asLinesIn[i] == "\n":
				i = i + 1
			sect = ConfigSect()		
			while i < len(asLinesIn) and asLinesIn[i] != "\n":
				sect.m_sSect += asLinesIn[i]
				i = i + 1
			
			sect.m_sCommentBlock = self.getCommentBlock ( sect.m_sSect ) 
					
			# --- Get default define values --- 
			n = i - 1
			matchUndef 		= undefScan.match  ( asLinesIn[n] )	# Check for '/* #undef DEFINE_NAME */'    on last line of section
			matchDefVal 	= defScanVal.match ( asLinesIn[n] )	# Check for '#define DEFINE_NAME VALUE'   on last line of section
			matchDefStrVal 	= defScanStrVal.match ( asLinesIn[n] )	# Check for '#define DEFINE_NAME "VALUE"' on last line of section
			matchDef 		= defScan.match    ( asLinesIn[n] )	# Check for '#define DEFINE_NAME'         on last line of section
			if matchUndef: 
				sect.m_sSectType 			= "SimpleDefine"
				sect.m_sDefineName 		= matchUndef.group(1)  
				sect.m_sDefineValue 	= ""  
			elif matchDefStrVal: 
				sect.m_sSectType 			= "StringDefine"
				sect.m_sDefineName 		= matchDefStrVal.group(1)  
				sect.m_sDefineValue 	= matchDefStrVal.group(2)  
			elif matchDefVal: 
				#print "SimpleDefine: %s" % (asLinesIn[n]) 
				sect.m_sSectType 			= "SimpleDefine"
				sect.m_sDefineName 		= matchDefVal.group(1)  
				sect.m_sDefineValue 	= matchDefVal.group(2)  
			elif matchDef:
				##print "LINE: %s" % (asLinesIn[n]) 
				sect.m_sSectType 			= "SimpleDefine"
				sect.m_sDefineName 		= matchDef.group(1)  
				matchIfNDef = ifNDefScan.match    ( asLinesIn[n-1] )	# Check for '#indef DEFINE_NAME'       on line above this
				if matchIfNDef:
					sect.m_sSectType		= "IncludeGuard"
			else:
				#print "readOrigHeaderFile: NO MATCH: %s " % ( asLinesIn[n])  	
				pass
			self.m_aSectionsDefault.append(sect)
	
	
	def   readOrigHeaderFileDotIn(self):
		undefScan		= re.compile( r"#undef\s*(\w+)\s*" 		)
		self.m_aSections = []
		asLinesIn = open(self.m_sOrigHeaderFileDotIn, "r").readlines()
		i = 0
		while i < len(asLinesIn): 
			while i < len(asLinesIn) and asLinesIn[i] == "\n":
				i = i + 1
			sect = ConfigSect()		
			while i < len(asLinesIn) and asLinesIn[i] != "\n":
				sect.m_sSect += asLinesIn[i]
				i = i + 1
			sect.m_sCommentBlock = self.getCommentBlock ( sect.m_sSect ) 
			matchUndef 	= undefScan.match( asLinesIn[i-1] )	# Check for simple define (ie. #UNDEF ) on last line of section
			if matchUndef: 
				sect.m_sSectType 			= "SimpleDefine"
				sect.m_sDefineName 		= matchUndef.group(1)  
				sect.m_sDefineValue 	= "1"  
			self.m_aSections.append(sect)
			
	
	def   readAmCommonDefines(self):
		haveVarScan		= re.compile( r"ADD_AM_HAVE_VAR\s*\(\s*(\w+)\s*\)" 		)
		stdVarScan		= re.compile( r"ADD_AM_STANDARD_VAR\s*\(\s*(\w+)\s*\)" 	)
		sizeofVarScan	= re.compile( r"ADD_AM_SIZEOF_VAR\s*\(\s*(\w+)\s*\)" 	)
		
		self.m_aAmVars = []
		asLinesIn = open(self.m_sAmCommonDefinesFile, "r").readlines()
		for sLine in asLinesIn: 
			matchHaveVar 	= haveVarScan.match( sLine )
			matchStdVar 	= stdVarScan.match( sLine )
			matchSizeofVar 	= sizeofVarScan.match( sLine )
			if  matchHaveVar:
				self.m_aAmVars.append( matchHaveVar.group(1) )
			elif matchStdVar:
				self.m_aAmVars.append( matchStdVar.group(1) )
			elif matchSizeofVar:
				self.m_aAmVars.append( matchSizeofVar.group(1) )
	
	
	def   removeAmDefinesFromSectionsList(self):
		aSections = []
		for sct in self.m_aSections:
			if sct.m_sSectType == "SimpleDefine":
				if  not self.inAmVarsList ( sct.m_sDefineName ):
					aSections.append ( sct ) 
			else:
				aSections.append ( sct ) 
				
		self.m_aSections = aSections
	
	def   removeAmDefinesFromSectionsDefaultList(self):
		aSections = []
		for sct in self.m_aSectionsDefault:
			if sct.m_sSectType == "SimpleDefine":
				if  not self.inAmVarsList ( sct.m_sDefineName ):
					aSections.append ( sct ) 
			else:
				aSections.append ( sct ) 
		self.m_aSectionsDefault = aSections
	

	#   ************************
	#   *** Helper Functions ***
	#   ************************
	def   setConfigFileNames(self, sOrigHeaderFileDotIn, sPrjTgtName  ):
		self.m_sPrjTgtName			=  sPrjTgtName	
		self.m_sAmCommonDefinesFile	=  self.m_sCbsPath / path("am_common_defines.cbs")	
		self.m_sOrigHeaderFileDotIn	=  sOrigHeaderFileDotIn	# This file we read from disk
		self.m_sOrigHeaderFile		=  sOrigHeaderFileDotIn.replace( ".in", "")	# This file we read from disk. Contains default values for the configure vars in case we have run configure :-)
		self.m_sCbsHeaderDotInFile	=  "cbs_%s" % (sOrigHeaderFileDotIn)	# This we create/write	
		self.m_sCbsConfigFile		=  "%s_config.cbs" % (sPrjTgtName)		# This we create/write	
		print "self.m_sAmCommonDefinesFile: %s" % self.m_sAmCommonDefinesFile
		print "self.m_sOrigHeaderFileDotIn: %s" % self.m_sOrigHeaderFileDotIn
		print "self.m_sOrigHeaderFile     : %s" % self.m_sOrigHeaderFile
		print "self.m_sCbsHeaderDotInFile : %s" % self.m_sCbsHeaderDotInFile
		print "self.m_sCbsConfigFile      : %s" % self.m_sCbsConfigFile
	
	def   inAmVarsList(self, sVarName  ):
		for sVar in self.m_aAmVars:
			#print "TEST: '%s'=='%s'" % ( sVar, sVarName )
			if sVar == sVarName:
				return True
		return False

	def   getSectFromDefaultList(self, sect  ):
		for sct in self.m_aSectionsDefault:
			if sct.m_sDefineName == sect.m_sDefineName:
				return sct
		return sect

	def   getCommentBlock(self, sSection):
		#print "Section >%s< \n" % ( sSection ) 
		sComment = ""
		i = 0
		while (i < len(sSection)) and (sSection[i] != '/'):	
			i = i + 1
		i = i + 1
		while (i < len(sSection)) and (sSection[i] != '*'):	
			i = i + 1
		i = i + 1 		
		while i < len(sSection) and sSection[i] != '*':	
			sComment += sSection[i]
			i = i + 1
		#sComment = sComment.replace ("\n" , " " )
		#print "Comment: >%s<" % ( sComment )
		return sComment


#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	CbsConfigConverter(sys.argv)






	##def   writeCbsHeaderDotInFile(self):
		##pass
		##of = open(self.m_sCbsHeaderDotInFile , "w")
		##bWrittenAmCommonDefinesInclude = False
		##for sct in self.m_aSections:
			##if sct.m_sSectType == "SimpleDefine":
				##if not bWrittenAmCommonDefinesInclude:
					##bWrittenAmCommonDefinesInclude = True
					##of.write( "#include <am_common_defines.h>\n\n" )
				### Method 1
				###sSect = sct.m_sSect.replace( "undef", "cmakedefine" )
				###sSect = sSect.replace( sct.m_sDefineName, "%s @%s@" % ( sct.m_sDefineName, sct.m_sDefineName ) )
				###of.write( sSect + "\n")
				### Method 2
				##of.write( sct.commentBlockCStyle() + "\n")
				##of.write( "#cmakedefine %s @%s@\n\n" % (sct.m_sDefineName, sct.m_sDefineName) )
				###print "Simple Define: '%s' " % ( sct.m_sDefineName )
			##else:
				##of.write(sct.m_sSect + "\n")
		##of.write("\n")	# Make sure we end with a newline to avoid compiler warnings
