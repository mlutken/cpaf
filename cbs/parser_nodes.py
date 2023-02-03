
import os
import shutil
import re
import sys
import time
import re


# ************
# *** Node ***
# ************
class Node:
    #private:
    # m_sName
    # 
  
    ''' Constructor '''
    def __init__(self, sName):
        self.m_sName        = sName.upper()
        self.m_reCbsCmd     = re.compile( '#\s*CBS\s+' )
    
    def name(self):
        return self.m_sName
    
    ''' 
    Tries to parse the given string (line) as a begin for this section. 
    Returns pair (bool, string). In case of success the remainder of the parsed 
    string is returned, otherwise the unchanged string is returned.'''
    def standardParse(self, sLine, reExpToMatch):
        mMatch = reExpToMatch.match( sLine ) 
        if mMatch:
            return 1, sLine[mMatch.end():len(sLine)]
        else:
            return 0, sLine            
    
    
    def startNewLine(self, sLine):
        print "startNewLine"
        
    # --- Simple debug functions ---   
    def toString(self):
        return 'Node -- name: %s' % self.m_sName


    
    

class Section(Node):
    #private:
    # m_sName
    # m_reBegin, m_reEnd
    def __beginName(self):
        return self.m_sName
    def __endName(self):
        return 'END%s' % self.m_sName
    
    #public:
    ''' Constructor - '''
    def __init__(self, sName):
        Node.__init__(self, sName)
        self.m_reBegin  = re.compile('#\s*%s' % (self.__beginName()) )
        self.m_reEnd    = re.compile('#\s*%s' % (self.__endName()) )


    ''' 
    Standard parse/match (see Node::standardParse()) of the begin section keyword. '''
    def parseBegin(self, sLine):
        return self.standardParse(sLine, self.m_reBegin)

    ''' 
    Standard parse/match (see Node::standardParse()) of the end section keyword. '''
    def parseEnd(self, sLine):
        return self.standardParse(sLine, self.m_reEnd)


