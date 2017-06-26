/*****************************************************************************
 *  Annotation.h:                                                            *
 * ----------------------                                                    *
 *  This file defines the Annotation section of the                          *
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

#ifndef ELLOGON_CDM_Annotation
#define ELLOGON_CDM_Annotation

#ifndef SWIG
#include <string>      /* For std::string         */
#include <vector>      /* For std::vector         */
#include <memory>      /* For smart pointers      */
#include <regex>       /* For regular expressions */
#include <utility>     /* For std::pair           */
#include <algorithm>   /* For std::for_each       */

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

#endif /* SWIG */

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    class Annotation;
    class AnnotationSet;

#ifndef SWIG
    namespace swap {
      void annotation(Annotation& first, Annotation& second);
      void annotationset(AnnotationSet& first, AnnotationSet& second);
    };
#endif /* SWIG */

    class Annotation: public serialisation::Serialisation<Annotation> {
      public:
        typedef typename std::vector<Attribute>::iterator attribute_iterator;
        typedef typename std::vector<Attribute>::const_iterator const_attribute_iterator;

        static const Id no;

        Annotation();
        Annotation(const char *type);
        Annotation(const char *type, const SpanSet &spans);
        Annotation(const char *type, const SpanSet &spans, const AttributeSet &attributes);
        Annotation(const char *type, const AttributeSet &attributes);
        Annotation(const std::string &type);
        Annotation(const std::string &type, const SpanSet &spans);
        Annotation(const Id id, const std::string &type, const SpanSet &spans, const AttributeSet &attributes);
        Annotation(const std::string &type, const SpanSet &spans, const AttributeSet &attributes);
        Annotation(const std::string &type, const AttributeSet &attributes);
        Annotation(const Annotation& src);
#ifndef SWIG
        Annotation(const char *type, SpanSet &&spans);
        Annotation(const char *type, SpanSet &&spans, AttributeSet &&attributes);
        Annotation(const char *type, AttributeSet &&attributes);
        Annotation(const std::string &type, SpanSet &&spans);
        Annotation(const std::string &type, SpanSet &&spans, AttributeSet &&attributes);
        Annotation(const Id id, const std::string &type, SpanSet &&spans, AttributeSet &&attributes);
        Annotation(const std::string &type, AttributeSet &&attributes);
        Annotation(Annotation&& other) : Annotation() {swap::annotation(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Annotation(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        const Id               id()         const;
        void                   clearId()          {_id = no;};
        bool                   hasId()      const {return _id != no;};
        bool                   emptyId()    const {return _id == no;};
        const std::string&     type()       const;
        void                   type(const char* type);
#ifndef SWIG
        void                   type(const std::string& type);
#endif /* SWIG */
        const SpanSet&         spans()      const;
        void                   spans(const SpanSet& spans);
#ifndef SWIG
        void                   spans(SpanSet&& spans);
#endif /* SWIG */
        const AttributeSet&    attributes() const;
        void                   attributes(const AttributeSet& attributes);
#ifndef SWIG
        void                   attributes(AttributeSet&& attributes);
#endif /* SWIG */

        bool                   exists(const char *name) const {return _attributes.exists(name);};
        bool                   exists(const std::string &name) const {return _attributes.exists(name);};
#ifndef SWIG
        attribute_iterator       find(const char *name) {return _attributes.find(name);};
        attribute_iterator       find(const std::string &name) {return _attributes.find(name);};
#endif
        const_attribute_iterator find(const char *name) const {return _attributes.find(name);};
        const_attribute_iterator find(const std::string &name) const {return _attributes.find(name);};

        bool                   containsAttributeMatchingValue(const char *name, const char *pattern) const;
        bool                   containsAttributeMatchingValue(const std::string& name, const std::string& pattern) const;
        bool                   containsAttributeMatchingValue(const char *name, const std::regex& pattern) const;
        bool                   containsAttributeMatchingValue(const std::string& name, const std::regex& pattern) const;
        bool                   contains(const Position p) const {return _spans.contains(p);};
        bool                   contains(const std::vector<Position>& p) const {return _spans.contains(p);};
        bool                   contains(const size_t items, const Position *positions) const {return _spans.contains(items, positions);};
        bool                   displace(const Int displacement) {return _spans.displace(displacement);};
        bool                   displace(const Position offset, const Int displacement) {return _spans.displace(offset, displacement);};
        bool                   matchesRange(const Position start, const Position end) const {return _spans.matchesRange(start, end);};
        void                   putAttribute(const Attribute& attribute) {_attributes.putAttribute(attribute);};
        void                   removeAttribute(const char *name) {_attributes.removeAttribute(name);};
        void                   removeSpan(const Span& span) {_spans.removeSpan(span);};
        void                   removeSpan(const Position start, const Position end) {_spans.removeSpan(start, end);};
        void                   addSpan(const Span& span) {_spans.addSpan(span);};
        void                   addSpan(const Position start, const Position end) {_spans.addSpan(start, end);};
        std::string            textRange(const std::string& text) const {return _spans.textRange(text);};
        std::vector<std::string>
                               textRanges(const std::string& text) const {return _spans.textRanges(text);};

        bool                   valid()      const;
        const std::string      toString()   const;
        bool operator< (const Annotation& Annotation) const;
        Annotation& operator=(Annotation other) {swap::annotation(*this, other); return *this;}
      private:
        Id                           _id;
        std::shared_ptr<std::string> _type;
        SpanSet                      _spans;
        AttributeSet                 _attributes;
        friend void swap::annotation(class Annotation& first, class Annotation& second);
        friend class AnnotationSet;

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version*/ ) {
          ar( cereal::make_nvp("id", _id), cereal::make_nvp("type", _type),
              cereal::make_nvp("spans", _spans), cereal::make_nvp("attributes", _attributes) );
        }
        friend class serialisation::Serialisation<Annotation>;
        static const char* serialise_variable_name() {return "Annotation";};
    }; /* class Annotation */

    namespace internal {
      namespace tags {
        struct id_asc  {};
        struct ann_asc {};
        struct hash    {};
        struct type    {};
      };

      typedef boost::multi_index::multi_index_container<
        Annotation,                                                                  // the stored object
        boost::multi_index::indexed_by<
          // sort by less<Id> on Annotation::id()
          boost::multi_index::ordered_unique<                                        // first view
            boost::multi_index::tag<tags::id_asc>,                                   // tag used to access the view
            boost::multi_index::const_mem_fun<Annotation, const Id, &Annotation::id>,// ordered on id()...
            std::less<const Id>                                                      // ... by ascending order
          >,
          // sort by Annotation::operator<
          boost::multi_index::ordered_non_unique<                                    // second view
            boost::multi_index::tag<tags::ann_asc>,                                  // tag used to access the view
            boost::multi_index::identity<Annotation>                                 // ordered on Annotation...
          >,
          // Quick lookup through a hash table on Annotation::id()
          boost::multi_index::hashed_unique<                                         // third view: unordered (hashed) on id
            boost::multi_index::tag<tags::hash>,                                     // tag used to access the view
            boost::multi_index::const_mem_fun<Annotation, const Id, &Annotation::id> // we hash on id()...
            // std::hash<const Id>
          >,
          // Quick lookup through a hash table on Annotation::type()
          boost::multi_index::hashed_non_unique<                                     // forth view: unordered (hashed) on type
            boost::multi_index::tag<tags::type>,                                     // tag used to access the view
            boost::multi_index::const_mem_fun<Annotation, const std::string&, &Annotation::type> // we hash on type()...
            // std::hash<const Id>
          >
        >
      > DocumentAnnotationSet;
    };
#ifndef SWIG
#endif /* SWIG */

    class AnnotationSet: public serialisation::Serialisation<AnnotationSet> {
      public:
        typedef typename internal::DocumentAnnotationSet::key_type key_type;
        typedef typename internal::DocumentAnnotationSet::value_type value_type;
        typedef typename internal::DocumentAnnotationSet::size_type size_type;
        typedef typename internal::DocumentAnnotationSet::iterator iterator;
        typedef typename internal::DocumentAnnotationSet::const_iterator const_iterator;
        typedef typename internal::DocumentAnnotationSet::reverse_iterator reverse_iterator;
        typedef typename internal::DocumentAnnotationSet::const_reverse_iterator const_reverse_iterator;
#ifndef SWIG
        typedef typename internal::DocumentAnnotationSet::reference reference;
        typedef typename internal::DocumentAnnotationSet::const_reference const_reference;
#else  /* SWIG */
        typedef       typename Annotation& reference;
        typedef const typename Annotation& const_reference;
#endif /* SWIG */

        AnnotationSet();
        AnnotationSet(const Annotation &annotation);
        AnnotationSet(const char *type);
        AnnotationSet(const char *type, const SpanSet &spans);
        AnnotationSet(const char *type, const SpanSet &spans, const AttributeSet &attributes);
        AnnotationSet(const char *type, const AttributeSet &attributes);
        AnnotationSet(const std::string &type);
        AnnotationSet(const std::string &type, const SpanSet &spans);
        AnnotationSet(const std::string &type, const SpanSet &spans, const AttributeSet &attributes);
        AnnotationSet(const std::string &type, const AttributeSet &attributes);
        AnnotationSet(const AnnotationSet& src);
#ifndef SWIG
        AnnotationSet(Annotation &&annotation);
        AnnotationSet(const char *type, SpanSet &&spans);
        AnnotationSet(const char *type, SpanSet &&spans, AttributeSet &&attributes);
        AnnotationSet(const char *type, AttributeSet &&attributes);
        AnnotationSet(const std::string &type, SpanSet &&spans);
        AnnotationSet(const std::string &type, SpanSet &&spans, AttributeSet &&attributes);
        AnnotationSet(const std::string &type, AttributeSet &&attributes);
        AnnotationSet(AnnotationSet&& other) : AnnotationSet() {swap::annotationset(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        AnnotationSet(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        Id                addAnnotation(const Annotation& ann)  {return insert(ann).first->id();};
#ifndef SWIG
        Id                addAnnotation(const Annotation&& ann) {return insert(ann).first->id();};
#endif /* SWIG */
        void              mergeAnnotations(const AnnotationSet& other);
        void              concatAnnotations(const AnnotationSet& other);
        bool              valid() const;
        const std::string toString() const;

        /* DocumentAnnotationSet, view tags::id_asc */
#ifndef SWIG
        //reference              at(size_type n) {auto&& view = set.get<internal::tags::id_asc>(); auto it = std::next(view.begin(), n); if (it == view.end()) throw std::out_of_range("index out of range"); return *it;};
#endif /* SWIG */
        const_reference        at(size_type n) const {auto&& view = set.get<internal::tags::id_asc>(); auto it = std::next(view.cbegin(), n); if (it == view.end()) throw std::out_of_range("index out of range"); return *it;};
#ifndef SWIG
        iterator               begin()   noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.begin();};
#endif /* SWIG */
        const_iterator         begin()   const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.begin();};
#ifndef SWIG
        iterator               end()     noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.end();};
#endif /* SWIG */
        const_iterator         end()     const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.end();};
#ifndef SWIG
        reverse_iterator       rbegin()  noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.rbegin();};
#endif /* SWIG */
        const_reverse_iterator rbegin()  const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.rbegin();};
