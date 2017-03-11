/*****************************************************************************
 *  CDM_Tcl.cpp:                                                             *
 * ----------------------                                                    *
 *  This file implements a set of new Tcl object types.                      *
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
#include <tcl.h>
#include "CDM3.h"
#include <iostream>
#include <sstream>
#include <typeinfo>

#include <cxxabi.h>

typedef struct cdm_type_info {
  const char           *pointerT;
  swig_type_info       *swigT;
} cdm_type_info;

static const cdm_type_info cdm_swig_pointer_type[] = {
  {"_p_ELEP__CDM__Span",            SWIGTYPE_p_ELEP__CDM__Span},
  {"_p_ELEP__CDM__SpanSet",         SWIGTYPE_p_ELEP__CDM__SpanSet},
  {"_p_ELEP__CDM__AttributeValue",  SWIGTYPE_p_ELEP__CDM__AttributeValue},
  {"_p_ELEP__CDM__Attribute",       SWIGTYPE_p_ELEP__CDM__Attribute},
  {"_p_ELEP__CDM__AttributeSet",    SWIGTYPE_p_ELEP__CDM__AttributeSet},
  {"_p_ELEP__CDM__Annotation",      SWIGTYPE_p_ELEP__CDM__Annotation},
  {"_p_ELEP__CDM__AnnotationSet",   SWIGTYPE_p_ELEP__CDM__AnnotationSet},
  {"_p_ELEP__CDM__Document",        SWIGTYPE_p_ELEP__CDM__Document},
  {"_p_ELEP__CDM__Collection",      SWIGTYPE_p_ELEP__CDM__Collection},
};
#define CDMTYPE_p_ELEP__CDM__Span             0
#define CDMTYPE_p_ELEP__CDM__SpanSet          1
#define CDMTYPE_p_ELEP__CDM__AttributeValue   2
#define CDMTYPE_p_ELEP__CDM__Attribute        3
#define CDMTYPE_p_ELEP__CDM__AttributeSet     4
#define CDMTYPE_p_ELEP__CDM__Annotation       5
#define CDMTYPE_p_ELEP__CDM__AnnotationSet    6
#define CDMTYPE_p_ELEP__CDM__Document         7
#define CDMTYPE_p_ELEP__CDM__Collection       8

template<typename T>
void Object_FreeInternalRep(Tcl_Obj *obj) {
  if (obj->internalRep.twoPtrValue.ptr1) {
    T *p = static_cast<T*> (obj->internalRep.twoPtrValue.ptr1);
    if (p != NULL) delete p;
  }
  obj->internalRep.twoPtrValue.ptr1 = NULL;
  obj->internalRep.twoPtrValue.ptr2 = NULL;
  obj->typePtr = NULL;
}

template<typename T, Tcl_ObjType *type>
void Object_DuplicateInternalRep(Tcl_Obj *src, Tcl_Obj *copy) {
  copy->internalRep.twoPtrValue.ptr1 = NULL;
  copy->internalRep.twoPtrValue.ptr2 = NULL;
  copy->typePtr = NULL;
  if (src->internalRep.twoPtrValue.ptr1) {
    const T *p = static_cast<const T*> (src->internalRep.twoPtrValue.ptr1);
    if (p == NULL) return;
    copy->internalRep.twoPtrValue.ptr1 = new T((const T) *p);
    copy->typePtr = type;
  }
};

template<typename T>
void Object_UpdateString(Tcl_Obj *obj) {
  T *p = static_cast<T*> (obj->internalRep.twoPtrValue.ptr1);
  obj->bytes = NULL; obj->length = 0;
  if (p == NULL) return;
  const std::string str = p->toString();
  obj->length = str.size();
  obj->bytes = Tcl_Alloc(obj->length+1);
  str.copy(obj->bytes, str.size());
  obj->bytes[obj->length] = '\0';
}

template<typename T, Tcl_ObjType *type>
int Object_SetFromAny(Tcl_Interp *interp, Tcl_Obj *obj) {
  void *p;
  if (obj->typePtr == type) return TCL_OK;
  /* Try to create an object from the Tcl object... */
  try {
    p = new T(interp, obj);
  }
  catch (std::exception &e) {
    // Conversion failed...
    int status;
    std::string msg("cannot convert input to \"");
    msg += abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    msg += "\": ";
    msg += e.what();
    Tcl_SetResult(interp, (char *) msg.c_str(), TCL_VOLATILE);
    return TCL_ERROR;
  }
  if (!p) {
    std::string msg("call returned NULL pointer: ");
    msg += Tcl_GetStringResult(interp);
    throw std::invalid_argument(msg);
  }
  /* The object has been created! */
  if (obj->typePtr != NULL && obj->typePtr->freeIntRepProc != NULL) {
    obj->typePtr->freeIntRepProc(obj);
  }
  //Tcl_InvalidateStringRep(obj);
  obj->typePtr = type;
  obj->internalRep.twoPtrValue.ptr1 = p;
  obj->internalRep.twoPtrValue.ptr2 = NULL;
  return TCL_OK;
}

