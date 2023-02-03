#!/usr/bin/python

# Document classes for implementing the project files parsing/writing
# and general manipulation.
#
# Built in types to maybe inherit from: list
#
# Get Class Name/Type:
#		print "ClassType: %s"  % self.__class__.__name__


# ******************
# *** BASE TYPES ***
# ******************

class Node:
	def	__init__( self ):
		pass
	
	def writeToFile(self, outFile):
		pass
	
	def debugPrint( self ):
		pass


class Line (Node):
#	m_sComment		= ""
#	m_sValue		= ""
	
	def	__init__( self, sValue = "", sComment = "" ):
		Node.__init__(self)
		self.m_sValue		= sValue
		self.m_sComment		= sComment
#		print "Line CONSTRUCTOR"
	
	def value(self):
		return self.m_sValue
	
	def commentString(self):
		if self.m_sComment != ""	: return '#%s' % self.m_sComment
		else						: return ''
	
	def string(self):
		return '%s%s' % ( self.value(), self.commentString())

	def writeToFile(self, outFile):
		outFile.write( '%s\n' % self.string() )

	def debugPrint( self ):
		print self.string()


#SET ( cpaf_USE                      BUILD    )


class Section (Node):
	def	__init__( self ):
		Node.__init__(self)
	
	def getLines( self ):
		return []
	
	def writeToFile( self, outFile):
		for line in self.getLines(): line.writeToFile( outFile )

	def debugPrint( self ):
		for line in self.getLines(): line.debugPrint()
	

# *************
# *** XXX ***
# *************

""" Just a sequence (paragraph) of simple lines ( Line ) """
class ParagraphLines (Section):
#	m_lValues	= []	# (List of Line) values

	def	__init__( self, lValues = [] ):
		Section.__init__(self)
		self.m_lValues	= []
		self.m_lValues.extend( lValues )
	
	def listName( self ):
		return self.m_sListName

	def getLines( self ):
		return self.m_lValues

	def append( self, line ):
		self.m_lValues.append( line )

	def appendLine( self, sValue = "", sComment = "" ):
		self.m_lValues.append( Line( sValue, sComment) )

	def extend( self, lValues ):
		self.m_lValues.extend( lValues)


# *************
# *** LISTS ***
# *************

""" SET ( m_sElemName ${m_sElemName}    m_sElemValue )"""
class Elem_ListSET (Line):
#	m_sListName		= ""
#	m_sElemValue	= ""
	
	def	__init__( self, sListName, sElemValue ):
		Line.__init__(self)
		self.m_sListName	= sListName
		self.m_sElemValue	= sElemValue
	
	def value(self):
		return 'SET ( %s   ${%s}  %s ) ' % ( self.m_sListName, self.m_sListName, self.m_sElemValue )


class ListSET (Section):
#	m_sListName	= ""
#	m_lsValues	= []	# (List of Strings) values

	def	__init__( self, sListName = ""):
		Section.__init__(self)
		self.m_sListName = sListName
		self.m_lsValues	= []	# (List of Strings) values

	def listName( self ):
		return self.m_sListName

	def getLines( self ):
		lines = []
		for sValue in self.m_lsValues:
			lines.append ( Elem_ListSET( self.listName(), sValue) )
		return lines

	def append( self, sValue ):
		self.m_lsValues.append( sValue )

	def extend( self, lsList ):
		self.m_lsValues.extend( lsList )


class ListPrjSET (ListSET):
#	m_sPrjName	= ''
#	m_sListType	= ''

	def	__init__( self, sPrjName, sListType ):
		ListSET.__init__(self)
		self.m_sPrjName 	= sPrjName
		self.m_sListType 	= sListType

	def setPrjName( self, sPrjName ):
		self.m_sPrjName = sPrjName

	def listName( self ):
		return '%s_%s' % ( self.m_sPrjName, self.m_sListType )


# *************
# *** XXX ***
# *************


