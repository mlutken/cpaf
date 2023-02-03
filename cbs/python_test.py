#!/usr/bin/python

# ----------------------------------------------
# --- CBS ... XX ---
# ----------------------------------------------
# Purely a testing bed for python scripts ....


import os
import sys
import re
import copy
import shutil
from path import path

import parser_nodes
import files
from parser_nodes import Node
from parser_nodes import Section
from doc_nodes import *


g_allFiles = [ 
"accel.cpp",
"accesscmn.cpp",
"anidecod.cpp",
"appcmn.cpp",
"app.cpp",
"artgtk.cpp",
"artprov.cpp",
"artstd.cpp",
"assertdlg_gtk.cpp",
"bitmap.cpp",
"bmpbase.cpp",
"bmpbuttn.cpp",
"bookctrl.cpp",
"brush.cpp",
"busyinfo.cpp",
"buttonbar.cpp",
"button.cpp",
"caret.cpp",
"checkbox.cpp",
"checklst.cpp",
"choicbkg.cpp",
"choiccmn.cpp",
"choicdgg.cpp",
"choice.cpp",
"clipbrd.cpp",
"clipcmn.cpp",
"clrpickercmn.cpp",
"clrpickerg.cpp",
"clrpicker.cpp",
"cmdproc.cpp",
"cmndata.cpp",
"collpaneg.cpp",
"collpane.cpp",
"colordlg.cpp",
"colourcmn.cpp",
"colour.cpp",
"colrdlgg.cpp",
"combobox.cpp",
"combocmn.cpp",
"combog.cpp",
"containr.cpp",
"control.cpp",
"cshelp.cpp",
"ctrlcmn.cpp",
"ctrlsub.cpp",
"cursor.cpp",
"datacmn.cpp",
"data.cpp",
"dataobj.cpp",
"dcbase.cpp",
"dcbufcmn.cpp",
"dcclient.cpp",
"dcgraph.cpp",
"dcmemory.cpp",
"dc.cpp",
"dcpsg.cpp",
"dcscreen.cpp",
"dialog.cpp",
"dialup.cpp",
"dirctrlg.cpp",
"dirdlgg.cpp",
"dirdlg.cpp",
"displayx11.cpp",
"dlgcmn.cpp",
"dndcmn.cpp",
"dnd.cpp",
"dobjcmn.cpp",
"docmdi.cpp",
"docview.cpp",
"dpycmn.cpp",
"dragimgg.cpp",
"dseldlg.cpp",
"effects.cpp",
"event.cpp",
"evtloopcmn.cpp",
"evtloop.cpp",
"fddlgcmn.cpp",
"fdrepdlg.cpp",
"filedlgg.cpp",
"filedlg.cpp",
"filepickercmn.cpp",
"filepickerg.cpp",
"filepicker.cpp",
"fldlgcmn.cpp",
"fontcmn.cpp",
"fontdlgg.cpp",
"fontdlg.cpp",
"fontenumcmn.cpp",
"fontenum.cpp",
"fontmap.cpp",
"font.cpp",
"fontpickercmn.cpp",
"fontpickerg.cpp",
"fontpicker.cpp",
"fontutil.cpp",
"framecmn.cpp",
"frame.cpp",
"fs_mem.cpp",
"gaugecmn.cpp",
"gauge.cpp",
"gbsizer.cpp",
"gdicmn.cpp",
"geometry.cpp",
"gifdecod.cpp",
"gprint.cpp",
"graphcmn.cpp",
"graphicc.cpp",
"gsockgtk.cpp",
"gvfs.cpp",
"helpbase.cpp",
"iconbndl.cpp",
"icon.cpp",
"imagall.cpp",
"imagbmp.cpp",
"image.cpp",
"imagfill.cpp",
"imaggif.cpp",
"imagiff.cpp",
"imagjpeg.cpp",
"imaglist.cpp",
"imagpcx.cpp",
"imagpng.cpp",
"imagpnm.cpp",
"imagtga.cpp",
"imagtiff.cpp",
"imagxpm.cpp",
"layout.cpp",
"lboxcmn.cpp",
"listbkg.cpp",
"listbox.cpp",
"listctrlcmn.cpp",
"listctrl.cpp",
"logg.cpp",
"main.cpp",
"matrix.cpp",
"mdi.cpp",
"menucmn.cpp",
"menu.cpp",
"minifram.cpp",
"msgdlgg.cpp",
"msgdlg.cpp",
"msgout.cpp",
"nbkbase.cpp",
"notebook.cpp",
"numdlgg.cpp",
"overlaycmn.cpp",
"paletteg.cpp",
"panelg.cpp",
"paper.cpp",
"pen.cpp",
"pickerbase.cpp",
"popupcmn.cpp",
"popupwin.cpp",
"printps.cpp",
"prntbase.cpp",
"prntdlgg.cpp",
"progdlgg.cpp",
"quantize.cpp",
"radiobox.cpp",
"radiobut.cpp",
"radiocmn.cpp",
"region.cpp",
"rendcmn.cpp",
"renderer.cpp",
"renderg.cpp",
"rgncmn.cpp",
"scrlwing.cpp",
"scrolbar.cpp",
"scrolwin.cpp",
"selstore.cpp",
"settcmn.cpp",
"settings.cpp",
"sizer.cpp",
"slider.cpp",
"spinbutt.cpp",
"spinctlg.cpp",
"spinctrl.cpp",
"splitter.cpp",
"srchcmn.cpp",
"srchctlg.cpp",
"statbar.cpp",
"statbmp.cpp",
"statbox.cpp",
"statline.cpp",
"stattext.cpp",
"statusbr.cpp",
"stockitem.cpp",
"tabg.cpp",
"tbarbase.cpp",
"tbargtk.cpp",
"textcmn.cpp",
"textctrl.cpp",
"textdlgg.cpp",
"tglbtn.cpp",
"timercmn.cpp",
"timer.cpp",
"tipwin.cpp",
"toolbkg.cpp",
"tooltip.cpp",
"toplevel.cpp",
"toplvcmn.cpp",
"treebase.cpp",
"treebkg.cpp",
"treectlg.cpp",
"treeentry_gtk.cpp",
"utilscmn.cpp",
"utilsgtk.cpp",
"utilsres.cpp",
"utilsunx.cpp",
"utilsx11.cpp",
"valgen.cpp",
"validate.cpp",
"valtext.cpp",
"vlbox.cpp",
"vscroll.cpp",
"wincmn.cpp",
"window.cpp",
"win_gtk.cpp",
"xpmdecod.cpp"
]