#ifndef SWIG
        reverse_iterator       rend()    noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.rend();};
#endif /* SWIG */
        const_reverse_iterator rend()    const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.rend();};
        const_iterator         cbegin()  const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.cbegin();};
        const_iterator         cend()    const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.cend();};
        const_reverse_iterator crbegin() const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.crbegin();};
        const_reverse_iterator crend()   const noexcept {auto&& view = set.get<internal::tags::id_asc>(); return view.crend();};

        const_reference        get(const Id id) const {auto&& view = set.get<internal::tags::hash>(); auto it = view.find(id); if (it == view.end()) throw std::out_of_range("annotation with specified id not found"); return *it;};

#ifndef SWIG
        iterator               iterator_to(const value_type& x) {auto&& view = set.get<internal::tags::id_asc>(); return view.iterator_to(x);};
#endif /* SWIG */
        const_iterator         iterator_to(const value_type& x) const {auto&& view = set.get<internal::tags::id_asc>(); return view.iterator_to(x);};

        // capacity:
        bool                   empty()    const noexcept {return set.empty();};
        size_type              size()     const noexcept {return set.size();};
        size_type              max_size() const noexcept {return set.max_size();};

        // modifiers:
#ifndef SWIG
        std::pair<iterator,bool> insert(const value_type& x);
        std::pair<iterator,bool> insert(value_type&& x);
