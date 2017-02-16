/*****************************************************************************
 *  Annotation.cpp:                                                          *
 * ----------------------                                                    *
 *  This file implements the Annotation section of the                       *
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
#include <climits>     /* For ULONG_MAX   */
#include <regex>

using namespace ELEP::CDM;

const Id ELEP::CDM::Annotation::no = ULONG_MAX;

ELEP::CDM::Annotation::Annotation() :
  _id {no}, _type(), _spans(), _attributes() {
};

ELEP::CDM::Annotation::Annotation(const char *type) :
  _id {no}, _spans(), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, const SpanSet &spans) :
  _id {no}, _spans(spans), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, const SpanSet &spans, const AttributeSet &attributes) :
  _id {no}, _spans (spans), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, const AttributeSet &attributes) :
  _id {no}, _spans(), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, SpanSet &&spans) :
  _id {no}, _spans(spans), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, SpanSet &&spans, AttributeSet &&attributes) :
  _id {no}, _spans(spans), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const char *type, AttributeSet &&attributes) :
  _id {no}, _spans(), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type) :
  _id {no}, _spans(), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, const SpanSet &spans) :
  _id {no}, _spans(spans), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, const SpanSet &spans, const AttributeSet &attributes) :
  _id {no}, _spans(spans), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, const AttributeSet &attributes) :
  _id {no}, _spans(), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, SpanSet &&spans) :
  _id {no}, _spans(spans), _attributes() {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, SpanSet &&spans, AttributeSet &&attributes) :
  _id {no}, _spans(spans), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const std::string &type, AttributeSet &&attributes) :
  _id {no}, _spans(), _attributes(attributes) {
  _type = Cache::get_string(type);
};

ELEP::CDM::Annotation::Annotation(const Annotation& src) :
  _id(src._id), _type(src._type), _spans(src._spans),
  _attributes(src._attributes) {
};

const Id ELEP::CDM::Annotation::Annotation::id() const {
  return _id;
};

const std::string& ELEP::CDM::Annotation::type() const {
  return *_type;
};

void ELEP::CDM::Annotation::type(const char* type) {
  _type = Cache::get_string(type);
};

void ELEP::CDM::Annotation::type(const std::string& type) {
  _type = Cache::get_string(type);
};

const SpanSet& ELEP::CDM::Annotation::spans() const {
  return _spans;
};

void ELEP::CDM::Annotation::spans(const SpanSet& spans) {
  _spans = spans;
};

void ELEP::CDM::Annotation::spans(SpanSet&& spans) {
  _spans = spans;
};

const AttributeSet& ELEP::CDM::Annotation::attributes() const {
  return _attributes;
};

void ELEP::CDM::Annotation::attributes(const AttributeSet& Attributes) {
  _attributes = Attributes;
};

void ELEP::CDM::Annotation::attributes(AttributeSet&& Attributes) {
  _attributes = Attributes;
};

