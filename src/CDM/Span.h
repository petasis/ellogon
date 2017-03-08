/*****************************************************************************
 *  Span.h:                                                                  *
 * ----------------------                                                    *
 *  This file defines the Span section of the                                *
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

#ifndef ELLOGON_CDM_SPAN
#define ELLOGON_CDM_SPAN

#ifndef SWIG
#include <utility>     /* For std::pair             */
#include <string>      /* For std::string           */
#include <vector>      /* For std::vector           */
#include <utility>     /* For std::swap             */
#include <climits>     /* For ULONG_MAX             */
#include <stdexcept>   /* For std::invalid_argument */
#endif /* SWIG */

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    class Span;
    class SpanSet;

#ifndef SWIG
    namespace swap {
      void span(Span& first, Span& second);
      void spanset(SpanSet& first, SpanSet& second);
    };
#endif /* SWIG */

    class Span: public serialisation::Serialisation<Span> {
      public:
        static const Position no;

        Span();
        Span(const Position start, const Position end);
        Span(const Span& src);
#ifndef SWIG
        Span(Span&& other) : Span() {swap::span(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Span(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        const Position&   start() const;
        const Position&   end()   const;
        void              start(const Position);
        void              end(const Position);
        Status            offsets(Position &start, Position &end) const;
        bool              contains(const Position p) const {return _start <= p && p <= _end;};
        bool              matchesRange(const Position start, const Position end) const;
        bool              valid() const;
        const std::string toString() const;
        bool operator< (const Span& span) const;
        Span& operator=(Span other) {swap::span(*this, other); return *this;}
      private:
#ifdef ELLOGON_CDM_SPAN_USE_PAIR
        std::pair<Position, Position> segment;
#else
        Position _start, _end;
#endif
        friend void swap::span(class Span& first, class Span& second);

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version*/ ) {
          ar( cereal::make_nvp("start", _start), cereal::make_nvp("end", _end) );
        }
        friend class serialisation::Serialisation<Span>;
        static const char* serialise_variable_name() {return "Span";};
    }; /* class Span */

    class SpanSet: public serialisation::Serialisation<SpanSet> {
      public:
        typedef typename std::vector<Span>::value_type value_type;
        typedef typename std::vector<Span>::size_type size_type;
        typedef typename std::vector<Span>::iterator iterator;
        typedef typename std::vector<Span>::const_iterator const_iterator;
        typedef typename std::vector<Span>::reverse_iterator reverse_iterator;
        typedef typename std::vector<Span>::const_reverse_iterator const_reverse_iterator;
#ifndef SWIG
        typedef typename std::vector<Span>::reference reference;
        typedef typename std::vector<Span>::const_reference const_reference;
#else  /* SWIG */
        typedef       typename Span& reference;
        typedef const typename Span& const_reference;
#endif /* SWIG */

        SpanSet();
        SpanSet(const Position start, const Position end);
        SpanSet(const SpanSet& src);
#ifndef SWIG
        SpanSet(SpanSet&& other) : SpanSet() {swap::spanset(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        SpanSet(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        void              addSpan(const Span& span) {push_back(span);};
        const Position&   firstSpanStart() const;
        const Position&   firstSpanEnd()   const;
        Status            firstSpanOffsets(Position &start, Position &end) const;
        const Position&   min()      const;
        const Position&   max()      const;
        bool              contains(const Position p) const {return _min <= p && p <= _max;};
        bool              matchesRange(const Position start, const Position end) const;
        bool              valid()    const;
        const std::string toString() const;
#ifndef SWIG
        void              update_boundaries(const value_type& val);
        void              update_boundaries(const value_type&& val);
        void              update_boundaries(const_iterator it);
        void              update_boundaries();
#endif

        /* The following methods simulate std::vector. */
        template <class InputIterator>
        void              assign (InputIterator first, InputIterator last) {set.assign(first, last); update_boundaries();};
        void              assign(size_type n, const value_type& val) {set.assign(n, val); update_boundaries(val);};
#ifndef SWIG
        // reference      at(size_type n) {return set.at(n);}; // We do not allow direct modifications
#endif /* SWIG */
        const_reference   at(size_type n) const {return set.at(n);};
        reference         back() {return set.back();};
#ifndef SWIG
        const_reference   back() const {return set.back();};
#endif /* SWIG */
        iterator          begin() noexcept {return set.begin();};
#ifndef SWIG
        const_iterator    begin() const noexcept {return set.begin();};
#endif /* SWIG */
        size_type         capacity() const noexcept {return set.capacity();};
        const_iterator    cbegin() const noexcept {return set.cbegin();};
        const_iterator    cend() const noexcept {return set.cend();};
        void              clear() noexcept {set.clear(); update_boundaries();};
        const_reverse_iterator crbegin() const noexcept {return set.crbegin();};
        const_reverse_iterator crend() const noexcept {return set.crend();};
#ifndef SWIG
        // value_type*       data() noexcept {return set.data();};
        // const value_type* data() const noexcept {return set.data();};
#endif /* SWIG */
        bool              empty() const noexcept {return set.empty();};
        iterator          end() noexcept {return set.end();};
#ifndef SWIG
        const_iterator    end() const noexcept {return set.end();};
#endif /* SWIG */
        iterator          erase(const_iterator position) {return set.erase(position); update_boundaries();};
        iterator          erase(const_iterator first, const_iterator last) {return set.erase(first, last); update_boundaries();};
        reference         front() {return set.front();};
#ifndef SWIG
        const_reference   front() const {return set.front();};
#endif /* SWIG */
        iterator          insert(const_iterator position, const value_type& val) {return set.insert(position, val); update_boundaries(val);};
        iterator          insert(const_iterator position, size_type n, const value_type& val) {return set.insert(position, n, val); update_boundaries(val);};
#ifndef SWIG
        template <class InputIterator>
        iterator          insert(const_iterator position, InputIterator first, InputIterator last) {return set.insert(position, first, last); update_boundaries();};
        iterator          insert(const_iterator position, value_type&& val) {return set.insert(position, val); update_boundaries(val);};
        iterator          insert(const_iterator position, std::initializer_list<value_type> il) {return set.insert(position, il); update_boundaries();};
#endif /* SWIG */
        size_type         max_size() const noexcept {return set.max_size();};
#ifndef SWIG
        // reference      operator[] (size_type n) {return set[n];}; // We do not allow direct modifications
#endif /* SWIG */
        const_reference   operator[] (size_type n) const {return set[n];};
        void              pop_back() {set.pop_back(); update_boundaries();};
        void              push_back(const value_type& val) {set.push_back(val); update_boundaries(val);};
#ifndef SWIG
        void              push_back(value_type&& val) {set.push_back(val); update_boundaries(val);};
#endif /* SWIG */
        reverse_iterator  rbegin() noexcept {return set.rbegin();};
#ifndef SWIG
        const_reverse_iterator rbegin() const noexcept {return set.rbegin();};
#endif /* SWIG */
        reverse_iterator  rend() noexcept {return set.rend();};
#ifndef SWIG
        const_reverse_iterator rend() const noexcept {return set.rend();};
#endif /* SWIG */
        void              reserve(size_type n) {set.reserve(n);};
        void              resize (size_type n) {set.resize(n);};
        void              resize (size_type n, const value_type& val) {set.resize(n, val);};
        void              shrink_to_fit() {set.shrink_to_fit();};
        size_type         size() const noexcept {return set.size();};

        bool operator< (const SpanSet& other) const;
        SpanSet& operator=(SpanSet other) {swap::spanset(*this, other); return *this;}
      private:
        Position _min, _max;
        std::vector<Span> set;
        friend void swap::spanset(class SpanSet& first, class SpanSet& second);

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version */ ) {
          ar( cereal::make_nvp("min", _min), cereal::make_nvp("max", _max), cereal::make_nvp("set", set) );
        };
        friend class serialisation::Serialisation<SpanSet>;
        static const char* serialise_variable_name() {return "SpanSet";};
    }; /* class SpanSet */


  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */

#ifndef SWIG
CEREAL_CLASS_VERSION(ELEP::CDM::Span,    1);
CEREAL_CLASS_VERSION(ELEP::CDM::SpanSet, 1);
#endif /* SWIG */

#endif /* __cplusplus */

CDM_Span     CDM_CreateSpan(const CDM_Position start, const CDM_Position end);
CDM_Position CDM_GetStart(const CDM_Span span);
CDM_Position CDM_GetEnd(const CDM_Span span);
CDM_Status   CDM_GetSpanOffsets(const CDM_Span span, CDM_Position *start, CDM_Position *end);

CDM_SpanSet  CDM_CreateSpanSet();
CDM_SpanSet  CDM_CreateSpanSet(const CDM_Position start, const CDM_Position end);
CDM_Status   CDM_GetFirstSpanOffsets(const CDM_SpanSet spans, CDM_Position *start, CDM_Position *end);
CDM_Status   CDM_AddSpan(CDM_SpanSet spans, const CDM_Span span);
CDM_Status   CDM_AddSpan(CDM_SpanSet spans, const CDM_Position start, const CDM_Position end);
CDM_Position CDM_SpanSetOffsetMin(const CDM_SpanSet spans);
CDM_Position CDM_SpanSetOffsetMax(const CDM_SpanSet spans);

#endif /* ELLOGON_CDM_SPAN */

/*
 * vim: nowrap columns=120
 */
