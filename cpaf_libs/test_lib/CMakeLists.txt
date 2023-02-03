
IF ( NOT CBS_BUILD )


ELSE ( NOT CBS_BUILD )
	project ( test_lib )
	INCLUDE ( $ENV{CBS_ROOT_DIR}/cbs_common_rules.cbs )
	BUILD_LIBRARY ( test_lib ) 
ENDIF ( NOT CBS_BUILD )

