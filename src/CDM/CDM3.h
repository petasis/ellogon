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

#define CDM_DEFAULT_LOCALE "en_US.utf8"
#ifndef SWIG
#ifdef __cplusplus
#include <cstdint>     /* For C++11 type uint32_t     */
#else
#include <stdint.h>
#endif
#endif /* SWIG */

/* Status */
#define CDM_OK                        0
#define CDM_ERROR                     1
#define CDM_FALSE                    10

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

/*
 * C types. These types are used by the C API (CDM_* functions).
 */
typedef int                                CDM_Status;
typedef uint32_t                           CDM_Position;
typedef uint32_t                           CDM_Id;
typedef  int32_t                           CDM_Int;
typedef size_t                             CDM_Size;
typedef struct CDM_Span_t *                CDM_Span;
typedef struct CDM_SpanSet_t *             CDM_SpanSet;
typedef struct CDM_AttributeValue_t *      CDM_AttributeValue;
typedef uint8_t                            CDM_AttributeType;
typedef struct CDM_Attribute_t *           CDM_Attribute;
typedef struct CDM_AttributeSet_t *        CDM_AttributeSet;
typedef struct CDM_Annotation_t *          CDM_Annotation;
typedef struct CDM_AnnotationSet_t *       CDM_AnnotationSet;
typedef char const *                       CDM_ByteSequence;
typedef struct CDM_ByteSequenceSet_t *     CDM_ByteSequenceSet;
typedef struct CDM_Document_t *            CDM_Document;
typedef struct CDM_Collection_t *          CDM_Collection;

#ifdef __cplusplus

#ifndef SWIG
#include <string>      /* For std::string             */
#include <utility>     /* For std::pair               */
#include <memory>      /* For smart pointers          */
#include <tuple>       /* For std::tuple              */
#endif /* SWIG */

#define CDM_CastFromOpaque(T, p) reinterpret_cast< T > ( p )
#define CDM_CastToOpaque(T, p)   reinterpret_cast< T > ( p )

namespace ELEP {
  namespace CDM {
    enum class                Status {OK = 0, ERROR = 1, FALSE = 10};
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
    typedef  int32_t          Int;
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
      int CDM_StringCaseMatch( const char *str, const char *pattern,
          int nocase = 1, const char *locale = CDM_DEFAULT_LOCALE);

      CDM_ByteSequence CDM_StringToCString(const std::string & str);
      void  CDM_FreeCString(CDM_ByteSequence str);
    }; /* namespace ELEP::CDM::Utilities */

    namespace Unicode {
      std::string normalise_nfc(const std::string& data,
                                const char *locale = CDM_DEFAULT_LOCALE);
      std::string utf8_substr(const std::string& str,
                              size_t start, size_t end);
      bool utf8_byteOffsets(const std::string& str,
                            const size_t start, const size_t end,
                            size_t *byte_start, size_t *byte_end);
    }; /* namespace ELEP::CDM::Unicode */

    class Annotation;
    namespace Functor {
      template<class Arg, class Result>
      class UnaryFunction {
        public:
          virtual Result operator()(const Arg &) const = 0;
        // base_type *CreateCopy() const =0
      }; /* UnaryFunction */

      template<class Arg, class Result, class State>
      class UnaryFunctionWithState : public UnaryFunction<Arg, Result> {
        public:
          //UnaryFunctionWithState() = default;
          UnaryFunctionWithState(State&& state) :
            m_state(std::forward<State>(state)) {};
        protected:
          State m_state;
      }; /* UnaryFunctionWithState */

      template<class Arg, class Result, class... Ts>
      class UnaryFunctionWithStates : public UnaryFunction<Arg, Result> {
        public:
          //UnaryFunctionWithStates() = default;
          template <class... Args>
          UnaryFunctionWithStates(Args&&... args) :
            m_state(std::forward<Args>(args)...) {};
        protected:
#ifdef   SWIG /* SWIG does not seem to understand std::tuple<Ts...> */
          std::tuple<Ts>    m_state;
#else
          std::tuple<Ts...> m_state;
#endif /* SWIG */
      }; /* UnaryFunctionWithStates */
      template<class Arg>
      class UnaryPredicate : public UnaryFunction<Arg, bool> {};
      template<class Arg, class State>
      class UnaryPredicateWithState :
        public UnaryFunctionWithState<Arg, bool, State> {};
      template<class Arg, class... States>
      class UnaryPredicateWithStates :
#ifdef    SWIG
        public UnaryFunctionWithStates<Arg, bool, States> {};
#else
        public UnaryFunctionWithStates<Arg, bool, States ... > {};
#endif /* SWIG */
      class AnnotationUnaryPredicate :
        public UnaryPredicate<ELEP::CDM::Annotation> {};
      template<class State>
      class AnnotationUnaryPredicateWithState :
        public UnaryPredicateWithState<ELEP::CDM::Annotation, State> {};
      template<class... States>
      class AnnotationUnaryPredicateWithStates :
#ifdef    SWIG
        public UnaryPredicateWithStates<ELEP::CDM::Annotation, States> {};
#else
        public UnaryPredicateWithStates<ELEP::CDM::Annotation, States ... > {};
#endif /* SWIG */
      class AnnotationUnaryPredicateWithStringState :
        public AnnotationUnaryPredicateWithState<std::string> {};
    }; /* namespace ELEP::CDM::Functor */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

#include "Serialisation.h"
#include "Span.h"
#include "Attribute.h"
#include "Annotation.h"
#include "Document.h"
#include "CDMFunctors.h"

#endif /* ELLOGON_CDM */