#endif /* SWIG */
#ifndef SWIG
        template<typename InputIterator>
        void insert(InputIterator first,InputIterator last) {std::for_each(first, last, [&](const value_type& x) {insert(x);});} ;
        void insert(std::initializer_list<value_type> list) {insert(list.begin(), list.end());};
#endif /* SWIG */

        iterator  erase(iterator position)                     {return set.erase(position);};
        size_type erase(const key_type& x)                     {return set.erase(x);};
        iterator  erase(iterator first,iterator last)          {return set.erase(first, last);};

        bool replace(iterator position,const value_type& x)    {return set.replace(position, x);};
#ifndef SWIG
        bool replace(iterator position,value_type&& x)         {return set.replace(position, x);};
#endif /* SWIG */

        void clear() noexcept {_next_ann_id = 0; set.clear();};

        // set operations:
        template<typename CompatibleKey>
        iterator find(const CompatibleKey& x)const;
        template<typename CompatibleKey,typename CompatibleCompare>
        iterator find(
          const CompatibleKey& x,const CompatibleCompare& comp)const;

        template<typename CompatibleKey>
        size_type count(const CompatibleKey& x)const;
        template<typename CompatibleKey,typename CompatibleCompare>
        size_type count(const CompatibleKey& x,const CompatibleCompare& comp)const;

        template<typename CompatibleKey>
        iterator lower_bound(const CompatibleKey& x)const;
        template<typename CompatibleKey,typename CompatibleCompare>
        iterator lower_bound(
          const CompatibleKey& x,const CompatibleCompare& comp)const;

        template<typename CompatibleKey>
        iterator upper_bound(const CompatibleKey& x)const;
        template<typename CompatibleKey,typename CompatibleCompare>
        iterator upper_bound(
          const CompatibleKey& x,const CompatibleCompare& comp)const;

        template<typename CompatibleKey>
        std::pair<iterator,iterator> equal_range(
          const CompatibleKey& x)const;
        template<typename CompatibleKey,typename CompatibleCompare>
        std::pair<iterator,iterator> equal_range(
          const CompatibleKey& x,const CompatibleCompare& comp)const;

        // Selectors...
        AnnotationSet select(const char* type)                 const {AnnotationSet s; select(s, type); return s;};
        AnnotationSet select(const std::string& type)          const {AnnotationSet s; select(s, type); return s;};

        bool displace(const Int displacement);
        bool displace(const Position offset, const Int displacement);

        void select(AnnotationSet& s, const char* type)        const {select(s, std::string(type));};
        void select(AnnotationSet& s, const std::string& type) const;
        void select(AnnotationSet& s, const char* type, const Functor::AnnotationUnaryPredicate &pred) const {select(s, std::string(type), pred);};
        void select(AnnotationSet& s, const std::string& type, const Functor::AnnotationUnaryPredicate &pred) const;
        void select(AnnotationSet& s, const char* type, const char* pred) const {select(s, std::string(type), pred);};
        void select(AnnotationSet& s, const std::string& type, const std::string& pred) const;