g_allFiles1 = [ 
"bitmap.cpp",
"checklst.cpp"
]

class PythonTest:
	# ***************
	# *** Members ***
	# ***************
	m_StartUpPath       	= path("")
	m_AllOut 				= []

	# *******************
	# *** Constructor ***
	# *******************
	def	__init__(self, cmd_args):
		print "OS name     : %s \n" % ( os.name )
		self.m_StartUpPath          = path(os.getcwd())
		print "m_StartUpPath : %s \n" % 	( self.m_StartUpPath 	)
		
		
		sWriteFilePath = self.m_StartUpPath / path ( "includes.txt" )
		open( sWriteFilePath, "w").write( "Headers:\n" )
		for file in g_allFiles:
			#print "Name: %s  " % ( file ) 
			sCmd = "find . -name %s" % (file)
			#print sCmd
			Lines = os.popen(sCmd)
			self.m_AllOut.append("\n")
			for line in Lines:
				bAdd = True
				line = line.replace( "./", "ADD_SOURCE_FILE ( ${wxWidgets_SOURCE_DIR}/" )
				if line.find("/motif/")         != -1 : line= line.replace( ".cpp", ".cpp\t\t__WXMOTIF__\t\t\t)" )			
				elif line.find("/palmos/")      != -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXPALMOS__\t\t)" )			
				elif line.find("/mac/classic/") != -1 : line= line.replace( ".cpp", ".cpp\t__WXMAC_CLASSIC__\t)" )			
				elif line.find("/mac/carbon/")  != -1 : line= line.replace( ".cpp", ".cpp\t\t__WXMAC_CARBON__\t)" )			
				elif line.find("/msw/wince/")   != -1 : line= line.replace( ".cpp", ".cpp\t__WXWINCE__\t\t\t)" )
				elif line.find("/msw/")         != -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXMSW__\t\t\t)" )			
				elif line.find("/dfb/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXDFB__\t\t\t)" )			
				elif line.find("/mgl/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXMGL__\t\t\t)" )			
				elif line.find("/gtk/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXGTK__\t\t\t)" )			
				elif line.find("/gtk1/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\tTODO_GTK1\t\t\t)" )			
				elif line.find("/x11/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\t__WXX11__\t\t\t)" )			
				elif line.find("/os2/")      	!= -1 : line= line.replace( ".cpp", ".cpp\t\t\t__OS2__\t\t\t\t)" )			
				elif line.find("/generic/")     != -1 : line= line.replace( ".cpp", ".cpp\t\tTODO_GENERIC\t)" )			
				elif line.find("/common/")      != -1 : line= line.replace( ".cpp", ".cpp\t\t\tTODO_COMMON\t\t\t)" )			
				elif line.find("/univ/")        != -1 : line= line.replace( ".cpp", ".cpp\t\t\tTODO_UNIV\t\t\t)" )			
				elif line.find("/unix/")        != -1 : line= line.replace( ".cpp", ".cpp\t\t\tTODO_UNIX\t\t\t)" )			
				elif line.find("/samples/")     != -1 : bAdd= False			
				else : line= line.replace( ".cpp", ".cpp\t\tTODO_UNKNOWN\t)" )			
				
				if bAdd: self.m_AllOut.append(line)
				#if bAdd:
				#	print "L: %s" % line,

		for line1 in self.m_AllOut:
			print line1,

#	This check detects that	the	script is	being	run	stand-alone, rather
#	than being imported	as a module.
if __name__	== '__main__':
	PythonTest(sys.argv)




#sLine = open(self.m_StartUpPath / path(file), "r").read()
#open( sWriteFilePath, "a").write( sLine + "\n" )
#os.popen(cmd)