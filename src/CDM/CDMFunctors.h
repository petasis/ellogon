/*****************************************************************************
 *  CDMFunctor.h:                                                            *
 * ----------------------                                                    *
 *  This file defines the Functors section of the                            *
 *  Collection and Document Manager (CDM) Public API.                        *
 *                                                                           *
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

#ifndef ELLOGON_CDM_Functor
#define ELLOGON_CDM_Functor

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    namespace Functor {
      namespace Annotation {
        class HasType: public AnnotationUnaryPredicateWithStringState {
          bool operator() (const ELEP::CDM::Annotation& ann) const { return ann.type() == m_state; };
        }; /* class HasType */

        class HasAttribute: public AnnotationUnaryPredicateWithStringState {
          bool operator() (const ELEP::CDM::Annotation& ann) const { return ann.exists(m_state); };
        }; /* class HasAttribute */

        template<typename Pattern>
        class HasAttributeMatchingValue: public AnnotationUnaryPredicateWithStates<std::string, Pattern> {
          bool operator() (const ELEP::CDM::Annotation& ann) const {
            return ann.containsAttributeMatchingValue(std::get<0>(AnnotationUnaryPredicateWithStates<std::string, Pattern>::m_state),
                                                      std::get<1>(AnnotationUnaryPredicateWithStates<std::string, Pattern>::m_state));
          };
        }; /* class HasAttributeMatchingValue */

        class ContainsPosition: public AnnotationUnaryPredicateWithState<ELEP::CDM::Position>  {
          bool operator() (const ELEP::CDM::Annotation& ann) const { return ann.contains(m_state); };
        }; /* class ContainsPosition */

      }; /* namespace ELEP::CDM::Functor::Annotation */
    }; /* namespace ELEP::CDM::Functor */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

#endif /* ELLOGON_CDM_Functor */
/*
 * vim: nowrap columns=120
 */