#if 0
        /* The following methods simulate std::vector. */
        template <class InputIterator>
        void              assign (InputIterator first, InputIterator last) {auto&& view = set.get<internal::tags::id_asc>(); view.assign(first, last);};
        void              assign(size_type n, const value_type& val) {set.assign(n, val);};
#ifndef SWIG
        reference         at(size_type n) {return set.at(n);};
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
        void              clear() noexcept {set.clear();};
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
        iterator          erase(const_iterator position) {return set.erase(position);};
        iterator          erase(const_iterator first, const_iterator last) {return set.erase(first, last);};
        reference         front() {return set.front();};
#ifndef SWIG
        const_reference   front() const {return set.front();};
#endif /* SWIG */
        iterator          insert(const_iterator position, const value_type& val) {return set.insert(position, val);};
#ifndef SWIG
        template <class InputIterator>
        iterator          insert(const_iterator position, InputIterator first, InputIterator last) {return set.insert(position, first, last);};
        iterator          insert(const_iterator position, value_type&& val) {return set.insert(position, val);};
        iterator          insert(const_iterator position, std::initializer_list<value_type> il) {return set.insert(position, il);};
#endif /* SWIG */
        size_type         max_size() const noexcept {return set.max_size();};
#ifndef SWIG
        reference         operator[] (size_type n) {return set[n];};
