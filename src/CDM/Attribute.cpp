/*****************************************************************************
 *  Attribute.cpp:                                                           *
 * ----------------------                                                    *
 *  This file implements the Attribute section of the                        *
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
#include <stdexcept>   /* For std::invalid_argument */
#include <algorithm>   /* For std::find             */
#include <regex>       /* For regular expressions   */

using namespace ELEP::CDM;

void ELEP::CDM::AttributeValue::store_value(const std::string &value) {
  switch (_type) {
    case AttributeType::NONE: {break;}
    case AttributeType::STRING: {
      /* Create a new string, and add it to the cache... */
      _v_string = Cache::get_string(value);
      break;
    }
    default: _v_string = Cache::get_string(value);
  }
};

ELEP::CDM::AttributeValue::AttributeValue() :
  _type {AttributeType::NONE} {
};

ELEP::CDM::AttributeValue::AttributeValue(const char *value,
                                          const AttributeType type) :
  _type {type} {
  store_value(value);
};

ELEP::CDM::AttributeValue::AttributeValue(const std::string &value,
                                          const AttributeType type) :
  _type {type} {
  store_value(value);
};

ELEP::CDM::AttributeValue::AttributeValue(const AttributeValue& src) :
  _type(src._type),  _v_string(src._v_string) {
};

const AttributeType& ELEP::CDM::AttributeValue::type() const {
  return _type;
};

const std::string& ELEP::CDM::AttributeValue::value() const {
  if (!_v_string) throw std::invalid_argument("undefined value");
  return *_v_string;
};

bool ELEP::CDM::AttributeValue::valueMatches(const char *pattern) const {
  return std::regex_match(value(), std::regex(pattern));
};

bool ELEP::CDM::AttributeValue::valueMatches(const std::string& pattern) const {
  return std::regex_match(value(), std::regex(pattern));
};

bool ELEP::CDM::AttributeValue::valueMatches(const std::regex& pattern) const {
  return std::regex_match(value(), pattern);
};

bool ELEP::CDM::AttributeValue::valid() const {
  return _v_string && _type != AttributeType::NONE;
};

const std::string ELEP::CDM::AttributeValue::toString() const {
  std::ostringstream ss;
  switch (_type) {
    case AttributeType::NONE:          {ss << "CDM_NONE ";          break;}
    case AttributeType::STRING:        {ss << "CDM_STRING ";        break;}
    case AttributeType::STRING_SET:    {ss << "CDM_STRING_SET ";    break;}
    case AttributeType::SPAN:          {ss << "CDM_SPAN ";          break;}
    case AttributeType::SPAN_SET:      {ss << "CDM_SPAN_SET ";      break;}
    case AttributeType::ANNOTATION_ID: {ss << "CDM_ANNOTATION_ID "; break;}
    case AttributeType::ANNOTATION_ID_SET: {
                        ss << "CDM_ANNOTATION_ID_SET ";             break;}
    case AttributeType::IMAGE:         {ss << "CDM_IMAGE ";         break;}
    case AttributeType::BASE64_IMAGE:  {ss << "CDM_BASE64_IMAGE ";  break;}
  }
  ss << '{' << ((_v_string) ?
     ELEP::CDM::Utilities::ensure_list_element(*_v_string) : "") << '}';
  return ss.str();
};

bool ELEP::CDM::AttributeValue::operator< (const AttributeValue& AttributeValue) const {
  if (_type < AttributeValue._type) {
    return true;
  }
  return false;
};

