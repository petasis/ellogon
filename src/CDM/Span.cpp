/*****************************************************************************
 *  Span.cpp:                                                                *
 * ----------------------                                                    *
 *  This file implements the Span section of the                             *
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
#include <algorithm>   /* For std::for_each         */

using namespace ELEP::CDM;

const Position ELEP::CDM::Span::no = UINT32_MAX;

#ifdef    ELLOGON_CDM_SPAN_USE_PAIR

ELEP::CDM::Span::Span() :
  segment {ELEP::CDM::Span::no, ELEP::CDM::Span::no} {
};

ELEP::CDM::Span::Span(const Position start, const Position end) :
  segment {start, end} {
};

ELEP::CDM::Span::Span(const Span& src) : segment {src.segment} {
};

const Position& ELEP::CDM::Span::start() const {
  return segment.first;
};

const Position& ELEP::CDM::Span::end() const {
  return segment.second;
};

bool ELEP::CDM::Span::valid() const {
  return segment.first != ELEP::CDM::Span::no && segment.second != ELEP::CDM::Span::no;
};

const std::string ELEP::CDM::Span::toString() const {
  if (!valid()) return "{}";
  std::ostringstream ss;
  ss << segment.first << " " << segment.second;
  return ss.str();
};

bool ELEP::CDM::Span::operator< (const Span& span) const {
  if (segment.first < span.segment.first ||
       (segment.first == span.segment.first &&
        segment.second < span.segment.second)) {
    return true;
  }
  return false;
};

#else  /* ELLOGON_CDM_SPAN_USE_PAIR */
ELEP::CDM::Span::Span() :
  _start {ELEP::CDM::Span::no}, _end {ELEP::CDM::Span::no} {
};

ELEP::CDM::Span::Span(const Position start, const Position end) {
  if (start == no)  throw std::invalid_argument("start cannot be equal to ELEP::CDM::Span::no");
  if (end   == no)  throw std::invalid_argument("end cannot be equal to ELEP::CDM::Span::no");
  if (start > end) throw std::invalid_argument("start exceeds end");
  _start = start; _end = end;
};

ELEP::CDM::Span::Span(const Span& src) :
  _start {src._start}, _end {src._end} {
};

const Position& ELEP::CDM::Span::start() const {
  return _start;
};

const Position& ELEP::CDM::Span::end() const {
  return _end;
};

void ELEP::CDM::Span::start(const Position start) {
  if (start == no)  throw std::invalid_argument("start cannot be equal to ELEP::CDM::Span::no");
  if (start > _end) throw std::invalid_argument("start exceeds end");
  _start = start;
};

void ELEP::CDM::Span::end(const Position end) {
  if (end   == no)  throw std::invalid_argument("end cannot be equal to ELEP::CDM::Span::no");
  if (_start > end) throw std::invalid_argument("start exceeds end");
  _end = end;
};

std::string ELEP::CDM::Span::textRange(const std::string& text) const {
  if (!valid()) return "";
  return Unicode::utf8_substr(text, _start, _end);
};

bool ELEP::CDM::Span::textRange(const std::string& text,
                  size_t *byte_start, size_t *byte_end) const {
  if (!valid()) return false;
  return Unicode::utf8_byteOffsets(text, _start, _end, byte_start, byte_end);
};

bool ELEP::CDM::Span::valid() const {
  return _end   != ELEP::CDM::Span::no &&
         _start <= _end;
};

const std::string ELEP::CDM::Span::toString() const {
  std::ostringstream ss;
  ss << _start << " " << _end;
  return ss.str();
};

bool ELEP::CDM::Span::operator< (const Span& span) const {
  if (_start < span._start ||
       (_start == span._start && _end < span._end)) {
    return true;
  }
  return false;
};
#endif /* ELLOGON_CDM_SPAN_USE_PAIR */

#ifdef TCL_VERSION
ELEP::CDM::Span::Span(Tcl_Interp *interp, Tcl_Obj *obj) {
  Tcl_Obj **items;
  int count;
  Tcl_WideInt start, end;

  /* A valid span is a list with 2 integers... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK ||
      count != 2) {
    std::string msg("invalid Span (2-item list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[0], &start) != TCL_OK) {
    std::string msg("invalid Span start (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (Tcl_GetWideIntFromObj(interp, items[1], &end) != TCL_OK) {
    std::string msg("invalid Span end (unsigned integer expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  if (start == no)  throw std::invalid_argument("start cannot be equal to ELEP::CDM::Span::no");
  if (end   == no)  throw std::invalid_argument("end cannot be equal to ELEP::CDM::Span::no");
  if (start > end)  throw std::invalid_argument("start exceeds end");
#ifdef    ELLOGON_CDM_SPAN_USE_PAIR
  segment.first  = start;
  segment.second = end;
#else  /* ELLOGON_CDM_SPAN_USE_PAIR */
  _start = start;
  _end   = end;
#endif /* ELLOGON_CDM_SPAN_USE_PAIR */

};
#endif /* TCL_VERSION */

Status ELEP::CDM::Span::offsets(Position &s, Position &e) const {
  s = _start; e = _end;
  return Status::OK;
};