#endif /* SWIG */
        const_reference   operator[] (size_type n) const {return set[n];};
        void              pop_back() {set.pop_back();};
        void              push_back(const value_type& val) {set.push_back(val);};
#ifndef SWIG
        void              push_back(value_type&& val) {set.push_back(val);};
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
#endif

        AnnotationSet& operator=(AnnotationSet other) {swap::annotationset(*this, other); return *this;}
      private:
        internal::DocumentAnnotationSet  set;
        Id                              _next_ann_id;
        bool                            _modified;
        friend void swap::annotationset(class AnnotationSet& first, class AnnotationSet& second);

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version*/ ) {
          ar( cereal::make_nvp("nextId", _next_ann_id), cereal::make_nvp("set", set) );
        }
        friend class serialisation::Serialisation<AnnotationSet>;
        static const char* serialise_variable_name() {return "AnnotationSet";};
    }; /* class AnnotationSet */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

CDM_Status                CDM_AddAnnotationSpan(CDM_Annotation Ann, const CDM_Span span);
CDM_Status                CDM_AddAnnotationSpan(CDM_Annotation Ann, const CDM_Position start, const CDM_Position end);
CDM_Status                CDM_AddSpan(CDM_Annotation Ann, const CDM_Span span);
CDM_Status                CDM_AddSpan(CDM_Annotation Ann, const CDM_Position start, const CDM_Position end);
int                       CDM_AnnotationContainsAttributeMatchingValue(const CDM_Annotation Ann, const char *AttributeName, const char *ValuePattern);
int                       CDM_AnnotationContainsPosition(const CDM_Annotation Ann, const CDM_Position Position);
int                       CDM_AnnotationContainsPositions(const CDM_Annotation Ann, const size_t items, const CDM_Position *Positions);
int                       CDM_AnnotationMatchesRange(const CDM_Annotation Ann, const CDM_Position start, const CDM_Position end);
int                       CDM_CompareAnnotations(const CDM_Annotation Ann1, const CDM_Annotation Ann2);

CDM_Annotation            CDM_CreateAnnotation(const char *type, const CDM_SpanSet spans, const CDM_AttributeSet attributes);
CDM_Annotation            CDM_CreateAnnotation(const char *type, const CDM_Position start, const CDM_Position end, const CDM_AttributeSet attributes);

CDM_Status                CDM_DisplaceAnnotation(CDM_Annotation Ann, long displacement);
CDM_Status                CDM_DisplaceAnnotation(CDM_Annotation Ann, long offset, long displacement);

const CDM_Attribute       CDM_GetAttribute(const CDM_Annotation Ann, const char *Name);
const CDM_AttributeSet    CDM_GetAttributes(const CDM_Annotation Ann);
CDM_AttributeValue        CDM_GetAttributeValue(const CDM_Annotation Ann, const char *Name, const CDM_AttributeValue def = nullptr);
const CDM_Span            CDM_GetAnnotationFirstSpan(const CDM_Annotation Ann);
CDM_Status                CDM_GetAnnotationFirstSpanOffsets(const CDM_Annotation Ann, CDM_Position *start, CDM_Position *end);
const CDM_SpanSet         CDM_GetSpans(const CDM_Annotation Ann);
CDM_Status                CDM_PutAttribute(CDM_Annotation Ann, const CDM_Attribute Attr);
CDM_Status                CDM_RemoveAttribute(CDM_Annotation Ann, const char *Name);
CDM_Status                CDM_RemoveSpan(CDM_Annotation Ann, const CDM_Span span);
CDM_Status                CDM_RemoveSpan(CDM_Annotation Ann, const CDM_Position start, const CDM_Position end);
const CDM_ByteSequence    CDM_GetType(const CDM_Annotation Ann);
CDM_Id                    CDM_GetId(const CDM_Annotation Ann);
CDM_ByteSequence          CDM_GetFirstAnnotatedTextRange(const CDM_ByteSequence Text, const CDM_Annotation Ann);

