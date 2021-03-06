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

#include "Expression.h"

using namespace ELEP::CDM;

const Id ELEP::CDM::Annotation::no = UINT32_MAX;

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

ELEP::CDM::Annotation::Annotation(const Id id, const std::string &type, const SpanSet &spans, const AttributeSet &attributes) :
  _id {id}, _spans(spans), _attributes(attributes) {
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

ELEP::CDM::Annotation::Annotation(const Id id, const std::string &type, SpanSet &&spans, AttributeSet &&attributes) :
  _id {id}, _spans(spans), _attributes(attributes) {
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
  return _attributes.containsAttributeMatchingValue(name, pattern);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(
           const std::string& name, const std::string& pattern) const {
  return _attributes.containsAttributeMatchingValue(name, pattern);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(const char *name,
           const std::regex& pattern) const {
  return _attributes.containsAttributeMatchingValue(name, pattern);
};

bool ELEP::CDM::Annotation::containsAttributeMatchingValue(
           const std::string& name, const std::regex& pattern) const {
  return _attributes.containsAttributeMatchingValue(name, pattern);
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
  // Examine spans: the one that preceeds, wins...
  if (_spans.operator<(Annotation._spans)) return true;
  return false;
};

#ifdef TCL_VERSION
ELEP::CDM::Annotation::Annotation(Tcl_Interp *interp, Tcl_Obj *obj) :
  _id {no} {
  Tcl_Obj **items;
  int count, len;
  Tcl_WideInt id;

  /* A valid Annotation is a list with 4 items, or a list of two items... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK) {
    throw std::invalid_argument("invalid Annotation (4-item list expected)");
  }
  if (count != 4) {
    std::ostringstream msg;
    msg << "invalid Annotation (4-item list expected): ";
    msg << "found items: " << count << " (\"" << Tcl_GetString(obj) << "\")";
    throw std::invalid_argument(msg.str());
  }
  /* Get Id: either an integer, or an empty id */
  if (Tcl_GetWideIntFromObj(interp, items[0], &id) != TCL_OK) {
    Tcl_GetStringFromObj(items[0], &len);
    if (len != 0) throw std::invalid_argument("id must be an integral value") ;
  } else {
    _id = id;
  }
  /* Get the type... */
  _type       = Cache::get_string(Tcl_GetString(items[1]));
  _spans      = SpanSet(interp, items[2]);
  _attributes = AttributeSet(interp, items[3]);
};
#endif /* TCL_VERSION */

ELEP::CDM::AnnotationSet::AnnotationSet() :
  set(), _next_ann_id(0), _modified(false) {
};

ELEP::CDM::AnnotationSet::AnnotationSet(const Annotation &annotation):
  set(), _next_ann_id(0), _modified(false) {
  insert(annotation);
};

ELEP::CDM::AnnotationSet::AnnotationSet(Annotation &&annotation):
  set(), _next_ann_id(0), _modified(false) {
  insert(annotation);
};


ELEP::CDM::AnnotationSet::AnnotationSet(const char *type):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const SpanSet &spans):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const SpanSet &spans, const AttributeSet &attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      const AttributeSet &attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      SpanSet &&spans):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      SpanSet &&spans, AttributeSet &&attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const char *type,
      AttributeSet &&attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const SpanSet &spans):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const SpanSet &spans, const AttributeSet &attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      const AttributeSet &attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      SpanSet &&spans):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      SpanSet &&spans, AttributeSet &&attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, spans, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const std::string &type,
      AttributeSet &&attributes):
  set(), _next_ann_id(0), _modified(false) {
  insert(Annotation(type, attributes));
};

ELEP::CDM::AnnotationSet::AnnotationSet(const AnnotationSet& src) :
  set(src.set), _next_ann_id(src._next_ann_id), _modified(src._modified) {
};