template<typename T, Tcl_ObjType *type>
Tcl_Obj *CDM_NewObject(T *p) {
  if (!p) {
    return Tcl_NewStringObj("call returned NULL pointer", -1);
  }
  Tcl_Obj *obj = Tcl_NewObj();
  Tcl_InvalidateStringRep(obj);
  obj->typePtr = type;
  obj->internalRep.twoPtrValue.ptr2 = NULL;
  obj->internalRep.twoPtrValue.ptr1 = p;
  return obj;
}

template<typename T, Tcl_ObjType *type>
void CDM_ReturnNewObject(Tcl_Interp *interp, T *p) {
  Tcl_SetObjResult(interp, CDM_NewObject<T, type>(p));
}

#define CDM_OBJ_ALLOCATED           100
#define CDM_OBJ_SWIG_OBJECT_REUSED  101
template<typename T, typename cT, const int cdmT, Tcl_ObjType *type>
int CDM_EnsureObject(Tcl_Interp *interp, Tcl_Obj *obj, cT *v) {
  if (obj->typePtr == type) {
    // We need to modify the object, so create a copy...
    *v = new T(*static_cast<T*>(obj->internalRep.twoPtrValue.ptr1));
    return CDM_OBJ_ALLOCATED;
  }
  /* If obj->typePtr == nullptr, we have a "pure string"... */
  if (!obj->typePtr || obj->typePtr == CDMTcl_StringType) {
    void *p = 0;
    int res = SWIG_ConvertPtr(obj, &p, cdm_swig_pointer_type[cdmT].swigT, 0);
    if (SWIG_IsOK(res)) {
      *v = static_cast<cT *>(p);
      return CDM_OBJ_SWIG_OBJECT_REUSED;
    }
  }
  int status;
  try {
    status = Object_SetFromAny<T, type>(interp, obj);
  } catch (std::exception &e) {
    Tcl_SetResult(interp, (char *) e.what(), TCL_VOLATILE);
    return TCL_ERROR;
  }
  if (status == TCL_ERROR) return TCL_ERROR;
  // We need to modify the object, so create a copy...
  *v = new T(*static_cast<T*>(obj->internalRep.twoPtrValue.ptr1));
  return CDM_OBJ_ALLOCATED;
}

template<typename T, const int cdmT, Tcl_ObjType *type>
void *CDM_EnsureConstObjectOrNULL(Tcl_Interp *interp, Tcl_Obj *obj) {
  // printf("2: %s\n", Tcl_GetString(obj));
  // printf("2: %p -> %p (%p)\n", obj->typePtr, type, CDMTcl_StringType);
  // if (obj->typePtr) {
  //   printf("2: %s -> %s\n", obj->typePtr->name, type->name);
  // }
  // fflush(0);
  if (obj->typePtr == type) {
    //Tcl_InvalidateStringRep(obj);
    return obj->internalRep.twoPtrValue.ptr1;
  }
  /* If obj->typePtr == nullptr, we have a "pure string"... */
  if (!obj->typePtr || obj->typePtr == CDMTcl_StringType) {
    void *p = 0;
    int res = SWIG_ConvertPtr(obj, &p, cdm_swig_pointer_type[cdmT].swigT, 0);
    if (SWIG_IsOK(res)) return p;
  }
  // Since all our objects are lists, take a shortcut:
  // If the object is the empty list, return NULL, and do not
  // bother converting the object...
  int status, len;
  if (Tcl_ListObjLength(interp, obj, &len) == TCL_OK && len == 0)
      return nullptr;
  try {
    status = Object_SetFromAny<T, type>(interp, obj);
  } catch (std::exception &e) {
    Tcl_SetResult(interp, (char *) e.what(), TCL_VOLATILE);
    throw e;
  }
  if (status != TCL_OK) {
    // Conversion failed...
    //  std::string msg("cannot convert input to \"");
    //  msg += abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
    //  msg += "\": ";
    //  msg += Tcl_GetStringResult(interp);
    //  Tcl_SetResult(interp, (char *) msg.c_str(), TCL_VOLATILE);
    throw std::runtime_error(Tcl_GetStringResult(interp));
  }
  return obj->internalRep.twoPtrValue.ptr1;
}

Tcl_ObjType CDM_Span_ObjType = {
    (char *) "CDM_Span",                                /* name */
    Object_FreeInternalRep<ELEP::CDM::Span>,            /* freeIntRepProc */
    Object_DuplicateInternalRep<ELEP::CDM::Span,
                                &CDM_Span_ObjType>,     /* dupIntRepProc */
    Object_UpdateString<ELEP::CDM::Span>,               /* updateStringProc */
    Object_SetFromAny<ELEP::CDM::Span,
                                &CDM_Span_ObjType>      /* setFromAnyProc */
};