bool ELEP::CDM::Span::contains(const std::vector<Position> &p) const {
  return std::all_of(p.cbegin(), p.cend(),
         [this](const Position& p){return this->contains(p);});
};

bool ELEP::CDM::Span::contains(const size_t items, const Position *p) const {
  for (size_t i = 0; i < items; ++i, ++p) {
    if (!contains(*p)) return false;
  }
  return true;
};

bool ELEP::CDM::Span::displace(const Int displacement) {
  if (displacement < 0 && -displacement > _start) {
    std::ostringstream out;
    out << "displacing span with start=" << _start << " with displacement="
        << displacement << " will result in a negative offset";
    throw std::runtime_error(out.str());
  }
  _start += displacement; _end += displacement;
  return true;
};

bool ELEP::CDM::Span::displace(const Position offset, const Int displacement) {
  if (displacement < 0 && -displacement > _start) {
    std::ostringstream out;
    out << "displacing span with start=" << _start << " with displacement="
        << displacement << " will result in a negative offset";
    throw std::runtime_error(out.str());
  }
  if (_start >= offset) {
    _start += displacement; _end += displacement;
    return true;
  } else if (_end >= offset) {
    _end += displacement;
    return true;
  }
  return false;
};

bool ELEP::CDM::Span::matchesRange(const Position s, const Position e) const {
  return s == _start && e == _end;
};

ELEP::CDM::SpanSet::SpanSet() : set(), _min {ELEP::CDM::Span::no}, _max {0} {
};

ELEP::CDM::SpanSet::SpanSet(const Position start, const Position end):
  set(1, Span(start, end)), _min {start}, _max {end} {
};

ELEP::CDM::SpanSet::SpanSet(const SpanSet& src) : set(src.set), _min(src._min), _max(src._max) {
};