#ifdef TCL_VERSION
ELEP::CDM::AnnotationSet::AnnotationSet(Tcl_Interp *interp, Tcl_Obj *obj):
  _next_ann_id(0), set(), _modified(false) {
  Tcl_Obj **items;
  int count;
  /* A valid Annotation set is a list of Annotations... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK) {
    throw Status::ERROR;
  }
  for (int i = 0; i < count; ++i) {
    insert(Annotation(interp, items[i]));
  }
};
#endif /* TCL_VERSION */

void ELEP::CDM::AnnotationSet::mergeAnnotations(const AnnotationSet& other) {
  insert(other.cbegin(), other.cend());
};

void ELEP::CDM::AnnotationSet::concatAnnotations(const AnnotationSet& other) {
  for (const auto& ann : other) {
    Annotation a(ann);
    a.clearId();
    insert(std::move(a));
  }
};

bool ELEP::CDM::AnnotationSet::valid() const {
  if (empty()) return false;
  auto it = cbegin();
  return it->valid();
};

const std::string ELEP::CDM::AnnotationSet::toString() const {
  if (empty()) return "";
  std::ostringstream ss;
  bool add_space = false;
  // ss << '{';
  for (auto it = cbegin(); it != cend(); ++it) {
    if (add_space) ss << ' ';
    ss << '{' << it->toString() << '}';
    add_space = true;
  }
  // ss << '}';
  return ss.str();
};

std::pair<ELEP::CDM::AnnotationSet::iterator, bool>
ELEP::CDM::AnnotationSet::insert(const ELEP::CDM::AnnotationSet::value_type& x) {
  if (_next_ann_id == ELEP::CDM::Annotation::no) {
    // We cannot add any more annotations...
    throw std::overflow_error("maximum number of annotations reached");
  }
  _modified = true;
  if (x.emptyId()) {
    return set.insert(Annotation(_next_ann_id++, x.type(),
                      x._spans, x._attributes));
  }
  if (x._id > _next_ann_id) _next_ann_id = x._id +1;
  std::pair<ELEP::CDM::AnnotationSet::iterator, bool>&& r = set.insert(x);
  if (r.second) return r;
  if (!set.replace(r.first, x)) {
    throw std::runtime_error("replacing annotation failed");
  }
  return std::make_pair(r.first, false);
};

std::pair<ELEP::CDM::AnnotationSet::iterator, bool>
ELEP::CDM::AnnotationSet::insert(ELEP::CDM::AnnotationSet::value_type&& x) {
  if (_next_ann_id == ELEP::CDM::Annotation::no) {
    // We cannot add any more annotations...
    throw std::overflow_error("maximum number of annotations reached");
  }
  _modified = true;
  if (x.emptyId()) {
    x._id = _next_ann_id++;
    return set.insert(x);
  }
  if (x._id > _next_ann_id) _next_ann_id = x._id +1;
  std::pair<ELEP::CDM::AnnotationSet::iterator, bool>&& r = set.insert(x);
  if (r.second) return r;
  if (!set.replace(r.first, x)) {
    throw std::runtime_error("replacing annotation failed");
  }
  return std::make_pair(r.first, false);
};

bool ELEP::CDM::AnnotationSet::displace(const Int displacement) {
  bool result = false;
  for (auto it = cbegin(); it != cend(); ++it) {
    Annotation ann(*it);
    if (ann.displace(displacement)) {
      insert(ann); if(!result) result= true;
    }
  }
  return result;
};

bool ELEP::CDM::AnnotationSet::displace(const Position offset,
                                        const Int displacement) {
  bool result = false;
  for (auto it = cbegin(); it != cend(); ++it) {
    Annotation ann(*it);
    if (ann.displace(offset, displacement)) {
      insert(ann); if(!result) result= true;
    }
  }
  return result;
};

void ELEP::CDM::AnnotationSet::select(AnnotationSet& s, const std::string& type) const {
  auto&& view = set.get<ELEP::CDM::internal::tags::type>();
  auto   p    = view.equal_range(type);
  while (p.first != p.second) {s.insert(*p.first); ++p.first;};
};

void ELEP::CDM::AnnotationSet::select(AnnotationSet& s, const std::string& type,
           const ELEP::CDM::Functor::AnnotationUnaryPredicate &pred) const {
  auto&& view = set.get<ELEP::CDM::internal::tags::type>();
  auto   p    = view.equal_range(type);
  while (p.first != p.second) {
    if (pred(*p.first)) s.insert(*p.first); ++p.first;
  };
};

void ELEP::CDM::AnnotationSet::select(AnnotationSet& s, const std::string& type,
                const std::string &pred) const {
  auto&& view = set.get<ELEP::CDM::internal::tags::type>();
  auto   p    = view.equal_range(type);

  // std::cout << "PRED: " << pred << std::endl;
  // Our AST
  ELEP::CDM::Expression::AST ast;
  // Our Interpreter
  auto interp = ELEP::CDM::Expression::get_interpreter(pred, ast);
  // ELEP::CDM::Expression::ast::print(std::cout, ast);

  while (p.first != p.second) {
    double result = interp.eval(&(*p.first), ast);
    // std::cout << "result: " << result << " (" << p.first->toString()
    //           << ")" << std::endl;
    if (result) s.insert(*p.first); ++p.first;
  };
};

/*
 * CDM_Annotation
 */

CDM_Status CDM_AddSpan(CDM_Annotation Ann, const CDM_Span span) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  const ELEP::CDM::Span *s =
        CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (!s) return CDM_ERROR;
  a->addSpan(*s);
  return CDM_OK;
};

