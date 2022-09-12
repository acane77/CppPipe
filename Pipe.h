#ifndef _ACANE_CPPPIPE_H_
#define _ACANE_CPPPIPE_H_

#include <functional>
#include <type_traits>


#define USING_STD_MOVE 0

#define AC_STD_BIND_TYPE std::_Bind

#if USING_STD_MOVE == 1
#define AC_STD_MOVE      std::move
#else
#define AC_STD_MOVE
#endif

#define DEFINE_FUNC_PIPE(name, func) \
    template <class... Args>\
    auto name(Args... args) {\
        return std::bind([]<class... ArgsC> (ArgsC... x) {\
            return func(x...);\
        }, std::placeholders::_1, args...);\
    }

#define PIPE_OP_FUNC_NAME(name) _cb_pipe_op_##name##_func
#define PIPE_GET_PARAM(i)  std::get<i>(param)
#define PIPE_BINARY_OP(op) (PIPE_GET_PARAM(0) op PIPE_GET_PARAM(1))
#define PIPE_UNARY_OP(op) (op PIPE_GET_PARAM(0))

#define DEFINE_OP_CB_FUNC(name, stmt) \
    namespace internal {                             \
    template <class ...Args>     \
    inline auto PIPE_OP_FUNC_NAME(name) (Args... args) { \
        std::tuple param = std::forward_as_tuple(args...);  \
        return AC_STD_MOVE(stmt);\
    }                                 \
    }

#define DEFINE_OP_PIPE(name, expr) \
    DEFINE_OP_CB_FUNC(name, expr)  \
    DEFINE_FUNC_PIPE(name, internal:: PIPE_OP_FUNC_NAME(name))

#define DEFINE_BINARY_OP_PIPE(name, op) \
    DEFINE_OP_PIPE(name, PIPE_BINARY_OP(op))

#define DEFINE_UNARY_OP_PIPE(name, op) \
    DEFINE_OP_PIPE(name, PIPE_UNARY_OP(op))

template <class T, class... Args>
auto operator| (T lhs, AC_STD_BIND_TYPE<Args...> fn) {
    return AC_STD_MOVE(fn(lhs));
}

namespace Acane::Pipe {
    template <class FuncTy, class... Args>
    auto Bind(FuncTy func, Args... args) {
        return std::bind(func, args...);
    }

    template <class FuncTy, class... Args>
    auto Proxy(FuncTy func, Args... args) {
        return std::bind(func, std::placeholders::_1, args...);
    }

#define __BINARY_OP__ DEFINE_BINARY_OP_PIPE
#define __UNARY_OP__  DEFINE_UNARY_OP_PIPE
#define __DEFINE_OP__ DEFINE_OP_PIPE
#define __DEFINE_FUNC__ DEFINE_FUNC_PIPE
    // Arithmetic
    __BINARY_OP__(Add, +)
    __BINARY_OP__(Sub, -)
    __BINARY_OP__(Mul, *)
    __BINARY_OP__(Div, /)
    __BINARY_OP__(Mod, %)
    __UNARY_OP__(Neg, -)
    __UNARY_OP__(Identical, +)
    __UNARY_OP__(Increase, ++)
    __UNARY_OP__(Decrease, --)

    // Comparison operators/relational operators
    __BINARY_OP__(Eq, ==)
    __BINARY_OP__(Ne, !=)
    __BINARY_OP__(Gt, >)
    __BINARY_OP__(Lt, <)
    __BINARY_OP__(Gte, >=)
    __BINARY_OP__(Lte, <=)

    // Logical operators
    __BINARY_OP__(And, &&)
    __BINARY_OP__(Or, ||)
    __UNARY_OP__(Not, !)
    __UNARY_OP__(LogicalNeg, !)

    // Bitwise operators
    __BINARY_OP__(Lshift, <<)
    __BINARY_OP__(Rshift, >>)
    __UNARY_OP__(BitNot, ~)
    __BINARY_OP__(BitOr, |)
    __BINARY_OP__(BitAnd, &)
    __BINARY_OP__(Xor, ^)
    __BINARY_OP__(BitXor, ^)

    // Member
    __DEFINE_OP__(Get, PIPE_GET_PARAM(0)[PIPE_GET_PARAM(1)])

    // Others
    __DEFINE_FUNC__(Call, ([]<class _Fn, class... Rest>(_Fn fn, Rest... rest) {
                              return AC_STD_MOVE(fn(rest...));
                          }))
    __DEFINE_FUNC__(Hash, std::hash)

    // Strings
    __BINARY_OP__(Append, +)
    DEFINE_FUNC_PIPE(ToString, std::to_string)

    // Containers
    __DEFINE_OP__(GetSize, PIPE_GET_PARAM(0).size())
    __DEFINE_OP__(Find, std::find(PIPE_GET_PARAM(0).begin(), PIPE_GET_PARAM(0).end(), PIPE_GET_PARAM(1)))

#undef __BINARY_OP__
#undef __UNARY_OP__
#undef __DEFINE_OP__
#undef __DEFINE_FUNC__

}

#endif // _ACANE_CPPPIPE_H_