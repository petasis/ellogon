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
#include <tuple>       /* For std::tuple              */
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
      int CDM_StringCaseMatch( const char *str, const char *pattern, int nocase);
    }; /* namespace ELEP::CDM::Utilities */

    namespace Unicode {
      const std::string normalise_nfc(const std::string& data);
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
#else  /* __cplusplus */
#include <stdint.h>
#endif /* __cplusplus */

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

typedef int               CDM_Status;
typedef uint32_t          CDM_Position;
typedef uint32_t          CDM_Id;
typedef  int32_t          CDM_Int;
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
#include "CDMFunctors.h"

#endif /* ELLOGON_CDM */
