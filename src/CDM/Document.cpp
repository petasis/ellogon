/*****************************************************************************
 *  Document.cpp:                                                            *
 * ----------------------                                                    *
 *  This file implements the Document section of the                         *
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

#include "CDM3.h"
#include <iostream>
#include <sstream>
#include <stdexcept>            /* For std::invalid_argument   */
#include <algorithm>            /* For std::for_each           */
#include <boost/filesystem.hpp> /* For boost::filesystem::path */

using namespace ELEP::CDM;

ELEP::CDM::Document::Document() :
  _data(), _id(), _external_id(), _attributes(), _annotations(),
  _collection() {
};

ELEP::CDM::Document::Document(const char* name, const char* data,
      const AttributeSet& attributes, const AnnotationSet& annotations):
  _external_id(name), _data(data), _attributes(attributes),
  _collection() {
  _id = boost::filesystem::path(_external_id).filename().string();
};

ELEP::CDM::Document::Document(const std::string& name,
      const std::string& data,
      const AttributeSet& attributes, const AnnotationSet& annotations):
  _external_id(name), _data(data), _attributes(attributes),
  _collection() {
  _id = boost::filesystem::path(_external_id).filename().string();
};

ELEP::CDM::Document::Document(const Document& src) :
  _data(src._data), _id(src._id), _external_id(src._external_id),
  _attributes(src._attributes), _annotations(src._annotations) {
};

#ifdef TCL_VERSION
ELEP::CDM::Document::Document(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  Tcl_WideInt start, end;

  /* A valid Document is a list with 2 integers... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count != 2) {
    std::string msg("invalid Document (2-item list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[0], &start) != TCL_OK) {
    std::string msg("invalid Document start (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[1], &end) != TCL_OK) {
    std::string msg("invalid Document end (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (start > end) throw std::invalid_argument("start exceeds end");
};
#endif /* TCL_VERSION */

const std::string& ELEP::CDM::Document::id() const {
  return _id;
};

const std::string& ELEP::CDM::Document::externalId() const {
  return _external_id;
};

const std::string& ELEP::CDM::Document::data() const {
  return _data;
};

const AttributeSet& ELEP::CDM::Document::attributes() const {
  return _attributes;
};

const AnnotationSet& ELEP::CDM::Document::annotations() const {
  return _annotations;
};

bool ELEP::CDM::Document::valid() const {
  return true;
};

const std::string ELEP::CDM::Document::toString() const {
  std::ostringstream ss;
  return ss.str();
};

bool ELEP::CDM::Document::operator< (const Document& Document) const {
  return true;
};

ELEP::CDM::Collection::Collection() :
  _id(), _attributes(), set() {
};

ELEP::CDM::Collection::Collection(const Collection& src) :
  _id(src._id), _attributes(src._attributes), set(src.set) {
};

#ifdef TCL_VERSION
ELEP::CDM::Collection::Collection(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  Tcl_WideInt start, end;

  /* A valid Collection is a list with 2 integers... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count != 2) {
    std::string msg("invalid Collection (2-item list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[0], &start) != TCL_OK) {
    std::string msg("invalid Collection start (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[1], &end) != TCL_OK) {
    std::string msg("invalid Collection end (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (start > end) throw std::invalid_argument("start exceeds end");
};
#endif /* TCL_VERSION */

bool ELEP::CDM::Collection::valid() const {
  return true;
};

const std::string ELEP::CDM::Collection::toString() const {
  std::ostringstream ss;
  return ss.str();
};

bool ELEP::CDM::Collection::operator< (const Collection& Collection) const {
  return true;
};

CDM_Document CDM_CreateDocument(const char *name, const CDM_ByteSequence data,
    const CDM_AttributeSet attributes, const CDM_AnnotationSet annotations) {
  const ELEP::CDM::AttributeSet *attrs =
        reinterpret_cast<const ELEP::CDM::AttributeSet*>(attributes);
  if (!name || *name == '\0') return nullptr;
  if (!attrs) return nullptr;
  const ELEP::CDM::AnnotationSet *anns =
        reinterpret_cast<const ELEP::CDM::AnnotationSet*>(annotations);
  if (!anns) return nullptr;
  return new Document(name, data, *attrs, *anns);
};