#if 0
int                 CDM_AnnotationContainsAttributeMatchingValues(CDM_Annotation Ann, char *AttributeName, Tcl_Obj *ValuePatternsObj = NULL);
CDM_ByteSequenceSet CDM_GetAnnotatedTextRanges(CDM_ByteSequence Text, CDM_Annotation Ann);
CDM_ByteSequence    CDM_GetTypeObj(CDM_Annotation Ann);
CDM_Annotation      CDM_ReconstructAnnotation(CDM_Annotation Ann);
#endif

CDM_AnnotationSet         CDM_CreateAnnotationSet();
CDM_Id                    CDM_AddAnnotationToSet(CDM_AnnotationSet Set, const CDM_Annotation Ann);
CDM_Size                  CDM_Length(const CDM_AnnotationSet Set);
const CDM_Annotation      CDM_Nth(const CDM_AnnotationSet Set, CDM_Size n);
CDM_Status                CDM_MergeAnnotations(CDM_AnnotationSet Set1, const CDM_AnnotationSet Set2);
CDM_Status                CDM_ConcatAnnotations(CDM_AnnotationSet Set1, const CDM_AnnotationSet Set2);

#if 0
int                 CDM_AnnotationSetGetElements(CDM_AnnotationSet AnnotationSet, int *objcPtr, CDM_Annotation **objvPtr);
CDM_AnnotationSet   CDM_AnnotationsAt(CDM_AnnotationSet Set, long Position);
CDM_AnnotationSet   CDM_AnnotationsContaining(CDM_AnnotationSet Set, long Position);
CDM_AnnotationSet   CDM_AnnotationsContaining(CDM_AnnotationSet Set, long Position1, long Position2);
CDM_AnnotationSet   CDM_AnnotationsInRange(CDM_AnnotationSet Set, long Start, long End);
CDM_AnnotationSet   CDM_AnnotationsMatchingRange(CDM_AnnotationSet Set, long Start, long End);
CDM_AnnotationSet   CDM_DisplaceAnnotations(CDM_AnnotationSet Set, long offset, long displacement);
CDM_Annotation      CDM_FirstAnnotationContaining(CDM_AnnotationSet Set, const long Position);
CDM_Annotation      CDM_FirstAnnotationContaining(CDM_AnnotationSet Set, const long Position1, const long Position2);
CDM_AnnotationSet   CDM_NextAnnotations(CDM_AnnotationSet Set, long Position);
CDM_AnnotationSet   CDM_RemoveAnnotation(CDM_AnnotationSet Set, long Id);
CDM_AnnotationSet   CDM_SelectAnnotationsSorted(CDM_AnnotationSet Set, char *Type);
CDM_AnnotationSet   CDM_SelectAnnotationsSorted(CDM_AnnotationSet Set, char *Type, char *Constraints);
CDM_AnnotationSet   CDM_SortAnnotationSet(CDM_AnnotationSet Set);
#endif
const CDM_Annotation      CDM_GetAnnotation(const CDM_AnnotationSet Set, const CDM_Id Id);
CDM_AnnotationSet         CDM_SelectAnnotations(const CDM_AnnotationSet Set, const char *Type);
CDM_AnnotationSet         CDM_SelectAnnotations(const CDM_AnnotationSet Set, const char *Type, const char *Constraints);

#endif /* ELLOGON_CDM_Annotation */

/*
 * vim: nowrap columns=120
 */
