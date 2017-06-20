/*****************************************************************************
 *  Expression.cpp:                                                          *
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

#include "CDM3.h"
#include "Expression.h"
#include <boost/foreach.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/assert.hpp>
#include <iostream>
#include <sstream>

namespace ELEP {
  namespace CDM {
    namespace Expression {
      /* AST */
      namespace ast {
        // print functions for debugging
        inline std::ostream& operator<<(std::ostream& out, nil)
        {
            out << "nil";
            return out;
        }

        //////////////////////////////////////////////////////////////////////
        // Implementation
        //////////////////////////////////////////////////////////////////////
        template <typename ErrorHandler>
        inline interpreter::interpreter(ErrorHandler const& error_handler)
            : error_handler(
                [&](x3::position_tagged pos, std::string const& msg)
                { error_handler(pos, msg); }
            )
        {}

        namespace detail
        {
            std::size_t const max_arity = 50;

            template <typename T>
            struct arity : arity<decltype(&T::operator())> {};

            template <typename F, typename RT, typename ...Args>
            struct arity<RT(F::*)(Args...) const>
            {
                enum { value = sizeof...(Args) - 1};
            };

            template <typename T, typename F>
            struct adapter_function
            {
                adapter_function(F f)
                    : f(f) {}

                template <std::size_t... I>
                Value dispatch(Annotation const* ann, T* args, std::index_sequence<I...>) const
                {
                    return f(ann, args[I]...);
                }

                Value operator()(Annotation const* ann, T* args) const
                {
                    return dispatch(ann, args, std::make_index_sequence<detail::arity<F>::value>());
                }

                F f;
            };
        }

        // INTERPRETER_ADD_FUNCTION_VISIT_BEGIN
        template <typename T = Value, typename F>
        inline void interpreter::add_function(std::string name, F f)
        {
            static_assert(detail::arity<F>::value <= detail::max_arity,
                "Function F has too many arguments (maximum == 50).");

            std::function<Value(Annotation const* ann, T* args)>
                 f_adapter = detail::adapter_function<T,F>(f);
            add_function(name, f_adapter, detail::arity<F>::value);
        }
        // INTERPRETER_ADD_FUNCTION_VISIT_END

      }; /* namespace ELEP::CDM::Expression::ast */

      /* Parser */
      namespace parser {
        ////////////////////////////////////////////////////////////////////////
        //  Our error handler
        ////////////////////////////////////////////////////////////////////////
        // ERROR_HANDLER1_VISIT_BEGIN
        // X3 Error Handler Utility
        // template <typename Iterator>
        // using error_handler = x3::error_handler<Iterator>;

        // tag used to get our error handler from the context
        // using x3::error_handler_tag;

        // struct error_handler_base
        // {
        //     error_handler_base();

        //     template <typename Iterator, typename Exception, typename Context>
        //     x3::error_handler_result on_error(
        //         Iterator& first, Iterator const& last
        //       , Exception const& x, Context const& context);

        //     std::map<std::string, std::string> id_map;
        // };
        // ERROR_HANDLER1_VISIT_END

        ////////////////////////////////////////////////////////////////////////
        // Implementation
        ////////////////////////////////////////////////////////////////////////

        // ERROR_HANDLER2_VISIT_BEGIN
        inline error_handler_base::error_handler_base()
        {
            id_map["expression"] = "Expression";
            id_map["additive_expr"] = "Expression";
            id_map["multiplicative_expr"] = "Expression";
            id_map["unary_expr"] = "Expression";
            id_map["primary_expr"] = "Expression";
            id_map["argument_list"] = "Argument List";
        }
        // ERROR_HANDLER2_VISIT_END

        // ERROR_HANDLER3_VISIT_BEGIN
        template <typename Iterator, typename Exception, typename Context>
        inline x3::error_handler_result
        error_handler_base::on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context)
        {
            std::string which = x.which();
            auto iter = id_map.find(which);
            if (iter != id_map.end())
                which = iter->second;

            std::string message = "Error! Expecting: " + which + " here:";
            auto& error_handler = x3::get<error_handler_tag>(context).get();
            error_handler(x.where(), message);
            return x3::error_handler_result::fail;
        }
        // ERROR_HANDLER3_VISIT_END

        // COMMON_VISIT_BEGIN
        using x3::raw;
        using x3::lexeme;
        using x3::alpha;
        using x3::alnum;
        using x3::double_;
        using x3::long_long;
        using x3::ulong_long;
        using x3::char_;
        using x3::string;
        using x3::repeat;
        using x3::no_case;
        using namespace x3::ascii;

        struct identifier_class;
        typedef
            x3::rule<identifier_class, std::string>
        identifier_type;
        identifier_type const identifier = "identifier";

        auto const identifier_def
            = raw[lexeme[(alpha | '_') >> *(alnum | '_')]];

        BOOST_SPIRIT_DEFINE(identifier);
        // COMMON_VISIT_END

        // EXPRESSION_VISIT_BEGIN
        struct expression_class;
        typedef
            x3::rule<expression_class, ast::expression>
        expression_type;
        BOOST_SPIRIT_DECLARE(expression_type);
        // EXPRESSION_VISIT_END

        // EXPRESSION_DEF1_VISIT_BEGIN
        struct additive_expr_class;
        struct multiplicative_expr_class;
        struct unary_expr_class;
        struct primary_expr_class;
        struct argument_list_class;
        struct function_call_class;

        struct quoted_string_class;
        struct comparison_expr_class;
        struct and_expr_class;
        struct or_expr_class;
        // EXPRESSION_DEF1_VISIT_END

        // EXPRESSION_DEF2_VISIT_BEGIN
        typedef x3::rule<additive_expr_class, ast::expression>
        additive_expr_type;

        typedef
            x3::rule<multiplicative_expr_class, ast::expression>
        multiplicative_expr_type;

        typedef
            x3::rule<unary_expr_class, ast::operand>
        unary_expr_type;

        typedef
            x3::rule<primary_expr_class, ast::operand>
        primary_expr_type;

        typedef
            x3::rule<argument_list_class, std::list<ast::expression>>
        argument_list_type;

        typedef
            x3::rule<function_call_class, ast::function_call>
        function_call_type;

        typedef x3::rule<quoted_string_class, ast::quoted_string>
                quoted_string_type;
        typedef x3::rule<comparison_expr_class, ast::expression>
                comparison_expr_type;
        typedef x3::rule<and_expr_class, ast::expression> and_expr_type;
        typedef x3::rule<or_expr_class, ast::expression> or_expr_type;
        // EXPRESSION_DEF2_VISIT_END

        // EXPRESSION_DEF3_VISIT_BEGIN
        expression_type const
            expression = "expression";

        additive_expr_type const
            additive_expr = "additive_expr";

        multiplicative_expr_type const
            multiplicative_expr = "multiplicative_expr";

        unary_expr_type const
            unary_expr = "unary_expr";

        primary_expr_type const
            primary_expr = "primary_expr";

        argument_list_type const
            argument_list = "argument_list";

        function_call_type const
            function_call = "function_call";

        quoted_string_type   const quoted_string   = "quoted_string";
        comparison_expr_type const comparison_expr = "comparison_expr";
        and_expr_type        const and_expr        = "and_expr";
        or_expr_type         const or_expr         = "or_expr";
        // EXPRESSION_DEF3_VISIT_END

        // EXPRESSION_DEF4_VISIT_BEGIN
        auto const additive_expr_def =
            multiplicative_expr
            >> *(   (string("+") > multiplicative_expr)
                |   (string("-") > multiplicative_expr)
                )
            ;

        auto const multiplicative_expr_def =
            unary_expr
            >> *(   (string("*") > unary_expr)
                |   (string("/") > unary_expr)
                )
            ;

        auto const unary_expr_def =
                primary_expr
            |   (string("-")   > primary_expr)
            |   (string("+")   > primary_expr)
            |   (string("!")   > primary_expr)
            ;
        // EXPRESSION_DEF4_VISIT_END

        // EXPRESSION_DEF5_VISIT_BEGIN
        auto argument_list_def = expression % ',';

        auto function_call_def =
                identifier
            >> -('(' > argument_list > ')')
            ;

        auto const quoted_string_def =
            '"' >> lexeme[*(('\\' >> char_) | (char_ - '"'))] >> '"';

        auto const primary_expr_def =
                DEFAULT_VALUE_PARSER
            |   function_call
            |   quoted_string
            |   '(' > expression > ')'
            ;

        auto const comparison_expr_def =
          additive_expr
          >> *( (string("==") > additive_expr)
              | (string("!=") > additive_expr)
              | (string("<=") > additive_expr)
              | (string(">=") > additive_expr)
              | (string("=<") > additive_expr)
              | (string("=>") > additive_expr)
              | (string("=")  > additive_expr)
              | (string("<")  > additive_expr)
              | (string(">")  > additive_expr)
              );

        auto const and_expr_def =
          comparison_expr
          >> *( (string("and") > comparison_expr)
              | (string("&&")  > comparison_expr)
              );
        auto const or_expr_def =
          and_expr
          >> *( (string("or") > and_expr)
              | (string("||") > and_expr)
              );

        // auto const expression_def = additive_expr;
        auto const expression_def = or_expr;
        // EXPRESSION_DEF5_VISIT_END

        // EXPRESSION_DEF6_VISIT_BEGIN
        BOOST_SPIRIT_DEFINE(
            expression
          , or_expr
          , and_expr
          , comparison_expr
          , additive_expr
          , multiplicative_expr
          , unary_expr
          , primary_expr
          , argument_list
          , function_call
          , quoted_string
        );
        // EXPRESSION_DEF6_VISIT_END

        // EXPRESSION_DEF7_VISIT_BEGIN
        struct unary_expr_class    : x3::annotate_on_success {};
        struct primary_expr_class  : x3::annotate_on_success {};
        struct function_call_class : x3::annotate_on_success {};
        struct quoted_string_class : x3::annotate_on_success {};
        struct expression_class    : x3::annotate_on_success,
                                     error_handler_base {};
        // EXPRESSION_DEF7_VISIT_END

        // CONFIG_VISIT_BEGIN
        // Our Iterator Type
        // typedef std::string::const_iterator iterator_type;

        // The Phrase Parse Context
        typedef
            x3::phrase_parse_context<x3::ascii::space_type>::type
        phrase_context_type;

        // Our Error Handler
        typedef error_handler<iterator_type> error_handler_type;

        // Combined Error Handler and Phrase Parse Context
        typedef x3::with_context<
            error_handler_tag
          , std::reference_wrapper<error_handler_type> const
          , phrase_context_type>::type
        context_type;
        // CONFIG_VISIT_END

      }; /* namespace ELEP::CDM::Expression::parser */

      //parser::expression_type const& expression();
      parser::expression_type const& expression()
      {
          return parser::expression;
      }

    }; /* namespace ELEP::CDM::Expression */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */

