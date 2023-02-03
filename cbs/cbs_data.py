#!/usr/bin/python

# Various data/templates for use in the CBS python scripts

# Template CMakeLists.txt project file
g_lCMakeListsFile = [
	"IF ( NOT CBS_BUILD )\n",
	"\n",
	"\n",
	"ELSE ( NOT CBS_BUILD )\n",
	"	project( PRJ_NAME )\n",
	"	INCLUDE ( $ENV{CBS_ROOT_DIR}/cbs_common_rules.cbs )\n",
	"	BUILD_LIBRARY ( PRJ_NAME )\n",
	"ENDIF ( NOT CBS_BUILD )\n" ]

