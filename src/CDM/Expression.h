/*****************************************************************************
 *  Expression.h:                                                            *
 * ----------------------                                                    *
 *  This file defines the Expression section of the                          *
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

/*=============================================================================
  The parser in this file is based on spirit x3 fun:

  Copyright (c) 2001-2015 Joel de Guzman

  Distributed under the Boost Software License, Version 1.0. (See accompanying
  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifndef ELLOGON_CDM_Expression
#define ELLOGON_CDM_Expression

#ifdef __cplusplus

#ifndef SWIG
#include <string>
#include <list>
#include <map>
#include <functional>
#include <utility>
#include <ostream>
#include <boost/mpl/int.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>

#include <boost/fusion/include/io.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#endif /* SWIG */

namespace ELEP {
  namespace CDM {
    namespace Expression {
      namespace x3 = boost::spirit::x3;

      typedef Position Value;
#define DEFAULT_VALUE_PARSER  x3::ulong_long

      namespace parser {
        // Our Iterator Type
        typedef std::string::const_iterator iterator_type;
        // Our Error Handler
        // X3 Error Handler Utility
        template <typename Iterator>
        using error_handler = x3::error_handler<Iterator>;
        typedef error_handler<iterator_type> error_handler_type;
        // tag used to get our error handler from the context
        using x3::error_handler_tag;

        struct error_handler_base
        {
            error_handler_base();

            template <typename Iterator, typename Exception, typename Context>
            x3::error_handler_result on_error(
                Iterator& first, Iterator const& last
              , Exception const& x, Context const& context);

            std::map<std::string, std::string> id_map;
        };

        struct identifier_class;
        struct expression_class;
        struct additive_expr_class;
        struct multiplicative_expr_class;
        struct unary_expr_class;
        struct primary_expr_class;
        struct argument_list_class;
        struct function_call_class;
      }; /* namespace ELEP::CDM::Expression::parser */

      namespace ast {
        // AST1_VISIT_BEGIN
        struct nil {};
        struct signed_;
        struct expression;
        struct function_call;
        struct quoted_string;

        struct operand :
            x3::variant<
                nil
              , Value
              , x3::forward_ast<signed_>
              , x3::forward_ast<expression>
              , x3::forward_ast<function_call>
              , x3::forward_ast<quoted_string>
            >
        {
            using base_type::base_type;
            using base_type::operator=;
        };
        // AST1_VISIT_END

        // AST2_VISIT_BEGIN
        struct quoted_string : x3::position_tagged
        {
            std::string value;
        };

        struct signed_
        {
            std::string sign;
            operand operand_;
        };

        struct operation : x3::position_tagged
        {
            std::string operator_;
            operand operand_;
        };

        struct expression : x3::position_tagged
        {
            operand first;
            std::list<operation> rest;
        };

        struct function_call : x3::position_tagged
        {
            std::string name;
            std::list<expression> arguments;
        };
        // AST2_VISIT_END

        // INTERPRETER_CLASS_VISIT_BEGIN
        typedef std::string const * f_string_t;
        typedef std::function<Value(ELEP::CDM::Annotation const* ann, Value* args)>     f_Value;
        typedef std::function<Value(ELEP::CDM::Annotation const* ann, f_string_t* args)> f_string;
        class interpreter
        {
        public:

            typedef std::function<
                void(x3::position_tagged, std::string const&)>
            error_handler_type;


            template <typename ErrorHandler>
            interpreter(ErrorHandler const& error_handler);

            template <typename T = Value, typename F>
            void add_function(std::string name, F f);

            Value eval(ast::expression const& ast);
            Value eval(Annotation const* ann, ast::expression const& ast);

        private:

            void add_function(const std::string& name, f_Value f, std::size_t arity)  {fmap[name]   = std::make_pair(f, arity);}
            void add_function(const std::string& name, f_string f, std::size_t arity) {fmap_s[name] = std::make_pair(f, arity);};
            std::map< std::string , std::pair<f_Value, std::size_t> > fmap;
            std::map< std::string , std::pair<f_string, std::size_t> > fmap_s;

            error_handler_type error_handler;
        };
        // INTERPRETER_CLASS_VISIT_END

        extern void print(std::ostream& out, ast::expression const& ast);

      }; /* namespace ELEP::CDM::Expression::ast */

      namespace parser {
         typedef x3::rule<expression_class, ast::expression> expression_type;
      }; /* namespace ELEP::CDM::Expression::parser */

      typedef ast::expression AST;
      typedef ast::interpreter interpreter;

      extern parser::expression_type const& expression();

      extern bool parse_expression(const std::string& expr, AST& ast);
      extern interpreter get_interpreter(const std::string& expr, AST& ast);
      // extern void interpreter_add_function(interpreter& interp,
      //      std::string name, const std::function<Value()>& f);

    }; /* namespace ELEP::CDM::Expression */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */
//#include "ExpressionGrammar.h"

#endif /* __cplusplus */

#endif /* ELLOGON_CDM_Expression */

/*
 * vim: nowrap columns=120
 */