CDM_Status CDM_AddSpan(CDM_Annotation Ann,
               const CDM_Position start, const CDM_Position end) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  a->addSpan(start, end);
  return CDM_OK;
};

CDM_Status CDM_AddAnnotationSpan(CDM_Annotation Ann, const CDM_Span span) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  const ELEP::CDM::Span *s =
        CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (!s) return CDM_ERROR;
  a->addSpan(*s);
  return CDM_OK;
};

CDM_Status CDM_AddAnnotationSpan(CDM_Annotation Ann,
               const CDM_Position start, const CDM_Position end) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  a->addSpan(start, end);
  return CDM_OK;
};

CDM_Annotation CDM_CreateAnnotation(const char *type, const CDM_SpanSet spans,
                                    const CDM_AttributeSet attributes) {
  ELEP::CDM::Annotation *p = nullptr;
  const ELEP::CDM::SpanSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::SpanSet*, spans);
  const ELEP::CDM::AttributeSet *a =
        CDM_CastFromOpaque(const ELEP::CDM::AttributeSet*, attributes);
  if (s) {
    if (a) p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::SpanSet) *s, (const ELEP::CDM::AttributeSet) *a);
    else   p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::SpanSet) *s);
  } else {
    if (a) p = new ELEP::CDM::Annotation(type, (const ELEP::CDM::AttributeSet) *a);
    else   p = new ELEP::CDM::Annotation(type);
  }
  return CDM_CastToOpaque(CDM_Annotation, p);
};

CDM_Annotation CDM_CreateAnnotation(const char *type,
                                    const CDM_Position start,
                                    const CDM_Position end,
                                    const CDM_AttributeSet attributes) {
  ELEP::CDM::Annotation *p = nullptr;
  const ELEP::CDM::AttributeSet *a =
        CDM_CastFromOpaque(const ELEP::CDM::AttributeSet*, attributes);
  if (a) p = new ELEP::CDM::Annotation(type, SpanSet(start, end), *a);
  else   p = new ELEP::CDM::Annotation(type, SpanSet(start, end));
  return CDM_CastToOpaque(CDM_Annotation, p);
};

CDM_Status CDM_DisplaceAnnotation(CDM_Annotation Ann, long displacement) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  if (a->displace(displacement)) return CDM_OK;
  return CDM_FALSE;
};

CDM_Status CDM_DisplaceAnnotation(CDM_Annotation Ann,
                                  long offset, long displacement) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  if (a->displace(offset, displacement)) return CDM_OK;
  return CDM_FALSE;
};

int CDM_AnnotationContainsAttributeMatchingValue(const CDM_Annotation Ann,
        const char *AttributeName, const char *ValuePattern) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) return a->containsAttributeMatchingValue(AttributeName, ValuePattern);
  return false;
};