#ifdef TCL_VERSION
ELEP::CDM::AttributeValue::AttributeValue(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  static const char *Types[] = {
    "CDM_NONE", "CDM_STRING", "CDM_STRING_SET", "CDM_SPAN", "CDM_SPAN_SET",
    "CDM_ANNOTATION_ID", "CDM_ANNOTATION_ID_SET",
    "CDM_IMAGE", "CDM_BASE64_IMAGE",
    (char *) NULL
  };
  enum types {
    NONE, STRING, STRING_SET, SPAN, SPAN_SET, ANNOTATION_ID, ANNOTATION_ID_SET,
    IMAGE, BASE64_IMAGE
  };

  /* A valid AttributeValue is a list with 2 items... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count != 2) {
    std::string msg("invalid AttributeValue (2-item list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetIndexFromObj(interp, items[0], (const char **) Types,
                              "type", 0, &count) != TCL_OK) {
    std::string msg("invalid AttributeValue type: ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  switch ((types) count) {
    case NONE:              {_type = AttributeType::NONE;              break;}
    case STRING:            {_type = AttributeType::STRING;            break;}
    case STRING_SET:        {_type = AttributeType::STRING_SET;        break;}
    case SPAN:              {_type = AttributeType::SPAN;              break;}
    case SPAN_SET:          {_type = AttributeType::SPAN_SET;          break;}
    case ANNOTATION_ID:     {_type = AttributeType::ANNOTATION_ID;     break;}
    case ANNOTATION_ID_SET: {_type = AttributeType::ANNOTATION_ID_SET; break;}
    case IMAGE:             {_type = AttributeType::IMAGE;             break;}
    case BASE64_IMAGE:      {_type = AttributeType::BASE64_IMAGE;      break;}
  }
  store_value(Tcl_GetString(items[1]));
};
#endif /* TCL_VERSION */

void ELEP::CDM::Attribute::store_value(const std::string &value) {
  switch (_type) {
    case AttributeType::NONE: {break;}
    case AttributeType::STRING: {
      /* Create a new string, and add it to the cache... */
      _v_string = Cache::get_string(value);
      break;
    }
    default: _v_string = Cache::get_string(value);
  }
};

ELEP::CDM::Attribute::Attribute() :
  _type {AttributeType::NONE}, _v_string() {
  _name = Cache::get_string("");
};

ELEP::CDM::Attribute::Attribute(const char *name) :
  _type {AttributeType::NONE}, _v_string() {
  _name = Cache::get_string(name);
};

ELEP::CDM::Attribute::Attribute(const std::string &name) :
  _type {AttributeType::NONE}, _v_string() {
  _name = Cache::get_string(name);
};

ELEP::CDM::Attribute::Attribute(const char *name, const char *value, const AttributeType type) :
  _type {type} {
  _name = Cache::get_string(name);
  if (value == nullptr) value = "";
  store_value(value);
};

ELEP::CDM::Attribute::Attribute(const std::string &name, const std::string &value, const AttributeType type) :
  _type {type} {
  _name = Cache::get_string(name);
  store_value(value);
};

ELEP::CDM::Attribute::Attribute(const char *name,
                                const AttributeValue &value) {
  _name = Cache::get_string(name);
  _type = value.type();
  store_value(value.value());
};

ELEP::CDM::Attribute::Attribute(const std::string &name,
                                const AttributeValue &value) {
  _name = Cache::get_string(name);
  _type = value.type();
  store_value(value.value());
};

ELEP::CDM::Attribute::Attribute(const Attribute& src) :
  _type(src._type), _name(src._name) {
  store_value(src.value());
};

const std::string& ELEP::CDM::Attribute::name() const {
  if (!_name) throw std::invalid_argument("undefined name");
  return *_name;
};

const AttributeType& ELEP::CDM::Attribute::type() const {
  return _type;
};

const std::string& ELEP::CDM::Attribute::value() const {
  if (!_v_string) throw std::invalid_argument("undefined value");
  return *_v_string;
};

bool ELEP::CDM::Attribute::valueMatches(const char *pattern) const {
  return std::regex_match(value(), std::regex(pattern));
};

bool ELEP::CDM::Attribute::valueMatches(const std::string& pattern) const {
  return std::regex_match(value(), std::regex(pattern));
};

bool ELEP::CDM::Attribute::valueMatches(const std::regex& pattern) const {
  return std::regex_match(value(), pattern);
};

bool ELEP::CDM::Attribute::valid() const {
  return _name && _name->size() && _type != AttributeType::NONE && _v_string;
};

const std::string ELEP::CDM::Attribute::toString() const {
  std::ostringstream ss;
  if (_name && _name->size()) {
    ss << ELEP::CDM::Utilities::ensure_list_element(*_name);
  } else {
    ss << "{}";
  }
  ss << " {";
  switch (_type) {
    case AttributeType::NONE:          {ss << "CDM_NONE ";          break;}
    case AttributeType::STRING:        {ss << "CDM_STRING ";        break;}
    case AttributeType::STRING_SET:    {ss << "CDM_STRING_SET ";    break;}
    case AttributeType::SPAN:          {ss << "CDM_SPAN ";          break;}
    case AttributeType::SPAN_SET:      {ss << "CDM_SPAN_SET ";      break;}
    case AttributeType::ANNOTATION_ID: {ss << "CDM_ANNOTATION_ID "; break;}
    case AttributeType::ANNOTATION_ID_SET: {
                        ss << "CDM_ANNOTATION_ID_SET ";             break;}
    case AttributeType::IMAGE:         {ss << "CDM_IMAGE ";         break;}
    case AttributeType::BASE64_IMAGE:  {ss << "CDM_BASE64_IMAGE ";  break;}
    default: {}
  }
  ss << '{' << (_v_string ?
     ELEP::CDM::Utilities::ensure_list_element(*_v_string) : "") << "}}";
  return ss.str();
};

bool ELEP::CDM::Attribute::operator< (const Attribute& Attribute) const {
  if (_name < Attribute._name) return true;
  return false;
};

#ifdef TCL_VERSION
ELEP::CDM::Attribute::Attribute(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items, *name = nullptr, *type = nullptr, *value = nullptr;
  int count;
  static const char *Types[] = {
    "CDM_NONE", "CDM_STRING", "CDM_STRING_SET", "CDM_SPAN", "CDM_SPAN_SET",
    "CDM_ANNOTATION_ID", "CDM_ANNOTATION_ID_SET",
    "CDM_IMAGE", "CDM_BASE64_IMAGE",
    (char *) NULL
  };
  enum types {
    NONE, STRING, STRING_SET, SPAN, SPAN_SET, ANNOTATION_ID, ANNOTATION_ID_SET,
    IMAGE, BASE64_IMAGE
  };

  /* A valid Attribute is a list with 3 items, or a list of two items... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count < 2 || count > 3) {
    std::string msg("invalid Attribute (2|3-item list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  name = items[0];
  if (count == 3) {
    type = items[1]; value = items[2];
  } else {
    if (Tcl_ListObjGetElements(interp, items[1], &count, &items) != TCL_OK ||
                               count != 2) {
      std::string msg("invalid Attribute AttributeValue (2-item list expected): ");
      msg += Tcl_GetStringResult(interp);
      throw Status::ERROR;
    }
    type = items[0]; value = items[1];
  }
  if (Tcl_GetIndexFromObj(interp, type, (const char **) Types,
                              "type", 0, &count) != TCL_OK) {
    std::string msg("invalid Attribute AttributeValue type: ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  switch ((types) count) {
    case NONE:              {_type = AttributeType::NONE;              break;}
    case STRING:            {_type = AttributeType::STRING;            break;}
    case STRING_SET:        {_type = AttributeType::STRING_SET;        break;}
    case SPAN:              {_type = AttributeType::SPAN;              break;}
    case SPAN_SET:          {_type = AttributeType::SPAN_SET;          break;}
    case ANNOTATION_ID:     {_type = AttributeType::ANNOTATION_ID;     break;}
    case ANNOTATION_ID_SET: {_type = AttributeType::ANNOTATION_ID_SET; break;}
    case IMAGE:             {_type = AttributeType::IMAGE;             break;}
    case BASE64_IMAGE:      {_type = AttributeType::BASE64_IMAGE;      break;}
  }
  _name = Cache::get_string(Tcl_GetString(name));
  store_value(Tcl_GetString(value));
};
#endif /* TCL_VERSION */

ELEP::CDM::AttributeSet::AttributeSet() : set() {
};

ELEP::CDM::AttributeSet::AttributeSet(const Attribute &attribute):
  set(1, attribute) {
};

ELEP::CDM::AttributeSet::AttributeSet(const char *name, const AttributeValue &value):
  set(1, Attribute(name, value)) {
};

ELEP::CDM::AttributeSet::AttributeSet(const std::string &name, const AttributeValue &value):
  set(1, Attribute(name, value)) {
};

ELEP::CDM::AttributeSet::AttributeSet(const char *name, const char *value, const AttributeType type):
  set(1, Attribute(name, value, type)) {
};

ELEP::CDM::AttributeSet::AttributeSet(const std::string &name, const std::string &value, const AttributeType type):
  set(1, Attribute(name, value, type)) {
};

ELEP::CDM::AttributeSet::AttributeSet(const AttributeSet& src) : set(src.set) {
};

#ifdef TCL_VERSION
ELEP::CDM::AttributeSet::AttributeSet(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  /* A valid Attribute set is a list of Attributes... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK) {
    std::string msg("invalid AttributeSet (list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  for (int i = 0; i < count; ++i) {
    set.push_back(Attribute(interp, items[i]));
  }
};
#endif /* TCL_VERSION */

bool ELEP::CDM::AttributeSet::containsAttributeMatchingValue(const char *name,
           const char *pattern) const {
  auto attr = find(name);
  if (attr == set.end()) return false;
  if (!pattern) return true;
  return std::regex_match(attr->value(), std::regex(pattern));
};

bool ELEP::CDM::AttributeSet::containsAttributeMatchingValue(
           const std::string& name, const std::string& pattern) const {
  auto attr = find(name);
  if (attr == set.end()) return false;
  if (!pattern.size()) return true;
  return std::regex_match(attr->value(), std::regex(pattern));
};

bool ELEP::CDM::AttributeSet::containsAttributeMatchingValue(const char *name,
           const std::regex& pattern) const {
  auto attr = find(name);
  if (attr == set.end()) return false;
  return std::regex_match(attr->value(), pattern);
};

bool ELEP::CDM::AttributeSet::containsAttributeMatchingValue(
           const std::string& name, const std::regex& pattern) const {
  auto attr = find(name);
  if (attr == set.end()) return false;
  return std::regex_match(attr->value(), pattern);
};

void ELEP::CDM::AttributeSet::removeAttribute(const char *name) {
  auto attr = find(name);
  if (attr != set.end()) erase(attr);
};

bool ELEP::CDM::AttributeSet::valid() const {
  if (set.empty()) return false;
  auto it = set.cbegin();
  return it->valid();
};

const std::string ELEP::CDM::AttributeSet::toString() const {
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

void ELEP::CDM::AttributeSet::ensure_unique() {
};

ELEP::CDM::AttributeSet::iterator
ELEP::CDM::AttributeSet::find(const char *name) {
  return std::find_if(set.begin(), set.end(),
    [&](const Attribute& n){return n.name() == name;});
};

ELEP::CDM::AttributeSet::iterator
ELEP::CDM::AttributeSet::find(const std::string &name) {
  return std::find_if(set.begin(), set.end(),
    [&](const Attribute& n){return n.name() == name;});
};

ELEP::CDM::AttributeSet::const_iterator
ELEP::CDM::AttributeSet::find(const char *name) const {
  return std::find_if(set.cbegin(), set.cend(),
    [&](const Attribute& n){return n.name() == name;});
};

ELEP::CDM::AttributeSet::const_iterator
ELEP::CDM::AttributeSet::find(const std::string &name) const {
  return std::find_if(set.cbegin(), set.cend(),
    [&](const Attribute& n){return n.name() == name;});
};

void ELEP::CDM::AttributeSet::assert_not_exists(const value_type& val) const {
  if (exists(val.name())) {
    std::string msg("attribute with name \"");
    msg += val.name();
    msg += "\" alread exists";
    throw std::invalid_argument(msg);
  }
};

void ELEP::CDM::AttributeSet::push_back(const Attribute& val) {
  // Do we have an item with the same name? If yes, overwrite it...
  auto it = find(val.name());
  if (it == set.end()) {
    set.push_back(val);
  } else {
    *it = val;
  }
};

void ELEP::CDM::AttributeSet::push_back(Attribute&& val) {
  // Do we have an item with the same name? If yes, overwrite it...
  auto it = find(val.name());
  if (it == set.end()) {
    set.push_back(val);
  } else {
    *it = val;
  }
};

CDM_AttributeValue CDM_CreateAttributeValue(const CDM_AttributeType type,
                                            const char *value) {
  return new ELEP::CDM::AttributeValue(value, convert::AttributeType(type));
};

CDM_Attribute CDM_CreateAttribute(const char *name,
                                  const CDM_AttributeValue value) {
  const ELEP::CDM::AttributeValue *p =
        static_cast<const ELEP::CDM::AttributeValue*>(value);
  if (p) return new ELEP::CDM::Attribute(name, *p);
  return new ELEP::CDM::Attribute(name);
};

CDM_Attribute CDM_CreateAttribute(const char *name, const char *value, const CDM_AttributeType type) {
  return new ELEP::CDM::Attribute(name, value, convert::AttributeType(type));
};

const char *CDM_GetName(const CDM_Attribute Attribute) {
  const ELEP::CDM::Attribute *p =
        static_cast<const ELEP::CDM::Attribute*>(Attribute);
  if (p) return p->name().c_str();
  return NULL;
};

CDM_AttributeValue CDM_GetValue(const CDM_Attribute Attribute) {
  const ELEP::CDM::Attribute *p =
        static_cast<const ELEP::CDM::Attribute*>(Attribute);
  if (p) return new ELEP::CDM::AttributeValue(p->value(), p->type());
  return NULL;
};

CDM_AttributeType CDM_GetValueType(const CDM_Attribute Attribute) {
  const ELEP::CDM::Attribute *p =
        static_cast<const ELEP::CDM::Attribute*>(Attribute);
  if (p) return (CDM_AttributeType) convert::AttributeType(p->type());
  return CDM_NONE;
};

const char *CDM_GetValueValue(const CDM_Attribute Attribute) {
  const ELEP::CDM::Attribute *p =
        static_cast<const ELEP::CDM::Attribute*>(Attribute);
  if (p) return p->value().c_str();
  return NULL;
};
const char *CDM_GetValueString(const CDM_Attribute Attribute) {
  return CDM_GetValueValue(Attribute);
};

CDM_AttributeSet CDM_CreateAttributeSet() {
  return new ELEP::CDM::AttributeSet();
};

CDM_AttributeSet CDM_CreateAttributeSet(const CDM_Attribute Attribute) {
  const ELEP::CDM::Attribute *p =
        static_cast<const ELEP::CDM::Attribute*>(Attribute);
  if (p) return new ELEP::CDM::AttributeSet(*p);
  return new ELEP::CDM::AttributeSet();
};

CDM_AttributeSet CDM_CreateAttributeSet(const char *name, const CDM_AttributeValue value) {
  const ELEP::CDM::AttributeValue *p =
        static_cast<const ELEP::CDM::AttributeValue*>(value);
  if (p) return new ELEP::CDM::AttributeSet(name, *p);
  return nullptr;
};

CDM_AttributeSet CDM_CreateAttributeSet(const char *name, const char *value,
                             const CDM_AttributeType type) {
  return new ELEP::CDM::AttributeSet(name, value, convert::AttributeType(type));
};

CDM_Status CDM_AddAttribute(CDM_AttributeSet Attributes,
                            const CDM_Attribute Attribute) {
  ELEP::CDM::AttributeSet *p =
        static_cast<ELEP::CDM::AttributeSet*>(Attributes);
  if (!p) return CDM_ERROR;
  ELEP::CDM::Attribute *a =
        static_cast<ELEP::CDM::Attribute*>(Attribute);
  if (!a) return CDM_ERROR;
  p->push_back(*a);
  return CDM_OK;
};

CDM_Status CDM_AddAttribute(CDM_AttributeSet Attributes, const char *name,
                            const CDM_AttributeValue value) {
  ELEP::CDM::AttributeSet *p =
        static_cast<ELEP::CDM::AttributeSet*>(Attributes);
  if (!p) return CDM_ERROR;
  ELEP::CDM::AttributeValue *v =
        static_cast<ELEP::CDM::AttributeValue*>(value);
  if (!v) return CDM_ERROR;
  p->push_back(Attribute(name, *v));
  return CDM_OK;
};