#ifdef TCL_VERSION
ELEP::CDM::SpanSet::SpanSet(Tcl_Interp *interp, Tcl_Obj *obj) : set(), _min {ELEP::CDM::Span::no}, _max {0} {
  Tcl_Obj **items;
  int count;
  /* A valid span set is a list of spans... */
  if (Tcl_ListObjGetElements(interp, obj, &count, &items) != TCL_OK) {
    std::string msg("invalid SpanSet (list expected): ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  for (int i = 0; i < count; ++i) {
    push_back(Span(interp, items[i]));
  }
};
#endif /* TCL_VERSION */

const Position& ELEP::CDM::SpanSet::firstSpanStart() const {
  if (set.empty()) return ELEP::CDM::Span::no;
  return set.cbegin()->start();
};

const Position& ELEP::CDM::SpanSet::firstSpanEnd() const {
  if (set.empty()) return ELEP::CDM::Span::no;
  return set.cbegin()->end();
};

Status ELEP::CDM::SpanSet::firstSpanOffsets(Position &start, Position &end) const {
  if (set.empty()) return Status::ERROR;
  return set.cbegin()->offsets(start, end);
};

bool ELEP::CDM::SpanSet::contains(const std::vector<Position> &p) const {
  return std::all_of(p.cbegin(), p.cend(),
         [this](const Position& p){return this->contains(p);});
};

bool ELEP::CDM::SpanSet::contains(const size_t items, const Position *p) const {
  for (size_t i = 0; i < items; ++i, ++p) {
    if (!contains(*p)) return false;
  }
  return true;
};

bool ELEP::CDM::SpanSet::displace(const Int displacement) {
  std::for_each(set.begin(), set.end(),
    [displacement](Span& s){s.displace(displacement);});
  _min += displacement; _max += displacement;
  return true;
};

bool ELEP::CDM::SpanSet::displace(const Position offset,
                                  const Int displacement) {
  if (_max < offset) return true;
  std::for_each(set.begin(), set.end(),
    [offset, displacement](Span& s){s.displace(offset, displacement);});
  if (_min >= offset) {
    _min += displacement; _max += displacement;
  } else if (_max >= offset) {
    _max += displacement;
  }
  return true;
};

const Position& ELEP::CDM::SpanSet::min() const {
  return _min;
};

const Position& ELEP::CDM::SpanSet::max() const {
  return _max;
};

bool ELEP::CDM::SpanSet::matchesRange(const Position s, const Position e) const {
  return std::any_of(set.cbegin(), set.cend(), [=](const Span& span) {
     return span.matchesRange(s, e);
  });
};

std::string ELEP::CDM::SpanSet::textRange(const std::string& text) const {
  if (set.empty()) return "";
  return set.cbegin()->textRange(text);
};

bool ELEP::CDM::SpanSet::textRange(const std::string& text,
           size_t *byte_start, size_t *byte_end) const {
  if (set.empty()) return false;
  return set.cbegin()->textRange(text, byte_start, byte_end);
};

std::vector<std::string>
ELEP::CDM::SpanSet::textRanges(const std::string& text) const {
  std::vector<std::string> ranges;
  for (auto s : set) {
    ranges.push_back(s.textRange(text));
  }
  return ranges;
};

std::vector<std::pair<size_t, size_t>>
ELEP::CDM::SpanSet::textRangesOffsets(const std::string& text) const {
  std::vector<std::pair<size_t, size_t>> ranges;
  size_t start, end;
  for (auto s : set) {
    if (s.textRange(text, &start, &end)) {
      ranges.push_back(std::make_pair(start, end));
    }
  }
  return ranges;
};

bool ELEP::CDM::SpanSet::valid() const {
  if (set.empty()) return false;
  return std::all_of(set.cbegin(), set.cend(),
    [](const Span& s){return s.valid();});
};

const std::string ELEP::CDM::SpanSet::toString() const {
  if (set.empty()) return "";
  std::ostringstream ss;
  bool add_space = false;
  // ss << '{';
  for (auto it = set.cbegin(); it != set.cend(); ++it) {
    if (add_space) ss << ' ';
    ss << '{' << it->start() << ' ' << it->end() << '}';
    add_space = true;
  }
  // ss << '}';
  return ss.str();
};

void ELEP::CDM::SpanSet::update_boundaries() {
  _min = ELEP::CDM::Span::no; _max = 0;
  std::for_each(set.cbegin(), set.cend(),
               [&](const value_type& val){update_boundaries(val);});
};

void ELEP::CDM::SpanSet::update_boundaries(const value_type& val) {
  if (val.start() < _min) _min = val.start();
  if (val.end()   > _max) _max = val.end();
};

void ELEP::CDM::SpanSet::update_boundaries(const value_type&& val) {
  if (val.start() < _min) _min = val.start();
  if (val.end()   > _max) _max = val.end();
};

void ELEP::CDM::SpanSet::update_boundaries(const_iterator it) {
  update_boundaries(*it);
};

bool ELEP::CDM::SpanSet::operator< (const SpanSet& span) const {
  if (_min < span._min ||
       (_min == span._min && _max < span._max)) {
    return true;
  }
  if (_min == span._min && _max == span._max) return size() < span.size();
  return false;
};

CDM_Span CDM_CreateSpan(const CDM_Position start, const CDM_Position end) {
  return CDM_CastToOpaque(CDM_Span, new ELEP::CDM::Span(start, end));
};

CDM_Position CDM_GetStart(const CDM_Span span) {
  const ELEP::CDM::Span *p = CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (p) return p->start();
  return ELEP::CDM::Span::no;
};

CDM_Position CDM_GetEnd(const CDM_Span span) {
  const ELEP::CDM::Span *p = CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (p) return p->end();
  return ELEP::CDM::Span::no;
};

CDM_Status CDM_GetSpanOffsets(const CDM_Span span, CDM_Position *start,
                                                   CDM_Position *end) {
  const ELEP::CDM::Span *p = CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (!p) return CDM_ERROR;
  if (start) *start= p->start();
  if (end)   *end  = p->end();
  return CDM_OK;
};

CDM_SpanSet CDM_CreateSpanSet() {
  return CDM_CastToOpaque(CDM_SpanSet, new ELEP::CDM::SpanSet());
};

CDM_SpanSet CDM_CreateSpanSet(const CDM_Position start,
                              const CDM_Position end) {
  return CDM_CastToOpaque(CDM_SpanSet, new ELEP::CDM::SpanSet(start, end));
};

CDM_Status CDM_GetFirstSpanOffsets(const CDM_SpanSet spans, CDM_Position *start,
                                                            CDM_Position *end) {
  const ELEP::CDM::SpanSet *p =
        CDM_CastFromOpaque(const ELEP::CDM::SpanSet*, spans);
  if (!p || p->empty()) return CDM_ERROR;
  auto it = p->begin();
  if (start) *start= it->start();
  if (end)   *end  = it->end();
  return CDM_OK;
};

CDM_Status CDM_AddSpan(CDM_SpanSet spans, const CDM_Span span) {
  ELEP::CDM::SpanSet *p = CDM_CastFromOpaque(ELEP::CDM::SpanSet*, spans);
  if (!p) return CDM_ERROR;
  const ELEP::CDM::Span *s = CDM_CastFromOpaque(const ELEP::CDM::Span*, span);
  if (!s) return CDM_ERROR;
  p->push_back(*s);
  return CDM_OK;
};

CDM_Status CDM_AddSpan(CDM_SpanSet spans, const CDM_Position start,
                                          const CDM_Position end) {
  ELEP::CDM::SpanSet *p = CDM_CastFromOpaque(ELEP::CDM::SpanSet*, spans);
  if (!p) return CDM_ERROR;
  p->push_back(Span(start, end));
  return CDM_OK;
};

CDM_Position CDM_SpanSetOffsetMin(const CDM_SpanSet spans) {
  const ELEP::CDM::SpanSet *p = CDM_CastFromOpaque(const ELEP::CDM::SpanSet*, spans);
  if (p) return p->min();
  return ELEP::CDM::Span::no;
};

CDM_Position CDM_SpanSetOffsetMax(const CDM_SpanSet spans) {
  const ELEP::CDM::SpanSet *p = CDM_CastFromOpaque(const ELEP::CDM::SpanSet*, spans);
  if (p) return p->max();
  return 0;
};