int CDM_AnnotationContainsAttributeMatchingValues(const CDM_Annotation Ann,
        const char *AttributeName,
        const char **ValuePatterns, const size_t ValuePatternsSize) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (!a) return false;
  for (size_t i=0; i<ValuePatternsSize; ++i) {
    if (!a->containsAttributeMatchingValue(AttributeName, ValuePatterns[i])) return false;
  }
  return true;
};

int CDM_AnnotationContainsPosition(const CDM_Annotation Ann,
        const CDM_Position Position) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) return a->contains(Position);
  return false;
};

int CDM_AnnotationContainsPositions(const CDM_Annotation Ann,
        const size_t items, const CDM_Position *Positions) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) return a->contains(items, Positions);
  return false;
};

int CDM_AnnotationMatchesRange(const CDM_Annotation Ann,
         const CDM_Position start, const CDM_Position end) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) return a->matchesRange(start, end);
  return false;
};

int CDM_CompareAnnotations(const CDM_Annotation Ann1,
                           const CDM_Annotation Ann2) {
  const ELEP::CDM::Annotation *a1 =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann1);
  const ELEP::CDM::Annotation *a2 =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann2);
  if (!a1) return  1;
  if (!a2) return -1;
  if (*a1 < *a2) return -1;
  if (*a2 < *a1) return  1;
  // The spans are equal...
  return 0;
};

const CDM_SpanSet CDM_GetSpans(const CDM_Annotation Ann) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) return (const CDM_SpanSet) &(a->spans());
  return nullptr;
};

const CDM_Span CDM_GetAnnotationFirstSpan(const CDM_Annotation Ann) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) {
    if (a->spans().empty()) return nullptr;
    return (const CDM_Span) &(*(a->spans().cbegin()));
  }
  return nullptr;
};

CDM_Status CDM_GetAnnotationFirstSpanOffsets(const CDM_Annotation Ann,
                                   CDM_Position *start, CDM_Position *end) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  if (a) {
    if (a->spans().empty()) return CDM_ERROR;
    if (start) *start = a->spans().begin()->start();
    if (end)   *end   = a->spans().begin()->end();
    return CDM_OK;
  }
  return CDM_ERROR;
};

const CDM_Attribute CDM_GetAttribute(const CDM_Annotation Annotation,
                                     const char *Name) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Annotation);
  if (a) {
    auto it = a->find(Name);
    if (it != a->attributes().end()) return (const CDM_Attribute) &(*it);
  }
  return nullptr;
};

const CDM_AttributeSet CDM_GetAttributes(const CDM_Annotation Annotation) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Annotation);
  if (a) return (const CDM_AttributeSet) &(a->attributes());
  return nullptr;
};

CDM_AttributeValue CDM_GetAttributeValue(const CDM_Annotation Annotation,
                        const char *Name, const CDM_AttributeValue def) {
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Annotation);
  if (a) {
    auto it = a->find(Name);
    if (it != a->attributes().end()) {
      return CDM_CastToOpaque(CDM_AttributeValue,
                 new ELEP::CDM::AttributeValue(it->value(), it->type()));
    }
  }
  if (def == nullptr) return nullptr;
  const AttributeValue *av = CDM_CastFromOpaque(const AttributeValue *, def);
  return CDM_CastToOpaque(CDM_AttributeValue,
                 new ELEP::CDM::AttributeValue(av->value(), av->type()));
};

CDM_Status CDM_PutAttribute(CDM_Annotation Ann, const CDM_Attribute Attr) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  const ELEP::CDM::Attribute *at =
        CDM_CastFromOpaque(const ELEP::CDM::Attribute*, Attr);
  if (!at) return CDM_ERROR;
  a->putAttribute(*at);
  return CDM_OK;
};

CDM_Status CDM_RemoveAttribute(CDM_Annotation Ann, const char *Name) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  a->removeAttribute(Name);
  return CDM_OK;
};

CDM_Status CDM_RemoveSpan(CDM_Annotation Ann, const CDM_Span span) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  const ELEP::CDM::Span *s =
        CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (!s) return CDM_ERROR;
  a->removeSpan(*s);
  return CDM_OK;
};

