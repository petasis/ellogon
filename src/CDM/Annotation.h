/*****************************************************************************
 *  Annotation.h:                                                             *
 * ----------------------                                                    *
 *  This file defines the Annotation section of the                           *
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

    class Annotation {
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
        Annotation(const std::string &type, const SpanSet &spans, const AttributeSet &attributes);
        Annotation(const std::string &type, const AttributeSet &attributes);
        Annotation(const Annotation& src);
#ifndef SWIG
        Annotation(const char *type, SpanSet &&spans);
        Annotation(const char *type, SpanSet &&spans, AttributeSet &&attributes);
        Annotation(const char *type, AttributeSet &&attributes);
        Annotation(const std::string &type, SpanSet &&spans);
        Annotation(const std::string &type, SpanSet &&spans, AttributeSet &&attributes);
        Annotation(const std::string &type, AttributeSet &&attributes);
        Annotation(Annotation&& other) : Annotation() {swap::annotation(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Annotation(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        const Id               id()         const;
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
        bool                   matchesRange(const Position start, const Position end) const {return _spans.matchesRange(start, end);};
        void                   putAttribute(const Attribute& attribute) {_attributes.push_back(attribute);};
        void                   addSpan(const Span& span) {_spans.push_back(span);};

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
    }; /* class Annotation */

    class AnnotationSet {
      public:
        typedef typename std::vector<Annotation>::value_type value_type;
        typedef typename std::vector<Annotation>::size_type size_type;
        typedef typename std::vector<Annotation>::iterator iterator;
        typedef typename std::vector<Annotation>::const_iterator const_iterator;
        typedef typename std::vector<Annotation>::reverse_iterator reverse_iterator;
        typedef typename std::vector<Annotation>::const_reverse_iterator const_reverse_iterator;
#ifndef SWIG
        typedef typename std::vector<Annotation>::reference reference;
        typedef typename std::vector<Annotation>::const_reference const_reference;
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
        bool              valid() const;
        const std::string toString() const;

        /* The following methods simulate std::vector. */
        template <class InputIterator>
        void              assign (InputIterator first, InputIterator last) {set.assign(first, last);};
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

        AnnotationSet& operator=(AnnotationSet other) {swap::annotationset(*this, other); return *this;}
      private:
        std::vector<Annotation> set;
        friend void swap::annotationset(class AnnotationSet& first, class AnnotationSet& second);
    }; /* class AnnotationSet */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

int                       CDM_AnnotationContainsAttributeMatchingValue(const CDM_Annotation Ann, const char *AttributeName, const char *ValuePattern);
int                       CDM_AnnotationContainsPosition(const CDM_Annotation Ann, const CDM_Position Position);
int                       CDM_AnnotationMatchesRange(const CDM_Annotation Ann, const CDM_Position start, const CDM_Position end);
int                       CDM_CompareAnnotations(const CDM_Annotation Ann1, const CDM_Annotation Ann2);

CDM_Annotation            CDM_CreateAnnotation(const char *type, const CDM_SpanSet spans, const CDM_AttributeSet attributes);
CDM_Annotation            CDM_CreateAnnotation(const char *type, const CDM_Position start, const CDM_Position end, const CDM_AttributeSet attributes);

const CDM_Attribute       CDM_GetAttribute(const CDM_Annotation Annotation, const char *Name);
const CDM_AttributeSet    CDM_GetAttributes(const CDM_Annotation Annotation);
const CDM_SpanSet         CDM_GetSpans(const CDM_Annotation Ann);
CDM_Status                CDM_PutAttribute(CDM_Annotation Ann, const CDM_Attribute Attr);

