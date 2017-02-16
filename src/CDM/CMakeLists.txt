##############################################################################
#   CMakeLists.txt:                                                          #
#  ----------------------                                                    #
#   This file builds Ellogon's                                               #
#   Collection and Document Manager (CDM) library.                           #
#                                                                            #
#   CDM Version: 3.0                                                         #
#   For more details regarding CDM Version consult file "CDM_Version.h"      #
#   CDM is the Collection and Document Manager and the core component of the #
#   Ellogon Language Engineering Platform.                                   #
#   CDM was designed and implemented by Georgios Petasis,                    #
#   e-mail: petasis@iit.demokritos.gr, petasisg@yahoo.gr, petasisg@gamil.com #
#   URL:    http://www.ellogon.org/petasis                                   #
#                                                                            #
#   Copyright 1998-2017 by:                                                  #
#   Georgios Petasis,                                                        #
#   Software and Knowledge Engineering Laboratory,                           #
#   Institute of Informatics and Telecommunications,                         #
#   National Center for Scientific Research (N.C.S.R.) "Demokritos",         #
#   Athens, Greece.                                                          #
#   E-mail: petasis@iit.demokritos.gr                                        #
#                                                                            #
#   This file is part of the "Ellogon" Language Engineering Platform.        #
#   For licencing information, please refer to the licence conditions of     #
#   "Ellogon" Language Engineering Platform.                                 #
##############################################################################

CMAKE_MINIMUM_REQUIRED ( VERSION 2.8 )
PROJECT ( CDM3 )

## ===========================================================================
##  Locate SWIG
## ===========================================================================
MESSAGE ( STATUS "Searching for SWIG..." )
FIND_PACKAGE ( SWIG REQUIRED )

IF ( SWIG_FOUND )
  MESSAGE ( STATUS "  SWIG_EXECUTABLE:        " ${SWIG_EXECUTABLE} )
  MESSAGE ( STATUS "  SWIG_VERSION:           " ${SWIG_VERSION} )
  INCLUDE(${SWIG_USE_FILE})
  SET (CMAKE_SWIG_FLAGS "-c++" )
ENDIF ( SWIG_FOUND )

## =========================================================================
##  Locate Tcl/Tk
## =========================================================================
MESSAGE ( STATUS "Searching for Tcl/Tk..." )
FIND_PACKAGE ( TCL REQUIRED )
FIND_PACKAGE ( TclStub REQUIRED )
ADD_DEFINITIONS ( -DELLOGON_CDM_USE_TCL -DUSE_TCL_STUBS )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_SOURCE_DIR} )

SET ( CDM_LIBRARY_SOURCES
  CDM.cpp
  Span.cpp
  Attribute.cpp
  Annotation.cpp
)
ADD_LIBRARY (CDM3 SHARED ${CDM_LIBRARY_SOURCES} )
ADD_LIBRARY (CDM3S STATIC ${CDM_LIBRARY_SOURCES} )
TARGET_LINK_LIBRARIES(CDM3 ${TCL_STUB_LIBRARY})

## Bindings for Tcl...
## ===========================================================================
##  Create CDM_wrap_tcl.cxx
## ===========================================================================
ADD_CUSTOM_COMMAND ( OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/CDM_wrap_tcl.cxx"
      MAIN_DEPENDENCY CDM3.h
      DEPENDS CDM3.h CDM.cpp Span.h Span.cpp
      Attribute.h Attribute.cpp
      Annotation.h Annotation.cpp
      swig/CDM_Tcl.i swig/CDM_Tcl.cpp
    COMMAND ${SWIG_EXECUTABLE} -Wall -c++ -tcl8 -O -module CDM -prefix CDM
      -namespace -pkgversion 3.0 -includeall -ignoremissing
      -o ${CMAKE_CURRENT_BINARY_DIR}/CDM_wrap_tcl.cxx
      -outdir ${CMAKE_CURRENT_BINARY_DIR} swig/CDM_Tcl.i
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
SET_SOURCE_FILES_PROPERTIES ( ${CMAKE_CURRENT_BINARY_DIR}/CDM_wrap_tcl.cxx
                              PROPERTIES GENERATED true )
ADD_LIBRARY ( tclCDM3 SHARED
   "${CMAKE_CURRENT_BINARY_DIR}/CDM_wrap_tcl.cxx" ${CDM_LIBRARY_SOURCES}
)
TARGET_COMPILE_DEFINITIONS(tclCDM3 PUBLIC
   -DSWIG_TCL_STUBS_VERSION=TCL_VERSION -DSWIGTCL )
TARGET_LINK_LIBRARIES(tclCDM3 ${TCL_STUB_LIBRARY})

## ===========================================================================
##  Tests
## ===========================================================================

## ===========================================================================
##  Install
## ===========================================================================
EX_INSTALL( Ellogon-CDM CDM3 CDM3S tclCDM3 )