CDM_Status CDM_RemoveSpan(CDM_Annotation Ann,
                          const CDM_Position start, const CDM_Position end) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return CDM_ERROR;
  a->removeSpan(start, end);
  return CDM_OK;
};

const CDM_ByteSequence CDM_GetType(const CDM_Annotation Ann) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return nullptr;
  return (const CDM_ByteSequence) a->type().c_str();
};

CDM_Id CDM_GetId(const CDM_Annotation Ann) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a) return ELEP::CDM::Annotation::no;
  return a->id();
};

CDM_ByteSequence CDM_GetFirstAnnotatedTextRange(
  const CDM_ByteSequence text, const CDM_Annotation Ann) {
  ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(ELEP::CDM::Annotation*, Ann);
  if (!a || !text) return Utilities::CDM_StringToCString("");
  // std::cout << a->toString() << std::endl;
  return Utilities::CDM_StringToCString(a->textRange(text));
};

/*
 * CDM_AnnotationSet
 */
CDM_AnnotationSet CDM_CreateAnnotationSet() {
  return CDM_CastToOpaque(CDM_AnnotationSet, new ELEP::CDM::AnnotationSet() );
};

CDM_Id CDM_AddAnnotationToSet(CDM_AnnotationSet Set, const CDM_Annotation Ann) {
  if (!Set || !Ann) return ELEP::CDM::Annotation::no;
  ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(ELEP::CDM::AnnotationSet*, Set);
  const ELEP::CDM::Annotation *a =
        CDM_CastFromOpaque(const ELEP::CDM::Annotation*, Ann);
  return s->addAnnotation(*a);
};

CDM_Size CDM_Length(const CDM_AnnotationSet Set) {
  const ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set);
  if (!s) return 0;
  return s->size();
};

const CDM_Annotation CDM_Nth(const CDM_AnnotationSet Set, CDM_Size n) {
  const ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set);
  if (!s) return nullptr;
  return (const CDM_Annotation) &(s->at(n));
};

CDM_Status CDM_MergeAnnotations(CDM_AnnotationSet Set1,
                                const CDM_AnnotationSet Set2) {
  ELEP::CDM::AnnotationSet *s1 =
        CDM_CastFromOpaque(ELEP::CDM::AnnotationSet*, Set1);
  if (!s1) return CDM_ERROR;
  const ELEP::CDM::AnnotationSet *s2 =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set2);
  if (!s2) return CDM_ERROR;
  s1->mergeAnnotations(*s2);
  return CDM_OK;
};

CDM_Status CDM_ConcatAnnotations(CDM_AnnotationSet Set1,
                                const CDM_AnnotationSet Set2) {
  ELEP::CDM::AnnotationSet *s1 =
        CDM_CastFromOpaque(ELEP::CDM::AnnotationSet*, Set1);
  if (!s1) return CDM_ERROR;
  const ELEP::CDM::AnnotationSet *s2 =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set2);
  if (!s2) return CDM_ERROR;
  s1->concatAnnotations(*s2);
  return CDM_OK;
};

const CDM_Annotation CDM_GetAnnotation(const CDM_AnnotationSet Set,
                                       const CDM_Id Id) {
  const ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set);
  if (!s) return nullptr;
  return (const CDM_Annotation) (&(s->get(Id)));
};

CDM_AnnotationSet CDM_SelectAnnotations(const CDM_AnnotationSet Set,
                                        const char *Type) {
  const ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set);
  if (!s) return nullptr;
  ELEP::CDM::AnnotationSet *set = new ELEP::CDM::AnnotationSet();
  s->select(*set, Type);
  return CDM_CastToOpaque(CDM_AnnotationSet, set);
};

CDM_AnnotationSet CDM_SelectAnnotations(const CDM_AnnotationSet Set,
                      const char *Type, const char *Constraints) {
  const ELEP::CDM::AnnotationSet *s =
        CDM_CastFromOpaque(const ELEP::CDM::AnnotationSet*, Set);
  if (!s) return nullptr;
  ELEP::CDM::AnnotationSet *set = new ELEP::CDM::AnnotationSet();
  s->select(*set, Type, Constraints);
  return CDM_CastToOpaque(CDM_AnnotationSet, set);
};
