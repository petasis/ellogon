/*****************************************************************************
 *  CDM.h:                                                                   *
 * ----------------------                                                    *
 *  This file defines the Collection and Document Manager (CDM)              *
 *  Public API.                                                              *
 *                                                                           *
 *  Version: 3.0                                                             *
 *  For more details regarding CDM Version consult file "CDM_Version.h"      *
 *  CDM is the Collection and Document Manager and the core component of the *
 *  Ellogon Language Engineering Platform.                                   *
 *  CDM was designed and implemented by Georgios Petasis,                    *
 *  e-mail: petasis@iit.demokritos.gr, petasisg@yahoo.gr, petasisg@gamil.com *
 *  URL:    http://www.ellogon.org/petasis                                   *
 *                                                                           *
 *  Copyright 1998-2017 by:                                                  *
 *  Georgios Petasis,                                                        *
 *  Software and Knowledge Engineering Laboratory,                           *
 *  Institute of Informatics and Telecommunications,                         *
 *  National Center for Scientific Research (N.C.S.R.) "Demokritos",         *
 *  Athens, Greece.                                                          *
 *  E-mail: petasis@iit.demokritos.gr                                        *
 *                                                                           *
 *  This file is part of the "Ellogon" Language Engineering Platform.        *
 *  For licencing information, please refer to the licence conditions of     *
 *  "Ellogon" Language Engineering Platform.                                 *
 *****************************************************************************/

#ifdef    ELLOGON_CDM_USE_TCL
#include <tcl.h>
#endif /* ELLOGON_CDM_USE_TCL */

#ifndef ELLOGON_CDM
#define ELLOGON_CDM

#ifdef __cplusplus

#ifndef SWIG
#include <string>      /* For std::string             */
#include <memory>      /* For smart pointers          */
#include <cstdint>     /* For C++11 type uint32_t     */
#endif /* SWIG */

namespace ELEP {
  namespace CDM {
    enum class                Status {OK = 0, ERROR = 1};
    enum class                AttributeType {
                                  NONE               =  0,
                                  STRING             =  1,
                                  STRING_SET         =  2,
                                  SPAN               =  3,
                                  SPAN_SET           =  4,
                                  ANNOTATION_ID      =  5,
                                  ANNOTATION_ID_SET  =  6,
                                  IMAGE              = 10,
                                  BASE64_IMAGE       = 11
    };
    typedef uint32_t          Position;
    typedef uint32_t          Id;
    typedef size_t            Size;

    namespace Cache {
      std::shared_ptr<std::string>
      get_string(const std::string &str, const bool clear = false);
#if 0
      std::shared_ptr<std::string>
      get_string(std::string &&str, const bool clear = false);
#endif
      // std::shared_ptr<std::string>
      // get_string(const char *str, const bool clear = false);
    }; /* namespace ELEP::CDM::Cache */

    namespace Utilities {
      std::string ensure_list_element(const std::string &str);

      int CDM_StringMatch(const char *str, const char *pattern);
      int CDM_StringCaseMatch( const char *str, const char *pattern, int nocase);
    }; /* namespace ELEP::CDM::Utilities */

    namespace Unicode {
      const std::string normalise_nfc(const std::string& data);
    }; /* namespace ELEP::CDM::Unicode */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#else  /* __cplusplus */
#include <stdint.h>
#endif /* __cplusplus */

/* Status */
#define CDM_OK                        0
#define CDM_ERROR                     1

/* Attribute Types */
#define CDM_NONE                      0
#define CDM_STRING                    1
#define CDM_STRING_SET                2
#define CDM_SPAN                      3
#define CDM_SPAN_SET                  4
#define CDM_ANNOTATION_ID             5
#define CDM_ANNOTATION_ID_SET         6
#define CDM_IMAGE                    10
#define CDM_BASE64_IMAGE             11

typedef int               CDM_Status;
typedef uint32_t          CDM_Position;
typedef uint32_t          CDM_Id;
typedef size_t            CDM_Size;
typedef void *            CDM_Span;
typedef void *            CDM_SpanSet;
typedef void *            CDM_AttributeValue;
typedef uint8_t           CDM_AttributeType;
typedef void *            CDM_Attribute;
typedef void *            CDM_AttributeSet;
typedef void *            CDM_Annotation;
typedef void *            CDM_AnnotationSet;
typedef char *            CDM_ByteSequence;
typedef void *            CDM_Document;
typedef void *            CDM_Collection;

#include "Serialisation.h"
#include "Span.h"
#include "Attribute.h"
#include "Annotation.h"
#include "Document.h"

#endif /* ELLOGON_CDM */
