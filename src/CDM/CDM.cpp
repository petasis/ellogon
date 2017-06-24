/*****************************************************************************
 *  CDM.cpp:                                                                 *
 * ----------------------                                                    *
 *  This file implements the Utitlities section of the                       *
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
#include <map>
#include <iostream>
#include <sstream>
#include <mutex>
#include <algorithm>
#include <boost/locale.hpp>     /* For normalising unicode strings */

namespace ELEP {
  namespace CDM {
    namespace Cache {

      std::shared_ptr<std::string>
      get_string(const std::string &str, const bool clear) {
        static std::map<std::string, std::weak_ptr<std::string>> cache;
        static std::mutex m;

        // std::cout << "get_string: '" << str << "', size: " << cache.size()
        //           << ", clear: " << clear << std::endl << std::flush;
        std::lock_guard<std::mutex> hold(m);
        auto sp = cache[str].lock();
        // if (!clear) std::cout << (sp?"OLD":"NEW") << ", size: "
        //                       << cache.size() << std::endl << std::flush;
        if (clear) cache.erase(str);
        else if (!sp) {
          /* Create a new string, to add to the cache */
          cache[str] = sp = std::shared_ptr<std::string>(new std::string(str),
                                                         [](std::string* str){
            ELEP::CDM::Cache::get_string(*str,true);
            delete str;
          });
        }
        return sp;
      }; /* ELEP::CDM::get_string */

#if 0
      std::shared_ptr<std::string>
      get_string(std::string &&str, const bool clear) {
        static std::map<std::string, std::weak_ptr<std::string>> cache;
        static std::mutex m;

        std::lock_guard<std::mutex> hold(m);
        auto sp = cache[str].lock();
        if(clear) cache.erase(str);
        else if (!sp) {
          /* Create a new string, to add to the cache */
          cache[str] = sp = std::shared_ptr<std::string>(new std::string(str),
                                                         [](std::string* str){
            ELEP::CDM::Cache::get_string(*str,true);
            delete str;
          });
        }
        return sp;
      }; /* ELEP::CDM::get_string */
#endif

      // std::shared_ptr<std::string>
      // get_string(const char *str, const bool clear) {
      //   return get_string(std::string(str), clear);
      // }; /* ELEP::CDM::get_string */

    }; /* namespace ELEP::CDM::Cache */

    namespace Utilities {
      std::string ensure_list_element(const std::string &str) {
        std::string tmp(str);
        size_t pos = 0;
        while((pos = tmp.find('{', pos)) != std::string::npos) {
          tmp.insert(pos, 1, '\\'); pos += 2;
        }
        pos = 0;
        while((pos = tmp.find('}', pos)) != std::string::npos) {
          tmp.insert(pos, 1, '\\'); pos += 2;
        }
        return tmp;
        // std::replace_if(str.begin(), str.end(),
        //    [](char c) { return (c == '{' ); }, "\\{");
        // std::replace_if(str.begin(), str.end(),
        //    [](char c) { return (c == '}' ); }, "\\{");
      };

    }; /* namespace ELEP::CDM::Utilities */

    namespace convert {
      const ELEP::CDM::AttributeType AttributeType(const CDM_AttributeType type) {
        switch (type) {
          case CDM_NONE:              return ELEP::CDM::AttributeType::NONE;
          case CDM_STRING:            return ELEP::CDM::AttributeType::STRING;
          case CDM_STRING_SET:        return ELEP::CDM::AttributeType::STRING_SET;
          case CDM_SPAN:              return ELEP::CDM::AttributeType::SPAN;
          case CDM_SPAN_SET:          return ELEP::CDM::AttributeType::SPAN_SET;
          case CDM_ANNOTATION_ID:     return ELEP::CDM::AttributeType::ANNOTATION_ID;
          case CDM_ANNOTATION_ID_SET: return ELEP::CDM::AttributeType::ANNOTATION_ID_SET;
          case CDM_IMAGE:             return ELEP::CDM::AttributeType::IMAGE;
          case CDM_BASE64_IMAGE:      return ELEP::CDM::AttributeType::BASE64_IMAGE;
        }
        return AttributeType::NONE;
      };

      const CDM_AttributeType AttributeType(const ELEP::CDM::AttributeType type) {
        switch (type) {
          case ELEP::CDM::AttributeType::NONE:              return CDM_NONE;
          case ELEP::CDM::AttributeType::STRING:            return CDM_STRING;
          case ELEP::CDM::AttributeType::STRING_SET:        return CDM_STRING_SET;
          case ELEP::CDM::AttributeType::SPAN:              return CDM_SPAN;
          case ELEP::CDM::AttributeType::SPAN_SET:          return CDM_SPAN_SET;
          case ELEP::CDM::AttributeType::ANNOTATION_ID:     return CDM_ANNOTATION_ID;
          case ELEP::CDM::AttributeType::ANNOTATION_ID_SET: return CDM_ANNOTATION_ID_SET;
          case ELEP::CDM::AttributeType::IMAGE:             return CDM_IMAGE;
          case ELEP::CDM::AttributeType::BASE64_IMAGE:      return CDM_BASE64_IMAGE;
        }
        return CDM_NONE;
      };
    }; /* namespace ELEP::CDM:;convert */

    namespace swap {
      void span(Span& first, Span& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first._start, second._start);
        swap(first._end,   second._end);
      };
      void spanset(SpanSet& first, SpanSet& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first.set,  second.set);
        swap(first._min, second._min);
        swap(first._max, second._max);
      };
      void attributevalue(AttributeValue& first, AttributeValue& second) {
        using std::swap;
        swap(first._type,     second._type);
        swap(first._v_string, second._v_string);
      };
      void attribute(Attribute& first, Attribute& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first._type,     second._type);
        swap(first._name,     second._name);
        swap(first._v_string, second._v_string);
      };
      void attributeset(AttributeSet& first, AttributeSet& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first.set,  second.set);
      };
      void annotation(Annotation& first, Annotation& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first._id,         second._id);
        swap(first._type,       second._type);
        swap(first._spans,      second._spans);
        swap(first._attributes, second._attributes);
      };
      void annotationset(AnnotationSet& first, AnnotationSet& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first.set,          second.set);
        swap(first._next_ann_id, second._next_ann_id);
      };
      void document(Document& first, Document& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first._id,          second._id);
        swap(first._external_id, second._external_id);
        swap(first._data,        second._data);
        swap(first._attributes,  second._attributes);
        swap(first._annotations, second._annotations);
        swap(first._collection,  second._collection);
      };
      void collection(Collection& first, Collection& second) {
        // enable ADL (not necessary in our case, but good practice)
        using std::swap;
        swap(first._id,         second._id);
        swap(first._attributes, second._attributes);
        swap(first.set,         second.set);
      };
    }; /* namespace ELEP::CDM::swap */

    namespace Unicode {
      std::string normalise_nfc(const std::string& data,
                                      const char *locale) {
        if (locale == nullptr) locale = "";
        std::locale loc =
           boost::locale::generator().generate(locale);
        return boost::locale::normalize(data, boost::locale::norm_nfc, loc);
      };
    }; /* namespace ELEP::CDM::Unicode */

  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
