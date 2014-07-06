cmake_minimum_required( VERSION 2.8 )
project( ZTEmodem )

set( EXECUTABLE_OUTPUT_PATH bin )
set( SOURCE_DIR src )
set( SUBPROGRAMS minit_session mget_imei msend_sms )

foreach( PROG_NAME ${SUBPROGRAMS} )
	add_executable( ${PROG_NAME} "${SOURCE_DIR}/${PROG_NAME}.c" )
endforeach( PROG_NAME )