Tcl_ObjType CDM_SpanSet_ObjType = {
    (char *) "CDM_SpanSet",                             /* name */
    Object_FreeInternalRep<ELEP::CDM::SpanSet>,         /* freeIntRepProc */
    Object_DuplicateInternalRep<ELEP::CDM::SpanSet,
                                &CDM_SpanSet_ObjType>,  /* dupIntRepProc */
    Object_UpdateString<ELEP::CDM::SpanSet>,            /* updateStringProc */
    Object_SetFromAny<ELEP::CDM::SpanSet,
                                &CDM_SpanSet_ObjType>   /* setFromAnyProc */
};

Tcl_ObjType CDM_AttributeValue_ObjType = {
    (char *) "CDM_AttributeValue",                            /* name */
    Object_FreeInternalRep<ELEP::CDM::AttributeValue>,        /* freeIntRepProc */
    Object_DuplicateInternalRep<ELEP::CDM::AttributeValue,
                                &CDM_AttributeValue_ObjType>, /* dupIntRepProc */
    Object_UpdateString<ELEP::CDM::AttributeValue>,           /* updateStringProc */
    Object_SetFromAny<ELEP::CDM::AttributeValue,
                                &CDM_AttributeValue_ObjType>  /* setFromAnyProc */
};

Tcl_ObjType CDM_Attribute_ObjType = {
    (char *) "CDM_Attribute",                            /* name */
    Object_FreeInternalRep<ELEP::CDM::Attribute>,        /* freeIntRepProc */
    Object_DuplicateInternalRep<ELEP::CDM::Attribute,
                                &CDM_Attribute_ObjType>, /* dupIntRepProc */
    Object_UpdateString<ELEP::CDM::Attribute>,           /* updateStringProc */
    Object_SetFromAny<ELEP::CDM::Attribute,
                                &CDM_Attribute_ObjType>  /* setFromAnyProc */
};

Tcl_ObjType CDM_AttributeSet_ObjType = {
    (char *) "CDM_AttributeSet",                           /* name */
    Object_FreeInternalRep<ELEP::CDM::AttributeSet>,       /* freeIntRepProc */
    Object_DuplicateInternalRep<ELEP::CDM::AttributeSet,
                                &CDM_AttributeSet_ObjType>,/* dupIntRepProc */
    Object_UpdateString<ELEP::CDM::AttributeSet>,          /* updateStringProc*/
    Object_SetFromAny<ELEP::CDM::AttributeSet,
                                &CDM_AttributeSet_ObjType> /* setFromAnyProc */
};

Tcl_ObjType CDM_Annotation_ObjType = {
    (char *) "CDM_Annotation",
    Object_FreeInternalRep<ELEP::CDM::Annotation>,
    Object_DuplicateInternalRep<ELEP::CDM::Annotation, &CDM_Annotation_ObjType>,
    Object_UpdateString<ELEP::CDM::Annotation>,
    Object_SetFromAny<ELEP::CDM::Annotation, &CDM_Annotation_ObjType>
};

Tcl_ObjType CDM_AnnotationSet_ObjType = {
    (char *) "CDM_AnnotationSet",
    Object_FreeInternalRep<ELEP::CDM::AnnotationSet>,
    Object_DuplicateInternalRep<ELEP::CDM::AnnotationSet, &CDM_AnnotationSet_ObjType>,
    Object_UpdateString<ELEP::CDM::AnnotationSet>,
    Object_SetFromAny<ELEP::CDM::AnnotationSet, &CDM_AnnotationSet_ObjType>
};

Tcl_ObjType CDM_Document_ObjType = {
    (char *) "CDM_Document",
    Object_FreeInternalRep<ELEP::CDM::Document>,
    Object_DuplicateInternalRep<ELEP::CDM::Document, &CDM_Document_ObjType>,
    Object_UpdateString<ELEP::CDM::Document>,
    Object_SetFromAny<ELEP::CDM::Document, &CDM_Document_ObjType>
};

Tcl_ObjType CDM_Collection_ObjType = {
    (char *) "CDM_Collection",
    Object_FreeInternalRep<ELEP::CDM::Collection>,
    Object_DuplicateInternalRep<ELEP::CDM::Collection, &CDM_Collection_ObjType>,
    Object_UpdateString<ELEP::CDM::Collection>,
    Object_SetFromAny<ELEP::CDM::Collection, &CDM_Collection_ObjType>
};

void CDM_InitialiseObjTypes() {
  Tcl_RegisterObjType(&CDM_Span_ObjType);
  Tcl_RegisterObjType(&CDM_SpanSet_ObjType);
  Tcl_RegisterObjType(&CDM_AttributeValue_ObjType);
  Tcl_RegisterObjType(&CDM_Attribute_ObjType);
  Tcl_RegisterObjType(&CDM_AttributeSet_ObjType);
  Tcl_RegisterObjType(&CDM_Annotation_ObjType);
  Tcl_RegisterObjType(&CDM_AnnotationSet_ObjType);
  Tcl_RegisterObjType(&CDM_Document_ObjType);
  Tcl_RegisterObjType(&CDM_Collection_ObjType);
};
