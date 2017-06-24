/* Language independent exception handler */
%include stdint.i
%include typemaps.i
%include exception.i
%include stl.i
%include tclinterp.i
%include attribute.i
%rename("%(regex:/^CDM_(.*)/tip_\\1/)s", %$isfunction) "";
%rename(__lt__) operator<;
%rename(__index__) operator[];

%exception
  %{try {
    $action
  } catch (std::exception &e) {
    Tcl_SetResult(interp, (char *) e.what(), TCL_VOLATILE);
    SWIG_fail;
  } catch (...) {
    SWIG_exception_fail(SWIG_UnknownError, "Unknown exception");
  }%}

%{
const Tcl_ObjType *CDMTcl_stringType  = NULL;
const Tcl_ObjType *CDMTcl_cmdNameType = NULL;
#include <tcl.h>
#include "CDM3.h"
#include "swig/CDM_Tcl.cpp"
%}

%init %{
#ifdef SWIG_namespace
  Tcl_Export(interp, Tcl_FindNamespace(interp, SWIG_namespace, NULL, TCL_GLOBAL_ONLY), "tip_*", 0);
#endif
#if 0
  /* Decrate all tip_* functions also on the global namespace... */
  for (i = 0; swig_commands[i].name; i++) {
    char *tip = strstr((char *) swig_commands[i].name, "tip_");
    if (tip == NULL) continue;
    Tcl_CreateObjCommand(interp, tip,
        (swig_wrapper_func) swig_commands[i].wrapper,
         swig_commands[i].clientdata, NULL);
  }
#endif
  CDM_InitialiseObjTypes();
  CDMTcl_stringType  = Tcl_GetObjType("string");
  CDMTcl_cmdNameType = Tcl_GetObjType("cmdName");
%}

%define TYPE_IN_CONST(cT, cppT, tclT, swigT)
/*
 * const cT
 */
