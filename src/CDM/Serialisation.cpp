/*****************************************************************************
 *  Serialisation.cpp:                                                       *
 * ----------------------                                                    *
 *  This file implements the Serialisation section of the                             *
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

#ifndef SWIG
#include <sstream>
#include <fstream>
#endif /* SWIG */


template <class T> const std::string
ELEP::CDM::serialisation::Serialisation<T>::save_binary() const {
  T const& obj = static_cast<T const&>(*this);
  std::ostringstream os;
  {
    cereal::PortableBinaryOutputArchive ar(os);
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  return os.str();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_binary(const char * fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::PortableBinaryOutputArchive ar(os);
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_binary(const std::string& fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::PortableBinaryOutputArchive ar(os);
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};

template <class T> const std::string
ELEP::CDM::serialisation::Serialisation<T>::save_JSON() const {
  T const& obj = static_cast<T const&>(*this);
  std::ostringstream os;
  {
    cereal::JSONOutputArchive ar(os, cereal::JSONOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  return os.str();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_JSON(const char * fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::JSONOutputArchive ar(os, cereal::JSONOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_JSON(const std::string& fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::JSONOutputArchive ar(os, cereal::JSONOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};

template <class T> const std::string
ELEP::CDM::serialisation::Serialisation<T>::save_XML() const {
  T const& obj = static_cast<T const&>(*this);
  std::ostringstream os;
  {
    cereal::XMLOutputArchive ar(os, cereal::XMLOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  return os.str();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_XML(const char * fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::XMLOutputArchive ar(os, cereal::XMLOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};

template <class T> void
ELEP::CDM::serialisation::Serialisation<T>::save_XML(const std::string& fname) const {
  std::ofstream os(fname, std::ios::binary);
  T const& obj = static_cast<T const&>(*this);
  {
    cereal::XMLOutputArchive ar(os, cereal::XMLOutputArchive::Options::NoIndent());
    ar(cereal::make_nvp(obj.serialise_variable_name(), obj));
  }
  os.close();
};