// AST_ADAPTED_VISIT_BEGIN
BOOST_FUSION_ADAPT_STRUCT(
    ELEP::CDM::Expression::ast::signed_,
    (std::string, sign)
    (ELEP::CDM::Expression::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    ELEP::CDM::Expression::ast::operation,
    (std::string, operator_)
    (ELEP::CDM::Expression::ast::operand, operand_)
)

BOOST_FUSION_ADAPT_STRUCT(
    ELEP::CDM::Expression::ast::expression,
    (ELEP::CDM::Expression::ast::operand, first)
    (std::list<ELEP::CDM::Expression::ast::operation>, rest)
)

BOOST_FUSION_ADAPT_STRUCT(
    ELEP::CDM::Expression::ast::function_call,
    (std::string, name)
    (std::list<ELEP::CDM::Expression::ast::expression>, arguments)
)

BOOST_FUSION_ADAPT_STRUCT(
    ELEP::CDM::Expression::ast::quoted_string,
    (std::string, value)
)
// AST_ADAPTED_VISIT_END

namespace ELEP {
  namespace CDM {
    namespace Expression {
      namespace parser {
        BOOST_SPIRIT_INSTANTIATE(expression_type, iterator_type, context_type);
      }; /* namespace ELEP::CDM::Expression::parser */

      namespace ast { namespace {
        typedef interpreter::error_handler_type error_handler_type;

        typedef std::map< std::string , std::pair<ast::f_Value, std::size_t> > fmap_type;
        typedef std::map< std::string , std::pair<ast::f_string, std::size_t> > fmap_s_type;

        struct interpreter_impl
        {
            typedef Value result_type;

            interpreter_impl(fmap_type const& fmap,
                             fmap_s_type const& fmap_s,
                             error_handler_type const& error_handler)
                : fmap(fmap)
                , fmap_s(fmap_s)
                , error_handler(error_handler)
                , ann(nullptr), str_value(nullptr)
            {}

            interpreter_impl(Annotation const* annotation,
                             fmap_type const& fmap,
                             fmap_s_type const& fmap_s,
                             error_handler_type const& error_handler)
                : fmap(fmap)
                , fmap_s(fmap_s)
                , error_handler(error_handler)
                , ann(annotation), str_value(nullptr)
            {}

            Value operator()(ast::nil) const { BOOST_ASSERT(0); }
            Value operator()(Value ast) const;
            Value operator()(Value lhs, ast::operation const& ast) const;
            Value operator()(ast::signed_ const& ast) const;
            Value operator()(ast::expression const& ast) const;
            Value operator()(ast::function_call const& ast) const;
            Value operator()(ast::quoted_string const& ast) const;

            fmap_type const& fmap;
            fmap_s_type const& fmap_s;
            error_handler_type const& error_handler;
            Annotation const* ann;
            mutable std::string const *str_value;
        };

        Value interpreter_impl::operator()(Value ast) const
        {
            // std::cout << "operator: Value\n";
            if (str_value) str_value = nullptr;
            return ast;
        }

        // INTERPRETER_OPERATION_VISIT_BEGIN
        Value interpreter_impl::operator()(Value lhs, ast::operation const& ast) const
        {
            // std::cout << "operator: operation " << ast.operator_ << " (" << lhs << ")\n";
            if (str_value) str_value = nullptr;
            Value rhs = boost::apply_visitor(*this, ast.operand_);
            if (ast.operator_ == "+") {
              return lhs + rhs;
            } else if (ast.operator_ == "-") {
              return lhs - rhs;
            } else if (ast.operator_ == "*") {
              return lhs * rhs;
            } else if (ast.operator_ == "/") {
              return lhs / rhs;
            } else if (ast.operator_ == "==") {
              return lhs == rhs;
            } else if (ast.operator_ == "!=") {
              return lhs != rhs;
            } else if (ast.operator_ == "<=") {
              return lhs <= rhs;
            } else if (ast.operator_ == ">=") {
              return lhs >= rhs;
            } else if (ast.operator_ == "=<") {
              return lhs <= rhs;
            } else if (ast.operator_ == "=>") {
              return lhs >= rhs;
            } else if (ast.operator_ == "=") {
              return lhs == rhs;
            } else if (ast.operator_ == "<") {
              return lhs < rhs;
            } else if (ast.operator_ == ">") {
              return lhs > rhs;
            } else if (ast.operator_ == "||" || ast.operator_ == "or" ) {
              return lhs || rhs;
            } else if (ast.operator_ == "&&" || ast.operator_ == "and") {
              return lhs && rhs;
            } else {
              throw std::runtime_error("unknown operator \"" +
                                        ast.operator_ + "\"");
              BOOST_ASSERT(0);
              return -1;
            }
        }
        // INTERPRETER_OPERATION_VISIT_END

        // INTERPRETER_SIGNED_VISIT_BEGIN
        Value interpreter_impl::operator()(ast::signed_ const& ast) const
        {
            // std::cout << "operator: signed\n";
            if (str_value) str_value = nullptr;
            Value r = boost::apply_visitor(*this, ast.operand_);
            if (ast.sign == "-") {
              return -r;
            } else if (ast.sign == "+") {
              return r;
            } else if (ast.sign == "!" || ast.sign == "not") {
              return !r;
            } else {
              BOOST_ASSERT(0);
              return -1;
            }
        }
        // INTERPRETER_SIGNED_VISIT_END

        Value interpreter_impl::operator()(ast::expression const& ast) const
        {
            // std::cout << "operator: expression\n";
            if (str_value) str_value = nullptr;
            Value r = boost::apply_visitor(*this, ast.first);
            for (auto const& oper : ast.rest)
                r = (*this)(r, oper);
             return r;
        }

        // INTERPRETER_FUNCTION_CALL_VISIT_BEGIN
        Value interpreter_impl::operator()(ast::function_call const& ast) const
        {
            // std::cout << "operator: function_call\n";
            if (str_value) str_value = nullptr;
            // std::string name_l(ast.name);
            // std::transform(name_l.begin(), name_l.end(), name_l.begin(), ::tolower);
            auto iter = fmap_s.find(ast.name);
            if (iter != fmap_s.end()) {
                if (ann == nullptr) throw std::runtime_error("functor without annotation");
                if (iter->second.second != ast.arguments.size()) {
                    std::stringstream out;
                    out << "Wrong number of arguments to function " << ast.name << " ("
                        << iter->second.second << " expected)." << std::endl;

                    throw std::runtime_error(out.str());
                    // error_handler(ast, out.str());
                    return -1;
                }
                // Get the args
                std::string argument_array[ast.arguments.size() + 1];
                std::string *ap = argument_array;
                f_string_t args[ast.arguments.size() + 1];
                f_string_t* p = args;
                for (auto const arg : ast.arguments) {
                    // Evaluate the expression, and check if result is a string...
                    Value result = (*this)(arg);
                    if (str_value == nullptr) {
                      std::stringstream out;
                      out << "function \"" << ast.name << "\" expects " << ast.arguments.size()
                          << " argument(s) of type string; instead got a number (" << result << ")";
                      throw std::runtime_error(out.str());
                    }
                    *ap = *str_value; str_value = nullptr;
                    *p++ = ap; ap++;
                }

                // call user function
                return iter->second.first(ann, args);
            }
            // Maybe a Value function?
            auto iterf = fmap.find(ast.name);
            if (iterf != fmap.end()) {
                if (iterf->second.second != ast.arguments.size()) {
                    std::stringstream out;
                    out << "wrong number of arguments to function \"" << ast.name << " ("
                        << iterf->second.second << " expected)" << std::endl;

                    throw std::runtime_error(out.str());
                    // error_handler(ast, out.str());
                    return -1;
                }
                // Get the args
                Value args[ast.arguments.size() + 1];
                Value* p = args;
                for (auto const& arg : ast.arguments) {
                    *p++ = (*this)(arg);
                    if (str_value) {
                      std::stringstream out;
                      out << "unexpected argument of type string for function \"" << ast.name
                          << "\": \"" << *str_value << '"';
                      throw std::runtime_error(out.str());
                    }
                }

                // call user function
                return iterf->second.first(ann, args);
            }
            throw std::runtime_error("undefined function: " + ast.name);
            // error_handler(ast, "Undefined function " + ast.name + '.');
            return -1;
        }
        Value interpreter_impl::operator()(ast::quoted_string const& ast) const
        {
            // std::cout << "operator: quoted_string\n";
            str_value = &ast.value;
            return 0;
        }
        // INTERPRETER_FUNCTION_CALL_VISIT_END
        }
        Value interpreter::eval(ast::expression const& ast)
        {
            return interpreter_impl(fmap, fmap_s, error_handler)(ast);
        }
        Value interpreter::eval(Annotation const* ann,
                                 ast::expression const& ast)
        {
            return interpreter_impl(ann, fmap, fmap_s, error_handler)(ast);
        }
      }; /* namespace ELEP::CDM::Expression::ast */

      namespace ast { namespace {
        //////////////////////////////////////////////////////////////////////
        //  The AST printer
        //////////////////////////////////////////////////////////////////////
        // AST_PRINTER1_VISIT_BEGIN
        struct printer
        {
            typedef void result_type;

            printer(std::ostream& out)
                : out(out)
            {}

            void operator()(ast::nil) const { BOOST_ASSERT(0); }
            void operator()(Value ast) const;
            void operator()(ast::operation const& ast) const;
            void operator()(ast::signed_ const& ast) const;
            void operator()(ast::expression const& ast) const;
            void operator()(ast::function_call const& ast) const;
            void operator()(ast::quoted_string const& ast) const;

            std::ostream& out;
        };
        // AST_PRINTER1_VISIT_END

        // AST_PRINTER2_VISIT_BEGIN
        void printer::operator()(Value ast) const
        {
            out << ast;
        }

        void printer::operator()(ast::operation const& ast) const
        {
            out << ' ' << ast.operator_ << ' ';
#if 0
            if (ast.operator_ == "+") {
              out << " + ";
            } else if (ast.operator_ == "-") {
              out << " - ";
            } else if (ast.operator_ == "*") {
              out << " * ";
            } else if (ast.operator_ == "/") {
              out << " / ";
            } else {
              throw std::runtime_error("unknown operator: " + ast.operator_);
              BOOST_ASSERT(0);
              return;
            }
#endif
            boost::apply_visitor(*this, ast.operand_);
        }
        // AST_PRINTER2_VISIT_END

        void printer::operator()(ast::signed_ const& ast) const
        {
            if (ast.sign == "-") {
              out << "- ";
            } else if (ast.sign == "+") {
            } else if (ast.sign == "!" || ast.sign == "not") {
              out << "! ";
            } else {
              throw std::runtime_error("unknown unary operator: " + ast.sign);
              BOOST_ASSERT(0);
              return;
            }
            boost::apply_visitor(*this, ast.operand_);
        }

        // AST_PRINTER3_VISIT_BEGIN
        void printer::operator()(ast::expression const& ast) const
        {
            if (ast.rest.size())
                out << '(';
            boost::apply_visitor(*this, ast.first);
            for (auto const& oper : ast.rest)
                (*this)(oper);
            if (ast.rest.size())
                out << ')';
        }
        // AST_PRINTER3_VISIT_END

        // AST_PRINTER4_VISIT_BEGIN
        void printer::operator()(ast::function_call const& ast) const
        {
            out << ast.name;
            if (ast.arguments.size())
                out << '(';
            bool first = true;
            for (auto const& arg : ast.arguments)
            {
                if (first)
                    first = false;
                else
                    out << ", ";
                (*this)(arg);
            }
            if (ast.arguments.size())
                out << ')';
        }

        void printer::operator()(ast::quoted_string const& ast) const {
          out << ast.value;
        }

        // AST_PRINTER4_VISIT_END
        }
        void print(std::ostream& out, ast::expression const& ast)
        {
            printer p(out);
            p(ast);
            out << std::endl;
        }
      }; /* namespace ELEP::CDM::Expression::ast */


      bool parse_expression(const std::string& expr,
                            ast::expression& ast) {
        using boost::spirit::x3::with;
        using ELEP::CDM::Expression::parser::error_handler_type;
        std::stringstream out;
        parser::iterator_type iter(expr.begin()), end(expr.end());
        error_handler_type error_handler(iter, end, out); // Our error handler
        // Our parser
        auto const parser =
          // we pass our error handler to the parser so we can access
          // it later on in our on_error and on_sucess handlers
          with<parser::error_handler_tag>(std::ref(error_handler))
          [
              ELEP::CDM::Expression::expression()
          ];
        using boost::spirit::x3::ascii::space;
        bool success = phrase_parse(iter, end, parser, space, ast);
        if (!success)    throw std::runtime_error("parse error: " + out.str());
        if (iter != end) {
          throw std::runtime_error("expecting end of input here: " +
                                   std::string(iter, end));
        }
        return success;
      }; /* parse_expression */

      ast::interpreter get_interpreter(const std::string& expr,
                                       ast::expression& ast) {
        parse_expression(expr, ast);
        using ELEP::CDM::Expression::parser::error_handler_type;
        std::stringstream out;
        parser::iterator_type iter(expr.begin()), end(expr.end());
        error_handler_type error_handler(iter, end, out); // Our error handler
        ast::interpreter interp(error_handler);
        interp.add_function("not",   [](Annotation const* ann, Value x){ return !x; });
        interp.add_function("true",  [](Annotation const* ann){ return 1; });
        interp.add_function("false", [](Annotation const* ann){ return 0; });
        interp.add_function("ContainsPosition",   [](Annotation const* ann, Value x){
          if (ann) return ann->contains(x); else return false; });
        // Functors...
        interp.add_function<ast::f_string_t>("HasAttribute", [](Annotation const* ann, ast::f_string_t x){
          if (ann && x) return ann->exists(*x); else return false;
        });
        interp.add_function<ast::f_string_t>("HasAttributeMatchingValue",
          [](Annotation const* ann, ast::f_string_t name, ast::f_string_t pattern){
          if (ann && name && pattern) return ann->containsAttributeMatchingValue(*name, *pattern);
          else return false;
        });
        return interp;
      }; /* get_interpreter */

      // void interpreter_add_function(ast::interpreter& interp,
      //      std::string name, const std::function<Value()>& f) {
      //   interp.add_function(name, f);
      // }; /* interpreter_add_function */

    }; /* namespace ELEP::CDM::Expression */
  }; /* namespace ELEP::CDM */
}; /* namespace ELEP */

