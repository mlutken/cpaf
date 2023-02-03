#!/usr/bin/python

# Various utility functions for use in the CBS python scripts

def createValuesSETList( sName, sValueType, lValues ):
	""" Create a list of valus with entries like e.g. 
		SET ( sName_VALUETYPE ${sName_VALUETYPE}    myvalue ) """
	lValuesSETList = []
	for sValue in lValues:
		lValuesSETList.append( "SET ( %s_%s ${%s_%s}    %s )\n" % ( sName, sValueType, sName, sValueType, sValue ) )
	return lValuesSETList


