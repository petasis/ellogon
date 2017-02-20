/*****************************************************************************
 *  Attribute.h:                                                             *
 * ----------------------                                                    *
 *  This file defines the Attribute section of the                           *
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

#ifndef ELLOGON_CDM_Attribute
#define ELLOGON_CDM_Attribute

#ifndef SWIG
#include <string>      /* For std::string         */
#include <vector>      /* For std::vector         */
#include <memory>      /* For smart pointers      */
#include <regex>       /* For regular expressions */
#include <initializer_list>
#endif /* SWIG */

#ifdef __cplusplus
namespace ELEP {
  namespace CDM {
    class AttributeValue;
    class Attribute;
    class AttributeSet;

#ifndef SWIG
    namespace swap {
      void attributevalue(AttributeValue& first, AttributeValue& second);
      void attribute(Attribute& first, Attribute& second);
      void attributeset(AttributeSet& first, AttributeSet& second);
    };

    namespace convert {
      const ELEP::CDM::AttributeType AttributeType(const CDM_AttributeType type);
      const CDM_AttributeType AttributeType(const ELEP::CDM::AttributeType type);
    };
#endif /* SWIG */

    class AttributeValue {
      public:
        AttributeValue();
        AttributeValue(const char *value, const AttributeType type=AttributeType::STRING);
        AttributeValue(const std::string &value, const AttributeType type=AttributeType::STRING);
        AttributeValue(const AttributeValue& src);
#ifndef SWIG
        AttributeValue(AttributeValue&& other) : AttributeValue() {swap::attributevalue(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        AttributeValue(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        const AttributeType&   type()  const;
        const std::string&     value() const;
        bool                   valueMatches(const char *pattern) const;
#ifndef SWIG
        bool                   valueMatches(const std::string& pattern) const;
        bool                   valueMatches(const std::regex& pattern) const;
#endif /* SWIG */
        bool                   valid() const;
        const std::string      toString() const;

        bool operator< (const AttributeValue& AttributeValue) const;
        AttributeValue& operator=(AttributeValue other) {swap::attributevalue(*this, other); return *this;}
      private:
        void store_value(const std::string &value);
        AttributeType                  _type;
        //union {
          std::shared_ptr<std::string> _v_string;
        //};
        friend void swap::attributevalue(class AttributeValue& first, class AttributeValue& second);
    }; /* class AttributeValue */

    class Attribute {
      public:
        Attribute();
        Attribute(const char *name);
        Attribute(const std::string &name);
        Attribute(const char *name, const char *value, const AttributeType type=AttributeType::STRING);
        Attribute(const std::string &name, const std::string &value, const AttributeType type=AttributeType::STRING);
        Attribute(const char *name, const AttributeValue &value);
        Attribute(const std::string &name, const AttributeValue &value);
        Attribute(const Attribute& src);
#ifndef SWIG
        // Attribute does not have constructor with move semantics for AttributeValue.
        Attribute(Attribute&& other) : Attribute() {swap::attribute(*this, other);}
#endif /* SWIG */
#ifdef TCL_VERSION
        Attribute(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        const std::string&     name()  const;
        const AttributeType&   type()  const;
        const std::string&     value() const;
        bool                   valueMatches(const char *pattern) const;
#ifndef SWIG
        bool                   valueMatches(const std::string& pattern) const;
        bool                   valueMatches(const std::regex& pattern) const;
#endif /* SWIG */
        bool                   valid() const;
        const std::string      toString() const;
        bool operator< (const Attribute& Attribute) const;
        Attribute& operator=(Attribute other) {swap::attribute(*this, other); return *this;}
      private:
        void store_value(const std::string &value);
        AttributeType                _type;
        std::shared_ptr<std::string> _name;
        //union {
          std::shared_ptr<std::string> _v_string;
        //};
        friend void swap::attribute(class Attribute& first, class Attribute& second);
    }; /* class Attribute */

    class AttributeSet {
      public:
        typedef typename std::vector<Attribute>::value_type value_type;
        typedef typename std::vector<Attribute>::size_type size_type;
        typedef typename std::vector<Attribute>::iterator iterator;
        typedef typename std::vector<Attribute>::const_iterator const_iterator;
        typedef typename std::vector<Attribute>::reverse_iterator reverse_iterator;
        typedef typename std::vector<Attribute>::const_reverse_iterator const_reverse_iterator;
#ifndef SWIG
        typedef typename std::vector<Attribute>::reference reference;
        typedef typename std::vector<Attribute>::const_reference const_reference;
#else  /* SWIG */
        typedef       typename Attribute& reference;
        typedef const typename Attribute& const_reference;
#endif /* SWIG */

        AttributeSet();
        AttributeSet(const Attribute &attribute);
        AttributeSet(const char *name, const AttributeValue &value);
        AttributeSet(const std::string &name, const AttributeValue &value);
        AttributeSet(const char *name, const char *value, const AttributeType type=AttributeType::STRING);
        AttributeSet(const std::string &name, const std::string &value, const AttributeType type=AttributeType::STRING);
        AttributeSet(const AttributeSet& src);
#ifndef SWIG
        AttributeSet(AttributeSet&& other) : AttributeSet() {swap::attributeset(*this, other);}
#endif /* SWIG */

#ifdef TCL_VERSION
        AttributeSet(Tcl_Interp *interp, Tcl_Obj *obj);
#endif /* TCL_VERSION */
        bool              exists(const char *name) const {return find(name) != set.end();};
        bool              exists(const std::string &name) const {return find(name) != set.end();};
#ifndef SWIG
        iterator          find(const char *name);
        iterator          find(const std::string &name);
#endif
        const_iterator    find(const char *name) const;
        const_iterator    find(const std::string &name) const;
        bool              valid() const;
        const std::string toString() const;
#ifndef SWIG
        void              ensure_unique();
#endif /* SWIG */
        bool              containsAttributeMatchingValue(const char *name, const char *pattern) const;
#ifndef SWIG
        bool              containsAttributeMatchingValue(const std::string& name, const std::string& pattern) const;
        bool              containsAttributeMatchingValue(const char *name, const std::regex& pattern) const;
        bool              containsAttributeMatchingValue(const std::string& name, const std::regex& pattern) const;
#endif /* SWIG */

        void              putAttribute(const Attribute& attribute) {push_back(attribute);};
        void              removeAttribute(const char *name);

        /* The following methods simulate std::vector. */
        template <class InputIterator>
        // void              assign (InputIterator first, InputIterator last) {set.assign(first, last);};
        // void              assign(size_type n, const value_type& val) {set.assign(n, val);};
#ifndef SWIG
        // reference         at(size_type n) {return set.at(n);};
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
        iterator          insert(const_iterator position, const value_type& val) {assert_not_exists(val); return set.insert(position, val);};
#ifndef SWIG
        template <class InputIterator>
        // iterator          insert(const_iterator position, InputIterator first, InputIterator last) {return set.insert(position, first, last);};
        iterator          insert(const_iterator position, value_type&& val) {assert_not_exists(val); return set.insert(position, val);};
        // iterator          insert(const_iterator position, std::initializer_list<value_type> il) {return set.insert(position, il);};
#endif /* SWIG */
        size_type         max_size() const noexcept {return set.max_size();};
#ifndef SWIG
        // reference         operator[] (size_type n) {return set[n];};
#endif /* SWIG */
        const_reference   operator[] (size_type n) const {return set[n];};
        void              pop_back() {set.pop_back();};
        void              push_back(const value_type& val);
#ifndef SWIG
        void              push_back(value_type&& val);
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

        AttributeSet& operator=(AttributeSet other) {swap::attributeset(*this, other); return *this;}
      private:
        std::vector<Attribute> set;
        void              assert_not_exists(const value_type& val) const;
        friend void swap::attributeset(class AttributeSet& first, class AttributeSet& second);
    }; /* class AttributeSet */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
#endif /* __cplusplus */

CDM_AttributeValue CDM_CreateAttributeValue(const CDM_AttributeType type, const char *value);
CDM_Attribute      CDM_CreateAttribute(const char *name, const CDM_AttributeValue attribute_value);
CDM_Attribute      CDM_CreateAttribute(const char *name, const char *value, const CDM_AttributeType type
#ifndef   SWIG
                                                                                  = CDM_STRING
#endif /* SWIG */
                                                                                  );
const char        *CDM_GetName(const CDM_Attribute Attribute);
CDM_AttributeValue CDM_GetValue(const CDM_Attribute Attribute);
CDM_AttributeType  CDM_GetValueType(const CDM_Attribute Attribute);
const char        *CDM_GetValueValue(const CDM_Attribute Attribute);
const char        *CDM_GetValueString(const CDM_Attribute Attribute);

CDM_AttributeSet   CDM_CreateAttributeSet();
CDM_AttributeSet   CDM_CreateAttributeSet(const CDM_Attribute Attribute);
CDM_AttributeSet   CDM_CreateAttributeSet(const char *name, const CDM_AttributeValue attribute_value);
CDM_AttributeSet   CDM_CreateAttributeSet(const char *name, const char *value, const CDM_AttributeType type
#ifndef   SWIG
                                                                                  = CDM_STRING
#endif /* SWIG */
                                                                                  );
CDM_Status         CDM_AddAttribute(CDM_AttributeSet Attributes, const CDM_Attribute Attribute);
CDM_Status         CDM_AddAttribute(CDM_AttributeSet Attributes, const char *name, const CDM_AttributeValue value);

#endif /* ELLOGON_CDM_Attribute */

/*
 * vim: nowrap columns=120
 */
