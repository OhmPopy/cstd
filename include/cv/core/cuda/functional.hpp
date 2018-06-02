

#ifndef OPENCC_CUDA_FUNCTIONAL_HPP
#define OPENCC_CUDA_FUNCTIONAL_HPP

#include <functional>
#include "saturate_cast.hpp"
#include "vec_traits.hpp"
#include "type_traits.hpp"
#include "device_functions.h"

/** @file
 * @deprecated Use @ref cudev instead.
 */

//! @cond IGNORED

namespace cv { namespace cuda { namespace device
{
    // Function Objects
    template<typename Argument, typename Result> struct unary_function : public std::unary_function<Argument, Result> {};
    template<typename Argument1, typename Argument2, typename Result> struct binary_function : public std::binary_function<Argument1, Argument2, Result> {};

    // Arithmetic Operations
    template <typename T> struct plus : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a + b;
        }
        __host__ __device__ __forceinline__ plus() {}
        __host__ __device__ __forceinline__ plus(const plus&) {}
    };

    template <typename T> struct minus : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a - b;
        }
        __host__ __device__ __forceinline__ minus() {}
        __host__ __device__ __forceinline__ minus(const minus&) {}
    };

    template <typename T> struct multiplies : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a * b;
        }
        __host__ __device__ __forceinline__ multiplies() {}
        __host__ __device__ __forceinline__ multiplies(const multiplies&) {}
    };

    template <typename T> struct divides : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a / b;
        }
        __host__ __device__ __forceinline__ divides() {}
        __host__ __device__ __forceinline__ divides(const divides&) {}
    };

    template <typename T> struct modulus : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a % b;
        }
        __host__ __device__ __forceinline__ modulus() {}
        __host__ __device__ __forceinline__ modulus(const modulus&) {}
    };

    template <typename T> struct negate : unary_function<T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a) const
        {
            return -a;
        }
        __host__ __device__ __forceinline__ negate() {}
        __host__ __device__ __forceinline__ negate(const negate&) {}
    };

    // Comparison Operations
    template <typename T> struct equal_to : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a == b;
        }
        __host__ __device__ __forceinline__ equal_to() {}
        __host__ __device__ __forceinline__ equal_to(const equal_to&) {}
    };

    template <typename T> struct not_equal_to : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a != b;
        }
        __host__ __device__ __forceinline__ not_equal_to() {}
        __host__ __device__ __forceinline__ not_equal_to(const not_equal_to&) {}
    };

    template <typename T> struct greater : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a > b;
        }
        __host__ __device__ __forceinline__ greater() {}
        __host__ __device__ __forceinline__ greater(const greater&) {}
    };

    template <typename T> struct less : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a < b;
        }
        __host__ __device__ __forceinline__ less() {}
        __host__ __device__ __forceinline__ less(const less&) {}
    };

    template <typename T> struct greater_equal : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a >= b;
        }
        __host__ __device__ __forceinline__ greater_equal() {}
        __host__ __device__ __forceinline__ greater_equal(const greater_equal&) {}
    };

    template <typename T> struct less_equal : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a <= b;
        }
        __host__ __device__ __forceinline__ less_equal() {}
        __host__ __device__ __forceinline__ less_equal(const less_equal&) {}
    };

    // Logical Operations
    template <typename T> struct logical_and : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a && b;
        }
        __host__ __device__ __forceinline__ logical_and() {}
        __host__ __device__ __forceinline__ logical_and(const logical_and&) {}
    };

    template <typename T> struct logical_or : binary_function<T, T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a,
                                                    typename TypeTraits<T>::ParameterType b) const
        {
            return a || b;
        }
        __host__ __device__ __forceinline__ logical_or() {}
        __host__ __device__ __forceinline__ logical_or(const logical_or&) {}
    };

    template <typename T> struct logical_not : unary_function<T, bool>
    {
        __device__ __forceinline__ bool operator ()(typename TypeTraits<T>::ParameterType a) const
        {
            return !a;
        }
        __host__ __device__ __forceinline__ logical_not() {}
        __host__ __device__ __forceinline__ logical_not(const logical_not&) {}
    };

    // Bitwise Operations
    template <typename T> struct bit_and : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a & b;
        }
        __host__ __device__ __forceinline__ bit_and() {}
        __host__ __device__ __forceinline__ bit_and(const bit_and&) {}
    };

    template <typename T> struct bit_or : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a | b;
        }
        __host__ __device__ __forceinline__ bit_or() {}
        __host__ __device__ __forceinline__ bit_or(const bit_or&) {}
    };

    template <typename T> struct bit_xor : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType a,
                                                 typename TypeTraits<T>::ParameterType b) const
        {
            return a ^ b;
        }
        __host__ __device__ __forceinline__ bit_xor() {}
        __host__ __device__ __forceinline__ bit_xor(const bit_xor&) {}
    };

    template <typename T> struct bit_not : unary_function<T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType v) const
        {
            return ~v;
        }
        __host__ __device__ __forceinline__ bit_not() {}
        __host__ __device__ __forceinline__ bit_not(const bit_not&) {}
    };

    // Generalized Identity Operations
    template <typename T> struct identity : unary_function<T, T>
    {
        __device__ __forceinline__ typename TypeTraits<T>::ParameterType operator()(typename TypeTraits<T>::ParameterType x) const
        {
            return x;
        }
        __host__ __device__ __forceinline__ identity() {}
        __host__ __device__ __forceinline__ identity(const identity&) {}
    };

    template <typename T1, typename T2> struct project1st : binary_function<T1, T2, T1>
    {
        __device__ __forceinline__ typename TypeTraits<T1>::ParameterType operator()(typename TypeTraits<T1>::ParameterType lhs, typename TypeTraits<T2>::ParameterType rhs) const
        {
            return lhs;
        }
        __host__ __device__ __forceinline__ project1st() {}
        __host__ __device__ __forceinline__ project1st(const project1st&) {}
    };

    template <typename T1, typename T2> struct project2nd : binary_function<T1, T2, T2>
    {
        __device__ __forceinline__ typename TypeTraits<T2>::ParameterType operator()(typename TypeTraits<T1>::ParameterType lhs, typename TypeTraits<T2>::ParameterType rhs) const
        {
            return rhs;
        }
        __host__ __device__ __forceinline__ project2nd() {}
        __host__ __device__ __forceinline__ project2nd(const project2nd&) {}
    };

    // Min/Max Operations

