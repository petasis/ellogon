/* Language independent exception handler */
%include typemaps.i
%include exception.i
%include stl.i
%include tclinterp.i
%include attribute.i
%rename("%(regex:/^CDM_(.*)/tip_\\1/)s", %$isfunction) "";
%rename(__lt__) operator<;
%rename(__index__) operator[];

%exception {
  try {
    $action
  } catch (std::exception &e) {
    Tcl_SetResult(interp, (char *) e.what(), TCL_VOLATILE);
    SWIG_fail;
  } catch (...) {
    SWIG_exception_fail(SWIG_UnknownError, "Unknown exception");
  }
}

%{
#include "swig/CDM_Tcl.cpp"
%}

%init %{
  /* Decrate all tip_* functions also on the global namespace... */
  for (i = 0; swig_commands[i].name; i++) {
    char *tip = strstr((char *) swig_commands[i].name, "tip_");
    if (tip == NULL) continue;
    Tcl_CreateObjCommand(interp, tip,
        (swig_wrapper_func) swig_commands[i].wrapper,
         swig_commands[i].clientdata, NULL);
  }
  CDM_InitialiseObjTypes();
%}

/*
 * CDM_Span
 */
%typemap(in) const CDM_Span
%{// Typemap(in) const CDM_Span
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::Span,
         &CDM_Span_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) CDM_Span
%{// Typemap(out) CDM_Span
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::Span, &CDM_Span_ObjType>
         (reinterpret_cast<ELEP::CDM::Span*>($1)));
  if (!$1) SWIG_fail;
%}

/*
 * const CDM_SpanSet
 */
%typemap(in) const CDM_SpanSet
%{// Typemap(in) const CDM_SpanSet
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::SpanSet,
         &CDM_SpanSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}
/*
  if (CDM_EnsureObject<ELEP::CDM::SpanSet,
         &CDM_SpanSet_ObjType>(interp, $input) == TCL_ERROR) SWIG_fail;
  $1 = $input->internalRep.twoPtrValue.ptr1;%}
*/

%typemap(argout) const CDM_SpanSet
%{// Typemap(argout) const CDM_SpanSet%}

%typemap(freearg) const CDM_SpanSet
%{// Typemap(freearg) const CDM_SpanSet%}

/*
 * CDM_SpanSet
 */
%typemap(in) CDM_SpanSet (int status)
%{// Typemap(in) CDM_SpanSet: input=$input, symname=$symname $argnum
  try {
    status = CDM_EnsureObject<ELEP::CDM::SpanSet,
                &CDM_SpanSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}
  if (status == TCL_ERROR) SWIG_fail;
  // We need to modify the object, so create a copy...
  $1 = new ELEP::CDM::SpanSet(*reinterpret_cast<ELEP::CDM::SpanSet*>(
     $input->internalRep.twoPtrValue.ptr1));
%}

%typemap(freearg) CDM_SpanSet {
  if ($1) delete (reinterpret_cast<ELEP::CDM::SpanSet*>($1));
  $1 = NULL;
}

%typemap(argout) CDM_SpanSet
%{// Typemap(argout) CDM_SpanSet: result=$result, input=$input, sysname=$symname
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::SpanSet, &CDM_SpanSet_ObjType>
         (reinterpret_cast<ELEP::CDM::SpanSet*>($1)));
 if (!$1) SWIG_fail; $1 = NULL;%}

%typemap(out) CDM_SpanSet
%{// Typemap(out) CDM_SpanSet
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::SpanSet, &CDM_SpanSet_ObjType>
         (reinterpret_cast<ELEP::CDM::SpanSet*>($1)));
  if (!$1) SWIG_fail; $1 = NULL;%}

/*
 * CDM_AttributeValue
 */
%typemap(in) const CDM_AttributeValue
%{// Typemap(in) const CDM_AttributeValue
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::AttributeValue,
         &CDM_AttributeValue_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) CDM_AttributeValue
%{// Typemap(out) CDM_AttributeValue
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::AttributeValue, &CDM_AttributeValue_ObjType>
         (reinterpret_cast<ELEP::CDM::AttributeValue*>($1)));
  if (!$1) SWIG_fail;%}

/*
 * CDM_Attribute
 */
%typemap(in) const CDM_Attribute
%{// Typemap(in) const CDM_Attribute
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::Attribute,
         &CDM_Attribute_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) CDM_Attribute
%{// Typemap(out) CDM_Attribute
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::Attribute, &CDM_Attribute_ObjType>
         (reinterpret_cast<ELEP::CDM::Attribute*>($1)));
  if (!$1) SWIG_fail;%}

/*
 * const CDM_AttributeSet
 */
