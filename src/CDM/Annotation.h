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
#include <string>      /* For std::string    */
#include <vector>      /* For std::vector    */
#include <memory>      /* For smart pointers */
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
        const SpanSet&         spans()      const;
        const AttributeSet&    attributes() const;
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
        typedef typename std::vector<Annotation>::size_type size_type;
        typedef typename std::vector<Annotation>::const_iterator const_iterator;

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

        void           push_back(const Annotation& val);
#ifndef SWIG
        void           push_back(Annotation&& val);
#endif /* SWIG */
        size_type      size()  const;
        bool           empty() const;
        const_iterator begin() const;
        const_iterator end()   const;
        void           clear();
        AnnotationSet& operator=(AnnotationSet other) {swap::annotationset(*this, other); return *this;}
      private:
        std::vector<Annotation> set;
        friend void swap::annotationset(class AnnotationSet& first, class AnnotationSet& second);
    }; /* class AnnotationSet */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

CDM_Annotation      CDM_CreateAnnotation(const char *type, const CDM_SpanSet spans, const CDM_AttributeSet attributes);
CDM_Annotation      CDM_CreateAnnotation(const char *type, const CDM_Position start, const CDM_Position end, const CDM_AttributeSet attributes);

#if 0
int                 CDM_AnnotationContainsAttributeMatchingValue(CDM_Annotation Ann, char *AttributeName, char *ValuePattern);
int                 CDM_AnnotationContainsAttributeMatchingValues(CDM_Annotation Ann, char *AttributeName, Tcl_Obj *ValuePatternsObj = NULL);
int                 CDM_AnnotationContainsPosition(const CDM_Annotation Ann, const long Position);
int                 CDM_AnnotationContainsPositions(const CDM_Annotation Ann, const unsigned int items, const long *Positions);
int                 CDM_AnnotationMatchesRange(const CDM_Annotation Ann, const long start, const long end);
int                 CDM_CompareAnnotations(CDM_Annotation Ann1, CDM_Annotation Ann2);
CDM_Annotation      CDM_DisplaceAnnotation(CDM_Annotation Ann, long offset, long displacement);
CDM_ByteSequenceSet CDM_GetAnnotatedTextRanges(CDM_ByteSequence Text, CDM_Annotation Annotation);
CDM_Attribute       CDM_GetAttribute(CDM_Annotation Annotation, const char *Name);
CDM_AttributeSet    CDM_GetAttributes(CDM_Annotation Annotation);
CDM_AttributeValue  CDM_GetAttributeValue(CDM_Annotation Annotation, const char *Name, CDM_AttributeValue def = NULL);
CDM_ByteSequenceSet CDM_GetFirstAnnotatedTextRange(CDM_ByteSequence Text, CDM_Annotation Annotation);
CDM_Span            CDM_GetFirstSpan(CDM_Annotation Ann);
CDM_Status          CDM_GetFirstSpanOffsets(CDM_Annotation Annotation, long *start, long *end);
CDM_SpanSet         CDM_GetSpans(CDM_Annotation Ann);
const std::string  &CDM_GetType(CDM_Annotation Ann);
CDM_ByteSequence    CDM_GetTypeObj(CDM_Annotation Ann);
CDM_Id              CDM_GetId(CDM_Annotation Ann);
CDM_Annotation      CDM_PutAttribute(CDM_Annotation Ann, CDM_Attribute Attr);
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