#define OPENCC_CUDA_IMPLEMENT_MINMAX(name, type, op) \
    template <> struct name<type> : binary_function<type, type, type> \
    { \
        __device__ __forceinline__ type operator()(type lhs, type rhs) const {return op(lhs, rhs);} \
        __host__ __device__ __forceinline__ name() {}\
        __host__ __device__ __forceinline__ name(const name&) {}\
    };

    template <typename T> struct maximum : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType lhs, typename TypeTraits<T>::ParameterType rhs) const
        {
            return max(lhs, rhs);
        }
        __host__ __device__ __forceinline__ maximum() {}
        __host__ __device__ __forceinline__ maximum(const maximum&) {}
    };

    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, uchar, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, schar, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, char, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, ushort, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, short, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, int, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, uint, ::max)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, float, ::fmax)
    OPENCC_CUDA_IMPLEMENT_MINMAX(maximum, double, ::fmax)

    template <typename T> struct minimum : binary_function<T, T, T>
    {
        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType lhs, typename TypeTraits<T>::ParameterType rhs) const
        {
            return min(lhs, rhs);
        }
        __host__ __device__ __forceinline__ minimum() {}
        __host__ __device__ __forceinline__ minimum(const minimum&) {}
    };

    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, uchar, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, schar, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, char, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, ushort, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, short, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, int, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, uint, ::min)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, float, ::fmin)
    OPENCC_CUDA_IMPLEMENT_MINMAX(minimum, double, ::fmin)