class Document (Node):
	def	__init__( self ):
		Node.__init__(self)
		self.m_bFooterAddedToAllSectionsList = False
		self.m_ParaHeader	= ParagraphLines()
		self.m_ParaFooter	= ParagraphLines()
		self.m_AllSections	= []
		self.m_AllSections	= [self.m_ParaHeader]
	
	def	addFooterToAllSectionsList( self ):
		self.m_AllSections.append(self.m_ParaFooter)
		self.m_bFooterAddedToAllSectionsList = True
	
	def getSections( self ):
		if not self.m_bFooterAddedToAllSectionsList: self.addFooterToAllSectionsList()
		return self.m_AllSections
	
	def writeToFile( self, outFile):
		for section in self.getSections(): section.writeToFile( outFile )

	def debugPrint( self ):
		for section in self.getSections(): section.debugPrint( )
		
		
class DocCbsTargetFile (Document):
	def	__init__( self, sPrjName ):
		Document.__init__(self)
		self.m_sPrjName 				= sPrjName
		self.m_INCLUDE_DIR 				= ListPrjSET(sPrjName, "INCLUDE_DIR"	)
		self.m_DEPENDS_ON 				= ListPrjSET(sPrjName, "DEPENDS_ON" 	)
		self.m_SRCS 					= ListPrjSET(sPrjName, "SRCS" 			)
		self.m_ParaPre_INCLUDE_DIR		= ParagraphLines()
		self.m_ParaPre_DEPENDS_ON		= ParagraphLines()
		self.m_ParaPre_SRCS				= ParagraphLines()
		
		self.m_AllSections.extend( [ self.m_ParaPre_INCLUDE_DIR, 	self.m_INCLUDE_DIR 	] 	)
		self.m_AllSections.extend( [ self.m_ParaPre_DEPENDS_ON,		self.m_DEPENDS_ON  	] 	)
		self.m_AllSections.extend( [ self.m_ParaPre_SRCS,			self.m_SRCS, 	 	]	)
		
	
	def makeDefaultSections(self):
		self.m_ParaPre_INCLUDE_DIR.appendLine()
		self.m_ParaPre_INCLUDE_DIR.appendLine(	"", " --- Target includes (if needed) ---" )
		self.m_ParaPre_DEPENDS_ON.appendLine()
		self.m_ParaPre_DEPENDS_ON.appendLine( 	"", " --- Target depends (on targets) ---" )
		self.m_ParaPre_SRCS.appendLine()
		self.m_ParaPre_SRCS.appendLine( 		"", " --- Add source files ---" )
		self.m_ParaFooter.appendLine()
		
	
		
		
# ********************
# *** DEBUG / TEST ***
# ********************
		
		
def	testDebugDocument():
	print "Testing document code ..."		
		
			
	docPrjFile = DocCbsTargetFile( "cpaf" )
	docPrjFile.makeDefaultSections()
	docPrjFile.m_DEPENDS_ON.append ( "boost_filesystem" )
	docPrjFile.m_DEPENDS_ON.append ( "boost_regex" )
	docPrjFile.m_SRCS.append ( "hest.cpp" )
	docPrjFile.m_INCLUDE_DIR.append ( "include" )
	#docPrjFile.append_SRCS("ged.c")
	docPrjFile.debugPrint()
#	paraLines = ParagraphLines( [Line()] )
	
	docPrjFile1 = DocCbsTargetFile( "openssl" )
	docPrjFile1.makeDefaultSections()
	docPrjFile1.m_DEPENDS_ON.append ( "aaaaaa" )
	docPrjFile1.m_DEPENDS_ON.append ( "bbbbbbb" )
	docPrjFile1.m_SRCS.append ( "cccc.cpp" )
	docPrjFile1.m_INCLUDE_DIR.append ( "inc" )
	#docPrjFile.append_SRCS("ged.c")
	docPrjFile1.debugPrint()
	
	
#	outFile = open('Hest.txt', 'w')
#	listSET.writeToFile(outFile)
		
	