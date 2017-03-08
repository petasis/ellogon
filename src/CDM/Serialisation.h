/*****************************************************************************
 *  Serialisation.h:                                                         *
 * ----------------------                                                    *
 *  This file defines the Serialisation section of the                       *
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

#ifndef ELLOGON_CDM_SERIALISATION
#define ELLOGON_CDM_SERIALISATION

/*
 * Serialisation support (through the cereal library)
 */
#ifndef SWIG
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/access.hpp>
#include <cereal/types/common.hpp>                /* Things like enum */
#include <cereal/types/vector.hpp>                /* std::vector      */
#include <cereal/types/string.hpp>                /* std::string      */
#include <cereal/types/memory.hpp>                /* std::shared_ptr  */
#include <cereal/types/boost_multi_index.hpp>
#endif /* SWIG */

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    namespace serialisation {

      template <class T>
      class Serialisation {
        public:
          const std::string save_binary() const;
          void              save_binary(const char *) const;
          void              save_binary(const std::string&) const;
          const std::string save_JSON() const;
          void              save_JSON(const char *) const;
          void              save_JSON(const std::string&) const;
          const std::string save_XML() const;
          void              save_XML(const char *) const;
          void              save_XML(const std::string&) const;
      };

    };
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */

#include "Serialisation.cpp"

#endif /* __cplusplus */

#endif /* ELLOGON_CDM_SERIALISATION */

/*
 * vim: nowrap columns=120
 */