#undef OPENCC_CUDA_IMPLEMENT_MINMAX

    // Math functions

    template <typename T> struct abs_func : unary_function<T, T>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType x) const
        {
            return abs(x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<unsigned char> : unary_function<unsigned char, unsigned char>
    {
        __device__ __forceinline__ unsigned char operator ()(unsigned char x) const
        {
            return x;
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<signed char> : unary_function<signed char, signed char>
    {
        __device__ __forceinline__ signed char operator ()(signed char x) const
        {
            return ::abs((int)x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<char> : unary_function<char, char>
    {
        __device__ __forceinline__ char operator ()(char x) const
        {
            return ::abs((int)x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<unsigned short> : unary_function<unsigned short, unsigned short>
    {
        __device__ __forceinline__ unsigned short operator ()(unsigned short x) const
        {
            return x;
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<short> : unary_function<short, short>
    {
        __device__ __forceinline__ short operator ()(short x) const
        {
            return ::abs((int)x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<unsigned int> : unary_function<unsigned int, unsigned int>
    {
        __device__ __forceinline__ unsigned int operator ()(unsigned int x) const
        {
            return x;
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<int> : unary_function<int, int>
    {
        __device__ __forceinline__ int operator ()(int x) const
        {
            return ::abs(x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<float> : unary_function<float, float>
    {
        __device__ __forceinline__ float operator ()(float x) const
        {
            return ::fabsf(x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };
    template <> struct abs_func<double> : unary_function<double, double>
    {
        __device__ __forceinline__ double operator ()(double x) const
        {
            return ::fabs(x);
        }

        __host__ __device__ __forceinline__ abs_func() {}
        __host__ __device__ __forceinline__ abs_func(const abs_func&) {}
    };

#define OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(name, func) \
    template <typename T> struct name ## _func : unary_function<T, float> \
    { \
        __device__ __forceinline__ float operator ()(typename TypeTraits<T>::ParameterType v) const \
        { \
            return func ## f(v); \
        } \
        __host__ __device__ __forceinline__ name ## _func() {} \
        __host__ __device__ __forceinline__ name ## _func(const name ## _func&) {} \
    }; \
    template <> struct name ## _func<double> : unary_function<double, double> \
    { \
        __device__ __forceinline__ double operator ()(double v) const \
        { \
            return func(v); \
        } \
        __host__ __device__ __forceinline__ name ## _func() {} \
        __host__ __device__ __forceinline__ name ## _func(const name ## _func&) {} \
    };

#define OPENCC_CUDA_IMPLEMENT_BIN_FUNCTOR(name, func) \
    template <typename T> struct name ## _func : binary_function<T, T, float> \
    { \
        __device__ __forceinline__ float operator ()(typename TypeTraits<T>::ParameterType v1, typename TypeTraits<T>::ParameterType v2) const \
        { \
            return func ## f(v1, v2); \
        } \
        __host__ __device__ __forceinline__ name ## _func() {} \
        __host__ __device__ __forceinline__ name ## _func(const name ## _func&) {} \
    }; \
    template <> struct name ## _func<double> : binary_function<double, double, double> \
    { \
        __device__ __forceinline__ double operator ()(double v1, double v2) const \
        { \
            return func(v1, v2); \
        } \
        __host__ __device__ __forceinline__ name ## _func() {} \
        __host__ __device__ __forceinline__ name ## _func(const name ## _func&) {} \
    };

    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(sqrt, ::sqrt)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(exp, ::exp)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(exp2, ::exp2)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(exp10, ::exp10)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(log, ::log)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(log2, ::log2)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(log10, ::log10)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(sin, ::sin)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(cos, ::cos)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(tan, ::tan)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(asin, ::asin)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(acos, ::acos)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(atan, ::atan)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(sinh, ::sinh)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(cosh, ::cosh)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(tanh, ::tanh)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(asinh, ::asinh)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(acosh, ::acosh)
    OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR(atanh, ::atanh)

    OPENCC_CUDA_IMPLEMENT_BIN_FUNCTOR(hypot, ::hypot)
    OPENCC_CUDA_IMPLEMENT_BIN_FUNCTOR(atan2, ::atan2)
    OPENCC_CUDA_IMPLEMENT_BIN_FUNCTOR(pow, ::pow)

    #undef OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR
    #undef OPENCC_CUDA_IMPLEMENT_UN_FUNCTOR_NO_DOUBLE
    #undef OPENCC_CUDA_IMPLEMENT_BIN_FUNCTOR

    template<typename T> struct hypot_sqr_func : binary_function<T, T, float>
    {
        __device__ __forceinline__ T operator ()(typename TypeTraits<T>::ParameterType src1, typename TypeTraits<T>::ParameterType src2) const
        {
            return src1 * src1 + src2 * src2;
        }
        __host__ __device__ __forceinline__ hypot_sqr_func() {}
        __host__ __device__ __forceinline__ hypot_sqr_func(const hypot_sqr_func&) {}
    };

    // Saturate Cast Functor
    template <typename T, typename D> struct saturate_cast_func : unary_function<T, D>
    {
        __device__ __forceinline__ D operator ()(typename TypeTraits<T>::ParameterType v) const
        {
            return saturate_cast<D>(v);
        }
        __host__ __device__ __forceinline__ saturate_cast_func() {}
        __host__ __device__ __forceinline__ saturate_cast_func(const saturate_cast_func&) {}
    };

    // Threshold Functors
    template <typename T> struct thresh_binary_func : unary_function<T, T>
    {
        __host__ __device__ __forceinline__ thresh_binary_func(T thresh_, T maxVal_) : thresh(thresh_), maxVal(maxVal_) {}

        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType src) const
        {
            return (src > thresh) * maxVal;
        }

        __host__ __device__ __forceinline__ thresh_binary_func() {}
        __host__ __device__ __forceinline__ thresh_binary_func(const thresh_binary_func& other)
            : thresh(other.thresh), maxVal(other.maxVal) {}

        T thresh;
        T maxVal;
    };

    template <typename T> struct thresh_binary_inv_func : unary_function<T, T>
    {
        __host__ __device__ __forceinline__ thresh_binary_inv_func(T thresh_, T maxVal_) : thresh(thresh_), maxVal(maxVal_) {}

        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType src) const
        {
            return (src <= thresh) * maxVal;
        }

        __host__ __device__ __forceinline__ thresh_binary_inv_func() {}
        __host__ __device__ __forceinline__ thresh_binary_inv_func(const thresh_binary_inv_func& other)
            : thresh(other.thresh), maxVal(other.maxVal) {}

        T thresh;
        T maxVal;
    };

    template <typename T> struct thresh_trunc_func : unary_function<T, T>
    {
        explicit __host__ __device__ __forceinline__ thresh_trunc_func(T thresh_, T maxVal_ = 0) : thresh(thresh_) {(void)maxVal_;}

        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType src) const
        {
            return minimum<T>()(src, thresh);
        }

        __host__ __device__ __forceinline__ thresh_trunc_func() {}
        __host__ __device__ __forceinline__ thresh_trunc_func(const thresh_trunc_func& other)
            : thresh(other.thresh) {}

        T thresh;
    };

    template <typename T> struct thresh_to_zero_func : unary_function<T, T>
    {
        explicit __host__ __device__ __forceinline__ thresh_to_zero_func(T thresh_, T maxVal_ = 0) : thresh(thresh_) {(void)maxVal_;}

        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType src) const
        {
            return (src > thresh) * src;
        }

        __host__ __device__ __forceinline__ thresh_to_zero_func() {}
       __host__  __device__ __forceinline__ thresh_to_zero_func(const thresh_to_zero_func& other)
            : thresh(other.thresh) {}

        T thresh;
    };

    template <typename T> struct thresh_to_zero_inv_func : unary_function<T, T>
    {
        explicit __host__ __device__ __forceinline__ thresh_to_zero_inv_func(T thresh_, T maxVal_ = 0) : thresh(thresh_) {(void)maxVal_;}

        __device__ __forceinline__ T operator()(typename TypeTraits<T>::ParameterType src) const
        {
            return (src <= thresh) * src;
        }

        __host__ __device__ __forceinline__ thresh_to_zero_inv_func() {}
        __host__ __device__ __forceinline__ thresh_to_zero_inv_func(const thresh_to_zero_inv_func& other)
            : thresh(other.thresh) {}

        T thresh;
    };

    // Function Object Adaptors
    template <typename Predicate> struct unary_negate : unary_function<typename Predicate::argument_type, bool>
    {
      explicit __host__ __device__ __forceinline__ unary_negate(const Predicate& p) : pred(p) {}

      __device__ __forceinline__ bool operator()(typename TypeTraits<typename Predicate::argument_type>::ParameterType x) const
      {
          return !pred(x);
      }

      __host__ __device__ __forceinline__ unary_negate() {}
      __host__ __device__ __forceinline__ unary_negate(const unary_negate& other) : pred(other.pred) {}

      Predicate pred;
    };

    template <typename Predicate> __host__ __device__ __forceinline__ unary_negate<Predicate> not1(const Predicate& pred)
    {
        return unary_negate<Predicate>(pred);
    }

    template <typename Predicate> struct binary_negate : binary_function<typename Predicate::first_argument_type, typename Predicate::second_argument_type, bool>
    {
        explicit __host__ __device__ __forceinline__ binary_negate(const Predicate& p) : pred(p) {}

        __device__ __forceinline__ bool operator()(typename TypeTraits<typename Predicate::first_argument_type>::ParameterType x,
                                                   typename TypeTraits<typename Predicate::second_argument_type>::ParameterType y) const
        {
            return !pred(x,y);
        }

        __host__ __device__ __forceinline__ binary_negate() {}
        __host__ __device__ __forceinline__ binary_negate(const binary_negate& other) : pred(other.pred) {}

        Predicate pred;
    };

    template <typename BinaryPredicate> __host__ __device__ __forceinline__ binary_negate<BinaryPredicate> not2(const BinaryPredicate& pred)
    {
        return binary_negate<BinaryPredicate>(pred);
    }

    template <typename Op> struct binder1st : unary_function<typename Op::second_argument_type, typename Op::result_type>
    {
        __host__ __device__ __forceinline__ binder1st(const Op& op_, const typename Op::first_argument_type& arg1_) : op(op_), arg1(arg1_) {}

        __device__ __forceinline__ typename Op::result_type operator ()(typename TypeTraits<typename Op::second_argument_type>::ParameterType a) const
        {
            return op(arg1, a);
        }

        __host__ __device__ __forceinline__ binder1st() {}
        __host__ __device__ __forceinline__ binder1st(const binder1st& other) : op(other.op), arg1(other.arg1) {}

        Op op;
        typename Op::first_argument_type arg1;
    };

    template <typename Op, typename T> __host__ __device__ __forceinline__ binder1st<Op> bind1st(const Op& op, const T& x)
    {
        return binder1st<Op>(op, typename Op::first_argument_type(x));
    }

    template <typename Op> struct binder2nd : unary_function<typename Op::first_argument_type, typename Op::result_type>
    {
        __host__ __device__ __forceinline__ binder2nd(const Op& op_, const typename Op::second_argument_type& arg2_) : op(op_), arg2(arg2_) {}

        __forceinline__ __device__ typename Op::result_type operator ()(typename TypeTraits<typename Op::first_argument_type>::ParameterType a) const
        {
            return op(a, arg2);
        }

        __host__ __device__ __forceinline__ binder2nd() {}
        __host__ __device__ __forceinline__ binder2nd(const binder2nd& other) : op(other.op), arg2(other.arg2) {}

        Op op;
        typename Op::second_argument_type arg2;
    };

    template <typename Op, typename T> __host__ __device__ __forceinline__ binder2nd<Op> bind2nd(const Op& op, const T& x)
    {
        return binder2nd<Op>(op, typename Op::second_argument_type(x));
    }

    // Functor Traits
    template <typename F> struct IsUnaryFunction
    {
        typedef char Yes;
        struct No {Yes a[2];};

        template <typename T, typename D> static Yes check(unary_function<T, D>);
        static No check(...);

        static F makeF();

        enum { value = (sizeof(check(makeF())) == sizeof(Yes)) };
    };

    template <typename F> struct IsBinaryFunction
    {
        typedef char Yes;
        struct No {Yes a[2];};

        template <typename T1, typename T2, typename D> static Yes check(binary_function<T1, T2, D>);
        static No check(...);

        static F makeF();

        enum { value = (sizeof(check(makeF())) == sizeof(Yes)) };
    };

    namespace functional_detail
    {
        template <size_t src_elem_size, size_t dst_elem_size> struct UnOpShift { enum { shift = 1 }; };
        template <size_t src_elem_size> struct UnOpShift<src_elem_size, 1> { enum { shift = 4 }; };
        template <size_t src_elem_size> struct UnOpShift<src_elem_size, 2> { enum { shift = 2 }; };

        template <typename T, typename D> struct DefaultUnaryShift
        {
            enum { shift = UnOpShift<sizeof(T), sizeof(D)>::shift };
        };

        template <size_t src_elem_size1, size_t src_elem_size2, size_t dst_elem_size> struct BinOpShift { enum { shift = 1 }; };
        template <size_t src_elem_size1, size_t src_elem_size2> struct BinOpShift<src_elem_size1, src_elem_size2, 1> { enum { shift = 4 }; };
        template <size_t src_elem_size1, size_t src_elem_size2> struct BinOpShift<src_elem_size1, src_elem_size2, 2> { enum { shift = 2 }; };

        template <typename T1, typename T2, typename D> struct DefaultBinaryShift
        {
            enum { shift = BinOpShift<sizeof(T1), sizeof(T2), sizeof(D)>::shift };
        };

        template <typename Func, bool unary = IsUnaryFunction<Func>::value> struct ShiftDispatcher;
        template <typename Func> struct ShiftDispatcher<Func, true>
        {
            enum { shift = DefaultUnaryShift<typename Func::argument_type, typename Func::result_type>::shift };
        };
        template <typename Func> struct ShiftDispatcher<Func, false>
        {
            enum { shift = DefaultBinaryShift<typename Func::first_argument_type, typename Func::second_argument_type, typename Func::result_type>::shift };
        };
    }

    template <typename Func> struct DefaultTransformShift
    {
        enum { shift = functional_detail::ShiftDispatcher<Func>::shift };
    };

    template <typename Func> struct DefaultTransformFunctorTraits
    {
        enum { simple_block_dim_x = 16 };
        enum { simple_block_dim_y = 16 };

        enum { smart_block_dim_x = 16 };
        enum { smart_block_dim_y = 16 };
        enum { smart_shift = DefaultTransformShift<Func>::shift };
    };

    template <typename Func> struct TransformFunctorTraits : DefaultTransformFunctorTraits<Func> {};

#define OPENCC_CUDA_TRANSFORM_FUNCTOR_TRAITS(type) \
    template <> struct TransformFunctorTraits< type > : DefaultTransformFunctorTraits< type >
}}} // namespace cv { namespace cuda { namespace cudev

//! @endcond

#endif // OPENCC_CUDA_FUNCTIONAL_HPP