%typemap(in) const cT
%{// Typemap(in) const cT
  try {
    $1 = CDM_CastToOpaque(cT, (CDM_EnsureConstObjectOrNULL<cppT, swigT, &tclT>(interp, $input)));
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) const cT
%{// Typemap(out) const cT
  if (!$1) {
    // We got a NULL pointer. Generate an error message:
    CDM_ReturnNewObject<cppT, &tclT>(interp, CDM_CastFromOpaque(cppT *, $1));
    SWIG_fail;
  }
  // This is a const reference, to a member of another object.
  // Which may be deleted (as it is bound to a caller variable).
  // So, return a copy.
  CDM_ReturnNewObject<cppT, &tclT>(interp, new cppT(*CDM_CastFromOpaque(cppT *, $1)));
  $1 = NULL;%}

/*
 * cT
 */
%typemap(out) cT
%{// Typemap(out) cT
  CDM_ReturnNewObject<cppT, &tclT>(interp, CDM_CastFromOpaque(cppT *, $1));
  if (!$1) SWIG_fail;%}
%enddef

%define TYPE_IN_ANY(cT, cppT, tclT, swigT)
/*
 * const cT
 */
%typemap(in) const cT
%{// Typemap(in) const cT
  try {
    $1 = CDM_CastToOpaque(cT, (CDM_EnsureConstObjectOrNULL<cppT, swigT, &tclT>(interp, $input)));
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) const cT
%{// Typemap(out) const cT
  if (!$1) {
    // We got a NULL pointer. Generate an error message:
    CDM_ReturnNewObject<cppT, &tclT>(interp, CDM_CastFromOpaque(cppT *, $1));
    SWIG_fail;
  }
  // This is a const reference, to a member of another object.
  // Which may be deleted (as it is bound to a caller variable).
  // So, return a copy.
  CDM_ReturnNewObject<cppT, &tclT>(interp, new cppT(*CDM_CastFromOpaque(cppT *, $1)));
  $1 = NULL;%}

%typemap(argout) const cT
%{// Typemap(argout) const cT%}

%typemap(freearg) const cT
%{// Typemap(freearg) const cT%}

/*
 * cT
 */
%typemap(in) cT (int status)
%{// Typemap(in) cT: input=$input, symname=$symname $argnum
  try {
    status = CDM_EnsureObject<cppT, cT, swigT, &tclT>(interp, $input, &$1);
  } catch (std::exception &e) {SWIG_fail;}
  if (status == TCL_ERROR) SWIG_fail;%}

%typemap(freearg) cT
%{// Typemap(freearg) cT: $argnum
  if ($1 && status$argnum == CDM_OBJ_ALLOCATED) delete (CDM_CastFromOpaque(cppT *, $1));
  $1 = NULL;%}

%typemap(argout) cT
%{// Typemap(argout) cT: result=$result, input=$input, sysname=$symname
  if (status$argnum == CDM_OBJ_ALLOCATED) {
    CDM_ReturnNewObject<cppT, &tclT>(interp, CDM_CastFromOpaque(cppT *, $1));
  }
  if (!$1) SWIG_fail; $1 = NULL;%}

%typemap(out) cT
%{// Typemap(out) cT
  CDM_ReturnNewObject<cppT, &tclT>(interp, CDM_CastFromOpaque(cppT *, $1));
  if (!$1) SWIG_fail; $1 = NULL;%}
%enddef

TYPE_IN_CONST(CDM_Span, ELEP::CDM::Span, CDM_Span_ObjType, CDMTYPE_p_ELEP__CDM__Span)
TYPE_IN_ANY  (CDM_SpanSet, ELEP::CDM::SpanSet, CDM_SpanSet_ObjType, CDMTYPE_p_ELEP__CDM__SpanSet)
TYPE_IN_CONST(CDM_AttributeValue, ELEP::CDM::AttributeValue, CDM_AttributeValue_ObjType, CDMTYPE_p_ELEP__CDM__AttributeValue)
TYPE_IN_CONST(CDM_Attribute, ELEP::CDM::Attribute, CDM_Attribute_ObjType, CDMTYPE_p_ELEP__CDM__Attribute)
TYPE_IN_ANY  (CDM_AttributeSet, ELEP::CDM::AttributeSet, CDM_AttributeSet_ObjType, CDMTYPE_p_ELEP__CDM__AttributeSet)
TYPE_IN_ANY  (CDM_Annotation, ELEP::CDM::Annotation, CDM_Annotation_ObjType, CDMTYPE_p_ELEP__CDM__Annotation)
TYPE_IN_ANY  (CDM_AnnotationSet, ELEP::CDM::AnnotationSet, CDM_AnnotationSet_ObjType, CDMTYPE_p_ELEP__CDM__AnnotationSet)
TYPE_IN_ANY  (CDM_Document, ELEP::CDM::Document, CDM_Document_ObjType, CDMTYPE_p_ELEP__CDM__Document)
//TYPE_IN_ANY(Collection)

/*
 * Integer types
 */

%typemap(out) ELEP::CDM::Position
%{// Typemap(out) ELEP::CDM::Position
  if ($1 != ELEP::CDM::Span::no) {
    Tcl_SetObjResult(interp, SWIG_From_unsigned_SS_long(static_cast< unsigned long >($1)));
  } else {
    Tcl_SetObjResult(interp, Tcl_NewStringObj("", 0));
  };%}

%typemap(out) CDM_Position = ELEP::CDM::Position;

%typemap(in) ELEP::CDM::Position& (ELEP::CDM::Position position)
%{// Typemap(in) ELEP::CDM::Position&
  $1 = &position;%}

%typemap(argout) ELEP::CDM::Position&
%{// Typemap(argout) ELEP::CDM::Position&: $result, $input, $symname, $1
  if (Tcl_ObjSetVar2(interp, $input, NULL, SWIG_From_unsigned_SS_long(*$1),
                     TCL_LEAVE_ERR_MSG) == NULL) SWIG_fail;%}

%typemap(out) ELEP::CDM::Position&
%{// Typemap(out) ELEP::CDM::Position&
  if (*$1 != ELEP::CDM::Span::no) {
    Tcl_SetObjResult(interp, SWIG_From_unsigned_SS_long(static_cast< unsigned long >(*$1)));
  } else {
    Tcl_SetObjResult(interp, Tcl_NewStringObj(nullptr, 0));
  };%}

%typemap(in)     CDM_Position* = ELEP::CDM::Position&;
%typemap(argout) CDM_Position* = ELEP::CDM::Position&;
%typemap(out)    CDM_Position* = ELEP::CDM::Position&;

%typemap(out) ELEP::CDM::SpanSet::size_type = unsigned int;

%typemap(out) CDM_Status
%{// Typemap(out) CDM_Status
  if ($1 == CDM_ERROR) SWIG_fail;%}

%typemap(out) CDM_Id
%{// Typemap(out) CDM_Id
  if ($1 != ELEP::CDM::Annotation::no) {
    Tcl_SetObjResult(interp, SWIG_From_unsigned_SS_long(static_cast< unsigned long >($1)));
  } else {
    Tcl_SetObjResult(interp, Tcl_NewStringObj("", 0));
  };%}

%typemap(in) const CDM_AttributeType
%{{// Typemap(in) const CDM_AttributeType: $input, $1
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
  int index;
  if (Tcl_GetIndexFromObj(interp, $input, (const char **) Types,
                              "type", 0, &index) != TCL_OK) {
    SWIG_fail;
  }
  switch ((types) index) {
    case NONE:                  {$1 = CDM_NONE;                  break;}
    case STRING:                {$1 = CDM_STRING;                break;}
    case STRING_SET:            {$1 = CDM_STRING_SET;            break;}
    case SPAN:                  {$1 = CDM_SPAN;                  break;}
    case SPAN_SET:              {$1 = CDM_SPAN_SET;              break;}
    case ANNOTATION_ID:         {$1 = CDM_ANNOTATION_ID;         break;}
    case ANNOTATION_ID_SET:     {$1 = CDM_ANNOTATION_ID_SET;     break;}
    case IMAGE:                 {$1 = CDM_IMAGE;                 break;}
    case BASE64_IMAGE:          {$1 = CDM_BASE64_IMAGE;          break;}
  }
}%}

%typemap(out) CDM_AttributeType
%{{// Typemap(out) CDM_AttributeType: $1
  const char *cdms = NULL;
  switch ($1) {
    case CDM_NONE:              {cdms = "CDM_NONE";              break;}
    case CDM_STRING:            {cdms = "CDM_STRING";            break;}
    case CDM_STRING_SET:        {cdms = "CDM_STRING_SET";        break;}
    case CDM_SPAN:              {cdms = "CDM_SPAN";              break;}
    case CDM_SPAN_SET:          {cdms = "CDM_SPAN_SET";          break;}
    case CDM_ANNOTATION_ID:     {cdms = "CDM_ANNOTATION_ID";     break;}
    case CDM_ANNOTATION_ID_SET: {cdms = "CDM_ANNOTATION_ID_SET"; break;}
    case CDM_IMAGE:             {cdms = "CDM_IMAGE";             break;}
    case CDM_BASE64_IMAGE:      {cdms = "CDM_BASE64_IMAGE";      break;}
  }
  Tcl_SetResult(interp, (char *) cdms, TCL_STATIC);}%}

%include "Serialisation.h"
%template(serialisation_Span)              ELEP::CDM::serialisation::Serialisation<ELEP::CDM::Span>;
%template(serialisation_SpanSet)           ELEP::CDM::serialisation::Serialisation<ELEP::CDM::SpanSet>;
%template(serialisation_AttributeValue)    ELEP::CDM::serialisation::Serialisation<ELEP::CDM::AttributeValue>;
%template(serialisation_Attribute)         ELEP::CDM::serialisation::Serialisation<ELEP::CDM::Attribute>;
%template(serialisation_AttributeSet)      ELEP::CDM::serialisation::Serialisation<ELEP::CDM::AttributeSet>;
%template(serialisation_Annotation)        ELEP::CDM::serialisation::Serialisation<ELEP::CDM::Annotation>;
%template(serialisation_AnnotationSet)     ELEP::CDM::serialisation::Serialisation<ELEP::CDM::AnnotationSet>;
%template(serialisation_Document)          ELEP::CDM::serialisation::Serialisation<ELEP::CDM::Document>;
%template(serialisation_Collection)        ELEP::CDM::serialisation::Serialisation<ELEP::CDM::Collection>;

%ignore ELEP::CDM::Functor::UnaryFunction;
%ignore ELEP::CDM::Functor::UnaryFunctionWithState;
%ignore ELEP::CDM::Functor::UnaryFunctionWithStates;
%ignore ELEP::CDM::Functor::UnaryPredicate;
%ignore ELEP::CDM::Functor::UnaryPredicateWithState;
%ignore ELEP::CDM::Functor::UnaryPredicateWithStates;
%ignore ELEP::CDM::Functor::AnnotationUnaryPredicate;
%ignore ELEP::CDM::Functor::AnnotationUnaryPredicateWithState;
%ignore ELEP::CDM::Functor::AnnotationUnaryPredicateWithState<ELEP::CDM::Position>;
%ignore ELEP::CDM::Functor::AnnotationUnaryPredicateWithStates;
%ignore ELEP::CDM::Functor::AnnotationUnaryPredicateWithStringState;

%nodefaultctor;

%include "CDM3.h"

/*
 * vim: nowrap columns=120
 */