#if 0
int                 CDM_AnnotationContainsAttributeMatchingValues(CDM_Annotation Ann, char *AttributeName, Tcl_Obj *ValuePatternsObj = NULL);
int                 CDM_AnnotationContainsPositions(const CDM_Annotation Ann, const unsigned int items, const long *Positions);
CDM_Annotation      CDM_DisplaceAnnotation(CDM_Annotation Ann, long offset, long displacement);
CDM_ByteSequenceSet CDM_GetAnnotatedTextRanges(CDM_ByteSequence Text, CDM_Annotation Annotation);
CDM_AttributeValue  CDM_GetAttributeValue(CDM_Annotation Annotation, const char *Name, CDM_AttributeValue def = NULL);
CDM_ByteSequenceSet CDM_GetFirstAnnotatedTextRange(CDM_ByteSequence Text, CDM_Annotation Annotation);
CDM_Span            CDM_GetFirstSpan(CDM_Annotation Ann);
CDM_Status          CDM_GetFirstSpanOffsets(CDM_Annotation Annotation, long *start, long *end);
const std::string  &CDM_GetType(CDM_Annotation Ann);
CDM_ByteSequence    CDM_GetTypeObj(CDM_Annotation Ann);
CDM_Id              CDM_GetId(CDM_Annotation Ann);
CDM_Annotation      CDM_RemoveAttribute(CDM_Annotation Ann, char *Name);
CDM_Annotation      CDM_ReconstructAnnotation(CDM_Annotation Annotation);
CDM_Annotation      CDM_AddAnnotationSpan(CDM_Annotation Annotation, CDM_Span span);
CDM_Annotation      CDM_RemoveSpan(CDM_Annotation Annotation, long start, long end);

int                 CDM_AnnotationSetGetElements(CDM_AnnotationSet AnnotationSet, int *objcPtr, CDM_Annotation **objvPtr);
CDM_AnnotationSet   CDM_CreateAnnotationSet(void);
CDM_AnnotationSet   CDM_AddAnnotation(CDM_AnnotationSet Set, CDM_Annotation Ann);
CDM_AnnotationSet   CDM_AnnotationsAt(CDM_AnnotationSet Set, long Position);
CDM_AnnotationSet   CDM_AnnotationsContaining(CDM_AnnotationSet Set, long Position);
CDM_AnnotationSet   CDM_AnnotationsContaining(CDM_AnnotationSet Set, long Position1, long Position2);
CDM_AnnotationSet   CDM_AnnotationsInRange(CDM_AnnotationSet Set, long Start, long End);
CDM_AnnotationSet   CDM_AnnotationsMatchingRange(CDM_AnnotationSet Set, long Start, long End);
CDM_AnnotationSet   CDM_DisplaceAnnotations(CDM_AnnotationSet Set, long offset, long displacement);
CDM_Annotation      CDM_FirstAnnotationContaining(CDM_AnnotationSet Set, const long Position);
CDM_Annotation      CDM_FirstAnnotationContaining(CDM_AnnotationSet Set, const long Position1, const long Position2);
CDM_Annotation      CDM_GetAnnotation(CDM_AnnotationSet Set, long Id);
int                 CDM_Length(CDM_AnnotationSet Set);
CDM_AnnotationSet   CDM_NextAnnotations(CDM_AnnotationSet Set, long Position);
Tcl_Obj            *CDM_Nth(Tcl_Obj *Set, long n);
CDM_AnnotationSet   CDM_MergeAnnotations(CDM_AnnotationSet Set1, CDM_AnnotationSet Set2);
CDM_AnnotationSet   CDM_RemoveAnnotation(CDM_AnnotationSet Set, long Id);
CDM_AnnotationSet   CDM_SelectAnnotations(CDM_AnnotationSet Set, char *Type);
CDM_AnnotationSet   CDM_SelectAnnotations(CDM_AnnotationSet Set, char *Type, char *Constraints);
CDM_AnnotationSet   CDM_SelectAnnotationsSorted(CDM_AnnotationSet Set, char *Type);
CDM_AnnotationSet   CDM_SelectAnnotationsSorted(CDM_AnnotationSet Set, char *Type, char *Constraints);
CDM_AnnotationSet   CDM_SortAnnotationSet(CDM_AnnotationSet Set);
#endif

#endif /* ELLOGON_CDM_Annotation */

/*
 * vim: nowrap columns=120
 */