%typemap(in) const CDM_AttributeSet
%{// Typemap(in) const CDM_AttributeSet
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::AttributeSet,
         &CDM_AttributeSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(argout) const CDM_AttributeSet
%{// Typemap(argout) const CDM_AttributeSet%}

%typemap(freearg) const CDM_AttributeSet
%{// Typemap(freearg) const CDM_AttributeSet%}

/*
 * CDM_AttributeSet
 */
%typemap(in) CDM_AttributeSet (int status)
%{// Typemap(in) CDM_AttributeSet: input=$input, symname=$symname $argnum
  try {
    status = CDM_EnsureObject<ELEP::CDM::AttributeSet,
                &CDM_AttributeSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}
  if (status == TCL_ERROR) SWIG_fail;
  // We need to modify the object, so create a copy...
  $1 = new ELEP::CDM::AttributeSet(*reinterpret_cast<ELEP::CDM::AttributeSet*>(
     $input->internalRep.twoPtrValue.ptr1));
%}

%typemap(freearg) CDM_AttributeSet {
  if ($1) delete (reinterpret_cast<ELEP::CDM::AttributeSet*>($1));
  $1 = NULL;
}

%typemap(argout) CDM_AttributeSet
%{// Typemap(argout) CDM_AttributeSet: result=$result, input=$input, sysname=$symname
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::AttributeSet, &CDM_AttributeSet_ObjType>
         (reinterpret_cast<ELEP::CDM::AttributeSet*>($1)));
  if (!$1) SWIG_fail; $1 = NULL;%}

%typemap(out) CDM_AttributeSet
%{// Typemap(out) CDM_AttributeSet
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::AttributeSet, &CDM_AttributeSet_ObjType>
         (reinterpret_cast<ELEP::CDM::AttributeSet*>($1)));
  if (!$1) SWIG_fail; $1 = NULL;%}

/*
 * CDM_Annotation
 */
%typemap(in) const CDM_Annotation
%{// Typemap(in) const CDM_Annotation
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::Annotation,
         &CDM_Annotation_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(out) CDM_Annotation
%{// Typemap(out) CDM_Annotation
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::Annotation, &CDM_Annotation_ObjType>
         (reinterpret_cast<ELEP::CDM::Annotation*>($1)));
  if (!$1) SWIG_fail;%}

/*
 * const CDM_AnnotationSet
 */
%typemap(in) const CDM_AnnotationSet
%{// Typemap(in) const CDM_AnnotationSet
  try {
    $1 = CDM_EnsureConstObjectOrNULL<ELEP::CDM::AnnotationSet,
         &CDM_AnnotationSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}%}

%typemap(argout) const CDM_AnnotationSet
%{// Typemap(argout) const CDM_AnnotationSet%}

%typemap(freearg) const CDM_AnnotationSet
%{// Typemap(freearg) const CDM_AnnotationSet%}

/*
 * CDM_AnnotationSet
 */
%typemap(in) CDM_AnnotationSet (int status)
%{// Typemap(in) CDM_AnnotationSet: input=$input, symname=$symname $argnum
  try {
    status = CDM_EnsureObject<ELEP::CDM::AnnotationSet,
                &CDM_AnnotationSet_ObjType>(interp, $input);
  } catch (std::exception &e) {SWIG_fail;}
  if (status == TCL_ERROR) SWIG_fail;
  // We need to modify the object, so create a copy...
  $1 = new ELEP::CDM::AnnotationSet(*reinterpret_cast<ELEP::CDM::AnnotationSet*>(
     $input->internalRep.twoPtrValue.ptr1));
%}

%typemap(freearg) CDM_AnnotationSet {
  if ($1) delete (reinterpret_cast<ELEP::CDM::AnnotationSet*>($1));
  $1 = NULL;
}

%typemap(argout) CDM_AnnotationSet
%{// Typemap(argout) CDM_AnnotationSet: result=$result, input=$input, sysname=$symname
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::AnnotationSet, &CDM_AnnotationSet_ObjType>
         (reinterpret_cast<ELEP::CDM::AnnotationSet*>($1)));
  if (!$1) SWIG_fail; $1 = NULL;%}

%typemap(out) CDM_AnnotationSet
%{// Typemap(out) CDM_AnnotationSet
  Tcl_SetObjResult(interp,
      CDM_NewObject<ELEP::CDM::AnnotationSet, &CDM_AnnotationSet_ObjType>
         (reinterpret_cast<ELEP::CDM::AnnotationSet*>($1)));
  if (!$1) SWIG_fail; $1 = NULL;%}

/*
 * Integer types
 */

%typemap(in) ELEP::CDM::Position& (ELEP::CDM::Position position)
%{// Typemap(in) ELEP::CDM::Position&
  $1 = &position;%}

%typemap(argout) ELEP::CDM::Position&
%{// Typemap(argout) ELEP::CDM::Position&: $result, $input, $symname, $1
  if (Tcl_ObjSetVar2(interp, $input, NULL, SWIG_From_unsigned_SS_long(*$1),
                     TCL_LEAVE_ERR_MSG) == NULL) SWIG_fail;%}

%typemap(in)     CDM_Position* = ELEP::CDM::Position&;
%typemap(argout) CDM_Position* = ELEP::CDM::Position&;

%typemap(out) ELEP::CDM::SpanSet::size_type = unsigned int;

%typemap(out) CDM_Status
%{// Typemap(out) CDM_Status
  if ($1 != CDM_OK) SWIG_fail;%}

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

%include "CDM3.h"
