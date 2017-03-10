/*****************************************************************************
 *  Document.h:                                                              *
 * ----------------------                                                    *
 *  This file defines the Document section of the                            *
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

#ifndef ELLOGON_CDM_Document
#define ELLOGON_CDM_Document

#ifndef SWIG
#include <string>      /* For std::string           */
#include <vector>      /* For std::vector           */
#include <memory>      /* For smart pointers        */
#include <utility>     /* For std::swap             */
#include <stdexcept>   /* For std::invalid_argument */
#endif /* SWIG */

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    class Document;
    class Collection;

#ifndef SWIG
    namespace swap {
      void document(Document& first, Document& second);
      void collection(Collection& first, Collection& second);
    };
#endif /* SWIG */

    class Document: public serialisation::Serialisation<Document> {
      public:
        Document();
        Document(const Document& src);
        Document(const char* name);
        Document(const char* name, const char* data);
        Document(const char* name, const char* data, const AttributeSet& attributes, const AnnotationSet& annotations);
#ifndef SWIG
        Document(const std::string& name);
        Document(const std::string& name, const std::string& data);
        Document(const std::string& name, const std::string& data, const AttributeSet& attributes, const AnnotationSet& annotations);
        Document(Document&& other) : Document() {swap::document(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Document(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */

        const std::string&           id()          const;
        const std::string&           externalId()  const;
        const std::string&           data()        const;
        const AttributeSet&          attributes()  const;
        const AnnotationSet&         annotations() const;

        Id                           addAnnotation(const Annotation& ann);
#ifndef SWIG
        Id                           addAnnotation(const Annotation&& ann);
#endif /* SWIG */

        bool                         valid()       const;
        const std::string            toString()    const;

        bool operator< (const Document& Document)  const;
        Document& operator=(Document other) {swap::document(*this, other); return *this;}
      private:
        std::string                  _id;
        std::string                  _external_id;
        std::string                  _data;
        AttributeSet                 _attributes;
        AnnotationSet                _annotations;
        std::shared_ptr<Collection>  _collection;
        friend void swap::document(class Document& first, class Document& second);

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version*/ ) {
          ar( cereal::make_nvp("id", _id), cereal::make_nvp("externalId", _external_id),
              cereal::make_nvp("collection", _collection), cereal::make_nvp("data", _data),
              cereal::make_nvp("attributes", _attributes), cereal::make_nvp("annotations", _annotations) );
        }
        friend class serialisation::Serialisation<Document>;
        static const char* serialise_variable_name() {return "Document";};
        void normalise_data();
    }; /* class Document */

    class Collection: public serialisation::Serialisation<Collection> {
      public:
        typedef typename std::vector<Document>::value_type value_type;
        typedef typename std::vector<Document>::size_type size_type;
        typedef typename std::vector<Document>::iterator iterator;
        typedef typename std::vector<Document>::const_iterator const_iterator;
        typedef typename std::vector<Document>::reverse_iterator reverse_iterator;
        typedef typename std::vector<Document>::const_reverse_iterator const_reverse_iterator;
#ifndef SWIG
        typedef typename std::vector<Document>::reference reference;
        typedef typename std::vector<Document>::const_reference const_reference;
#else  /* SWIG */
        typedef       typename Document& reference;
        typedef const typename Document& const_reference;
#endif /* SWIG */

        Collection();
        Collection(const Collection& src);
#ifndef SWIG
        Collection(Collection&& other) : Collection() {swap::collection(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Collection(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */

        bool              valid()    const;
        const std::string toString() const;
#if 0
        void              addDocument(const Document& Document) {push_back(Document);};
        const Position&   firstDocumentStart() const;
        const Position&   firstDocumentEnd()   const;
        Status            firstDocumentOffsets(Position &start, Position &end) const;
        const Position&   min()      const;
        const Position&   max()      const;
        bool              contains(const Position p) const {return _min <= p && p <= _max;};
        bool              matchesRange(const Position start, const Position end) const;
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

#endif
        bool operator< (const Collection& other) const;
        Collection& operator=(Collection other) {swap::collection(*this, other); return *this;}
      private:
        std::string                  _id;
        AttributeSet                 _attributes;
        std::vector<Document>        set;
        friend void swap::collection(class Collection& first, class Collection& second);

        friend class cereal::access;
        template <class Archive>
        void serialize( Archive & ar /*, const std::uint32_t version*/ ) {
          ar( cereal::make_nvp("id", _id), cereal::make_nvp("attributes", _attributes),
              cereal::make_nvp("documents", set) );
        }
        friend class serialisation::Serialisation<Collection>;
        static const char* serialise_variable_name() {return "Collection";};
    }; /* class Collection */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

CDM_Id                CDM_AddAnnotation(CDM_Document Document, const CDM_Annotation Ann);
CDM_Document          CDM_CreateDocument(const char *name, const CDM_ByteSequence data, const CDM_AttributeSet attributes, const CDM_AnnotationSet annotations);
#if 0
CDM_AnnotationSet     CDM_AnnotationsAt(const CDM_Document Document, const CDM_Position Position);
CDM_AnnotationSet     CDM_AnnotationsContaining(const CDM_Document Document, const CDM_Position Position);
CDM_AnnotationSet     CDM_AnnotationsContaining(const CDM_Document Document, const CDM_Position Position1, const CDM_Position Position2);
CDM_AnnotationSet     CDM_AnnotationsInRange(const CDM_Document Document, const CDM_Position Start, const CDM_Position End);
CDM_AnnotationSet     CDM_AnnotationsMatchingRange(const CDM_Document Document, const CDM_Position Start, const CDM_Position End);
CDM_ByteSequence      CDM_ByteSequenceInsertString(CDM_Document Document, const CDM_Position pos, const char *string);
CDM_ByteSequence      CDM_ByteSequenceReplaceCharacters(CDM_Document Document, const CDM_Position pos, int chars, const char *string);
CDM_ByteSequence      CDM_ByteSequenceReplace(CDM_Document Document, const CDM_Position first, const CDM_Position last, const char *newstring);
CDM_Status            CDM_Close(CDM_Document Document);
CDM_Status            CDM_DeleteAnnotations(CDM_Document Document, const char *Type);
CDM_Status            CDM_DeleteAnnotations(CDM_Document Document, const char *Type, const char *Constraints);
CDM_Status            CDM_DeleteAttributes(CDM_Document Document);
CDM_Status            CDM_DisplaceAnnotations(CDM_Document Document, const CDM_Position offset, const CDM_Position displacement);
CDM_Id                CDM_FindMaxUsedAnnotationId(const CDM_Document Document);
CDM_Annotation        CDM_FirstAnnotationContaining(const CDM_Document Document, const CDM_Position Position);
CDM_Annotation        CDM_FirstAnnotationContaining(const CDM_Document Document, const CDM_Position Position1, const CDM_Position Position2);
CDM_ByteSequenceSet   CDM_GetAnnotatedTextRanges(const CDM_Document Document, const CDM_Annotation Annotation);
CDM_Annotation        CDM_GetAnnotation(const CDM_Document Document, const CDM_Id Id);
CDM_AnnotationSet     CDM_GetAnnotations(const CDM_Document Document);
CDM_Attribute         CDM_GetAttribute(const CDM_Document Document, const char *Name);
CDM_AttributeSet      CDM_GetAttributes(const CDM_Document Document);
CDM_AttributeValue    CDM_GetAttributeValue(const CDM_Document Document, const char *Name, const CDM_AttributeValue def = NULL);
CDM_ByteSequence      CDM_GetByteSequence(const CDM_Document Document);
const char*           CDM_GetExternalId(const CDM_Document Document);
CDM_ByteSequenceSet   CDM_GetFirstAnnotatedTextRange(const CDM_Document Document, const CDM_Annotation Annotation);
CDM_Annotation        CDM_GetFirstAnnotation(const CDM_Document Document, const char *Type);
const char*           CDM_GetId(const CDM_Collection Collection, const CDM_Document Document);
const char*           CDM_GetId(const CDM_Document Document);
CDM_Annotation        CDM_GetNextAnnotation(const CDM_Document Document);
CDM_Collection        CDM_GetParent(const CDM_Document Document);
int                   CDM_Modified(const CDM_Document Document);
int                   CDM_Modified(const CDM_Document Document, const int value);
CDM_AnnotationSet     CDM_NextAnnotations(const CDM_Document Document, const CDM_Position Position);
CDM_Status            CDM_PutAttribute(CDM_Document Document, const CDM_Attribute Attr);
int                   CDM_ReadOnly(const CDM_Document Document);
int                   CDM_ReadOnly(CDM_Document Document, const int value);
CDM_Status            CDM_RemoveAnnotation(CDM_Document Document, const CDM_Id Id);
CDM_Status            CDM_RemoveAttribute(CDM_Document Document, const char *Name);
CDM_AnnotationSet     CDM_SelectAnnotations(const CDM_Document Document, const char *Type);
CDM_AnnotationSet     CDM_SelectAnnotations(const CDM_Document Document, const char *Type, const char *Constraints);
CDM_AnnotationSet     CDM_SelectAnnotationsSorted(const CDM_Document Document, const char *Type);
CDM_AnnotationSet     CDM_SelectAnnotationsSorted(const CDM_Document Document, const char *Type, const char *Constraints);
CDM_Status            CDM_SetByteSequence(CDM_Document Document, const CDM_ByteSequence Text);
CDM_Status            CDM_SetExternalId(CDM_Document Document, const char *ExternalId);
CDM_Status            CDM_Sync(const CDM_Document Document);
CDM_Status            CDM_Free(CDM_Document Document);
CDM_Status            CDM_Reset(CDM_Document Document);

int                   CDM_Close(CDM_Collection Collection);
CDM_Collection        CDM_CreateCollection(
                          char *Name, CDM_AttributeSet Attributes, char *Encoding);
CDM_Collection        CDM_CreateCollection(char *Name,CDM_AttributeSet Attributes);
CDM_Document          CDM_CreateDocument(
                          CDM_Collection Collection, char *ExternalId,
                          CDM_ByteSequence RawData, CDM_AnnotationSet Annotations,
                          CDM_AttributeSet Attributes, char *Encoding);
CDM_Document          CDM_CreateDocument(
                          CDM_Collection Collection, char *ExternalId,
                          CDM_ByteSequence RawData, CDM_AnnotationSet Annotations,
                          CDM_AttributeSet Attributes);
int                   CDM_DeleteAttributes(CDM_Collection Collection);
int                   CDM_Destroy(char *Name);
CDM_Document          CDM_FirstDocument(CDM_Collection Collection);
Tcl_Obj           *   CDM_GetAssociatedInfo(CDM_Collection Collection);
CDM_Attribute         CDM_GetAttribute(CDM_Collection Collection,
                                       const char *Name);
CDM_AttributeSet      CDM_GetAttributes(CDM_Collection Collection);
CDM_AttributeValue    CDM_GetAttributeValue(CDM_Collection Collection,
                      const char *Name, CDM_AttributeValue def = NULL);
CDM_Document          CDM_GetByExternalId(CDM_Collection Collection, char *XID);
void              *   CDM_GetClientData(CDM_Collection Collection);
Tcl_Obj           *   CDM_GetComment(CDM_Collection Collection);
CDM_Document          CDM_GetDocument(CDM_Collection Collection, char *Id);
char              *   CDM_GetEncoding(CDM_Collection Collection);
char              *   CDM_GetName(CDM_Collection Collection);
unsigned long         CDM_GetOpenedDocumentIndex(CDM_Collection Collection,
                                                 CDM_Document   Document);
char              *   CDM_GetOwner(CDM_Collection Collection);
unsigned long         CDM_Length(CDM_Collection Collection);
int                   CDM_Modified(CDM_Collection Collection);
int                   CDM_Modified(CDM_Collection Collection, int *result);
int                   CDM_Modified(CDM_Collection Collection, int value);
CDM_Document          CDM_NextDocument(CDM_Collection Collection);
CDM_Collection        CDM_OpenCollection(CDM_CONST char *Path);
int                   CDM_PutAttribute(CDM_Collection Collection,
                                       CDM_Attribute Attr);
int                   CDM_ReadOnly(CDM_Collection Collection);
int                   CDM_ReadOnly(CDM_Collection Collection, int *result);
int                   CDM_ReadOnly(CDM_Collection Collection, int value);
int                   CDM_RemoveAttribute(CDM_Collection Collection, char *Name);
int                   CDM_RemoveDocument(CDM_Collection Collection, char *Id);
int                   CDM_SetAssociatedInfo(CDM_Collection Collection,
                                       Tcl_Obj *Info);
int                   CDM_SetClientData(CDM_Collection Collection, void *data);
int                   CDM_SetComment(CDM_Collection Collection, Tcl_Obj *Info);
char              *   CDM_SetEncoding(CDM_Collection Collection, char *Encoding);
char              *   CDM_SetOwner(CDM_Collection Collection, char *Owner);
char              *   CDM_SetName(CDM_Collection Collection, char *Name);
CDM_ByteSequence      CDM_Status(CDM_Collection Collection);
int                   CDM_Sync(CDM_Collection Collection);
#endif

#endif /* ELLOGON_CDM_Document */

/*
 * vim: nowrap columns=120
 */