bool ELEP::CDM::Annotation::valid() const {
  return true;
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(const char *name,
           const char *pattern) const {
  auto attr = find(name);
  if (attr == _attributes.end()) return false;
  if (!pattern) return true;
  std::regex re(pattern);
  return std::regex_match(attr->value(), re);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(
           const std::string& name, const std::string& pattern) const {
  auto attr = find(name);
  if (attr == _attributes.end()) return false;
  if (!pattern.size()) return true;
  std::regex re(pattern);
  return std::regex_match(attr->value(), re);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(const char *name,
           const std::regex& pattern) const {
  auto attr = find(name);
  if (attr == _attributes.end()) return false;
  return std::regex_match(attr->value(), pattern);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(
           const std::string& name, const std::regex& pattern) const {
  auto attr = find(name);
  if (attr == _attributes.end()) return false;
  return std::regex_match(attr->value(), pattern);
};

const std::string ELEP::CDM::Annotation::toString() const {
  std::ostringstream ss;
  if (_id != no) ss << _id; else ss << "{}";
  ss << ' '
     << (_type ? ELEP::CDM::Utilities::ensure_list_element(*_type) : "{}")
     << " {" << _spans.toString() << "} {" << _attributes.toString() << '}';
  return ss.str();
};

bool ELEP::CDM::Annotation::operator< (const Annotation& Annotation) const {
  return false;
};

#ifdef TCL_VERSION
ELEP::CDM::Annotation::Annotation(Tcl_Interp *interp, Tcl_Obj *obj) :
  _id {no} {
  Tcl_Obj **items;
  int count, len;
  Tcl_WideInt id;

  /* A valid Annotation is a list with 4 items, or a list of two items... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count != 4) {
    throw Status::ERROR;
  }
  /* Get Id: either an integer, or an empty id */
  if (Tcl_GetWideIntFromObj(interp, items[0], &id) != TCL_OK) {
    Tcl_GetStringFromObj(items[0], &len);
    if (len != 0) throw Status::ERROR;
  }
  /* Get the type... */
  _id         = id;
  _type       = Cache::get_string(Tcl_GetString(items[1]));
  _spans      = SpanSet(interp, items[2]);
  _attributes = AttributeSet(interp, items[3]);
};
#endif /* TCL_VERSION */

ELEP::CDM::AnnotationSet::AnnotationSet() : set() {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const Annotation &annotation):
  set(1, annotation) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(Annotation &&annotation):
  set(1, annotation) {
};


ELEP::CDM::AnnotationSet::AnnotationSet(const char *type):
  set(1, Annotation(type)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const SpanSet &spans):
  set(1, Annotation(type, spans)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const SpanSet &spans, const AttributeSet &attributes):
  set(1, Annotation(type, spans, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const AttributeSet &attributes):
  set(1, Annotation(type, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      SpanSet &&spans):
  set(1, Annotation(type, spans)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      SpanSet &&spans, AttributeSet &&attributes):
  set(1, Annotation(type, spans, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      AttributeSet &&attributes):
  set(1, Annotation(type, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type):
  set(1, Annotation(type)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const SpanSet &spans):
  set(1, Annotation(type, spans)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const SpanSet &spans, const AttributeSet &attributes):
  set(1, Annotation(type, spans, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const AttributeSet &attributes):
  set(1, Annotation(type, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      SpanSet &&spans):
  set(1, Annotation(type, spans)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      SpanSet &&spans, AttributeSet &&attributes):
  set(1, Annotation(type, spans, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      AttributeSet &&attributes):
  set(1, Annotation(type, attributes)) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const AnnotationSet& src) :
  set(src.set) {
};

#ifdef TCL_VERSION
ELEP::CDM::AnnotationSet::AnnotationSet(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  /* A valid Annotation set is a list of Annotations... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK) {
    throw Status::ERROR;
  }
  for (int i = 0; i < count; ++i) {
    set.push_back(Annotation(interp, items[i]));
  }
};
#endif /* TCL_VERSION */

bool ELEP::CDM::AnnotationSet::valid() const {
  if (set.empty()) return false;
  auto it = set.cbegin();
  return it->valid();
};

const std::string ELEP::CDM::AnnotationSet::toString() const {
  if (set.empty()) return "";
  std::ostringstream ss;
  bool add_space = false;
  // ss << '{';
  for (auto it = set.cbegin(); it != set.cend(); ++it) {
    if (add_space) ss << ' ';
    ss << '{' << it->toString() << '}';
    add_space = true;
  }
  // ss << '}';
  return ss.str();
};

CDM_Annotation CDM_CreateAnnotation(const char *type, const CDM_SpanSet spans,
                                    const CDM_AttributeSet attributes) {
  ELEP::CDM::Annotation *p = nullptr;
  const ELEP::CDM::SpanSet *s =
        reinterpret_cast<const ELEP::CDM::SpanSet*>(spans);
  const ELEP::CDM::AttributeSet *a =
        reinterpret_cast<const ELEP::CDM::AttributeSet*>(attributes);
  if (s) {
    if (a) p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::SpanSet) *s, (const ELEP::CDM::AttributeSet) *a);
    else   p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::SpanSet) *s);
  } else {
    if (a) p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::AttributeSet) *a);
    else   p = new ELEP::CDM::Annotation(type);
  }
  return p;
};

CDM_Annotation CDM_CreateAnnotation(const char *type,
                                    const CDM_Position start,
                                    const CDM_Position end,
                                    const CDM_AttributeSet attributes) {
  ELEP::CDM::Annotation *p = nullptr;
  const ELEP::CDM::AttributeSet *a =
        reinterpret_cast<const ELEP::CDM::AttributeSet*>(attributes);
  if (a) p = new ELEP::CDM::Annotation(type, SpanSet(start, end), *a);
  else   p = new ELEP::CDM::Annotation(type, SpanSet(start, end));
  return p;
};

int CDM_AnnotationContainsAttributeMatchingValue(const CDM_Annotation Ann,
        const char *AttributeName, const char *ValuePattern) {
  const ELEP::CDM::Annotation *a =
        reinterpret_cast<const ELEP::CDM::Annotation*>(Ann);
  if (a) return a->containsAttributeMatchingValue(AttributeName, ValuePattern);
  return false;
};

int CDM_AnnotationContainsPosition(const CDM_Annotation Ann,
        const CDM_Position Position) {
  const ELEP::CDM::Annotation *a =
        reinterpret_cast<const ELEP::CDM::Annotation*>(Ann);
  if (a) return a->contains(Position);
  return false;
};

int CDM_AnnotationMatchesRange(const CDM_Annotation Ann,
         const CDM_Position start, const CDM_Position end) {
  const ELEP::CDM::Annotation *a =
        reinterpret_cast<const ELEP::CDM::Annotation*>(Ann);
  if (a) return a->matchesRange(start, end);
  return false;
};
