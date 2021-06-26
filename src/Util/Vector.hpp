#ifndef VECTOR_HPP
#define VECTOR_HPP

#if defined(__GNUC__)
#define vector_inline /** @cond */ __attribute__((always_inline)) /** @endcond */
#else
#define vector_inline /** @cond */ inline /** @endcond */
#endif

#define ASSERT_HERE(msg)          \
{                                 \
	[]<bool v = false>()          \
	{ static_assert(v, msg); }(); \
}

#ifdef VECTOR_USE_STD
#include <iostream>
#include <cmath>
#endif

#ifdef VECTOR_USE_FMT
#include <fmt/format.h>
#endif

#define __expand(...) __VA_ARGS__
#define vector_foreach_do_v(cexpr, expr, __no_parallel, __N, __capture)                                                          \
	{                                                                                                                            \
		if constexpr (S.UseFor)                                                                                                  \
		{                                                                                                                        \
			size_type i;                                                                                                         \
			if constexpr (!__no_parallel && S.UseParallel)                                                                       \
			{                                                                                                                    \
				if constexpr (S.NumThreads != 0)                                                                                 \
				{                                                                                                                \
					_Pragma("omp parallel for private(i) schedule(dynamic) num_threads(S.NumThreads)") for (i = 0; i < __N; ++i) \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
				else                                                                                                             \
				{                                                                                                                \
					_Pragma(                                                                                                     \
						"omp parallel for private(i) schedule(dynamic) ") for (i = 0;                                            \
																			   i < __N;                                          \
																			   ++i)                                              \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
			}                                                                                                                    \
			else                                                                                                                 \
			{                                                                                                                    \
				if constexpr (S.UseSIMD)                                                                                         \
				{                                                                                                                \
					_Pragma(                                                                                                     \
						"omp simd safelen(S.SIMDSafeLen) simdlen(S.SIMDLen)") for (i = 0;                                        \
																				   i <                                           \
																				   __N;                                          \
																				   ++i)                                          \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
				else                                                                                                             \
				{                                                                                                                \
					for (i = 0; i < __N; ++i)                                                                                    \
					{                                                                                                            \
						(__expand(expr));                                                                                        \
					}                                                                                                            \
				}                                                                                                                \
			}                                                                                                                    \
		}                                                                                                                        \
		else                                                                                                                     \
		{                                                                                                                        \
			if constexpr (S.NoCE)                                                                                                \
			{                                                                                                                    \
				[__capture]<size_type... i>(MPDetail::sequence<size_type, i...>)                                                 \
				{                                                                                                                \
					((__expand(expr)), ...);                                                                                     \
				}                                                                                                                \
				(typename MPDetail::SetSize<size_type>::make_sequence<size_type, __N>::type{});                                  \
			}                                                                                                                    \
			else                                                                                                                 \
			{                                                                                                                    \
				[__capture]<size_type... i>(MPDetail::sequence<size_type, i...>)                                                 \
				{                                                                                                                \
					((__expand(cexpr)), ...);                                                                                    \
				}                                                                                                                \
				(typename MPDetail::SetSize<size_type>::make_sequence<size_type, __N>::type{});                                  \
			}                                                                                                                    \
		}                                                                                                                        \
	}
#define vector_foreach_do(cexpr, expr) vector_foreach_do_v(cexpr, expr, false, N, __expand(&, this))

namespace MPDetail
{
	////////////////////////////////////////////////
	/// \brief Represents a compile-time constant
	/// \tparam T The constant's type
	/// \tparam v The constant's value
	////////////////////////////////////////////////
	template <class T, T v>
	struct constant
	{
		using value_type = T;
		using type = constant;
		static constexpr inline T value = v;
	
		constexpr operator value_type() const noexcept
		{ return value; }
		constexpr value_type operator()() const noexcept
		{ return value; }
	};

	////////////////////////////////////////////////
	/// \brief Checks if two types are equal
	/// \tparam U The first type
	/// \tparam V The second type
	////////////////////////////////////////////////
	template <class U, class V>
	struct is_same : constant<bool, false> {};

	template <class U>
	struct is_same<U, U> : constant<bool, true> {};

	////////////////////////////////////////////////
	/// \brief Checks if two types are equal
	/// \tparam U The first type
	/// \tparam V The second type
	////////////////////////////////////////////////
	template<class U, class V>
	static constexpr inline bool is_same_v = is_same<U, V>::value;

	////////////////////////////////////////////////
	/// \brief Checks if two types are equal (commutatively)
	/// \tparam U The first type
	/// \tparam V The second type
	////////////////////////////////////////////////
	template <class U, class V>
	concept same_as = is_same_v<U, V> && is_same_v<V, U>;

	////////////////////////////////////////////////
	/// \brief Removes reference
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	struct remove_reference
	{ using type = T; };
	
	////////////////////////////////////////////////
	/// \brief Removes reference
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	struct remove_reference<T&>
	{ using type = T; };
	
	////////////////////////////////////////////////
	/// \brief Removes reference
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	struct remove_reference<T&&>
	{ using type = T; };
	
	////////////////////////////////////////////////
	/// \brief Removes reference
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	using remove_reference_t = typename remove_reference<T>::type;
	
	////////////////////////////////////////////////
	/// \brief Moves an object
	/// \param value The oject
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	constexpr static remove_reference_t<T>&& move(T&& value) noexcept
	{ return static_cast<remove_reference_t<T>&&>(value); }
	
	////////////////////////////////////////////////
	/// \brief Forwards from lvalue
	/// \param value The object
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	constexpr static T&& forward(remove_reference_t<T>& value) noexcept
	{
		return static_cast<T&&>(value);
	}
	
	////////////////////////////////////////////////
	/// \brief Forwards from rvalue
	/// \param value The object
	/// \tparam T The type
	////////////////////////////////////////////////
	template <class T>
	constexpr static T&& forward(remove_reference_t<T>&& value) noexcept
	{
		return static_cast<T&&>(value);
	}

	template <class T, T... Is>
	struct sequence {};

	template <class...>
	struct merge_sequence;
	template <class T, T... Is, T... Js>
	struct merge_sequence<sequence<T, Is...>, sequence<T, Js...>>
	{ using type = sequence<T, Is..., Js...>; };

	template <class size_type>
	struct SetSize
	{
		// TODO: Better generation algorithm
		template <class T, size_type N, T... Is>
		struct make_sequence
		{
			using type = typename make_sequence<T, N-1, N-1, Is...>::type;
		};

		template <class T, T... Is>
		struct make_sequence<T, 0, Is...>
		{
			using type = sequence<T, Is...>;
		};
		template <class T, size_type N, size_type O = 0>
		struct generate_seq
		{
			using type = typename merge_sequence<
				typename generate_seq<T, N/2, O/2-N/2>::type,
				typename generate_seq<T, N-N/2, O-O/2 - N-N/2>::type
			>::type;
		};
		template <class T, size_type O>
		struct generate_seq<T, 0, O>
		{
			using type = sequence<T>;
		};
		template <class T, size_type O>
		struct generate_seq<T, 1, O>
		{
			using type = sequence<T, O>;
		};
	};

	////////////////////////////////////////////////
	/// \brief Details of a lambda
	/// Only retreives the lambda's return type
	////////////////////////////////////////////////
	template <class R, class C, class Mutable, class... Ts>
	struct lambda_detail
	{
		using return_type = R;
	};

	////////////////////////////////////////////////
	/// \brief Traits of a lambda expression (\see lambda_detail)
	/// \tparam F The lambda
	////////////////////////////////////////////////
	template <class F>
	struct lambda_traits : public lambda_traits<decltype(&F::operator())> {};
	template <class R, class C, class... Ts>
	struct lambda_traits<R(C::*)(Ts...)> : public lambda_detail<R, C, constant<bool, true>, Ts...> {};
	template <class R, class C, class... Ts>
	struct lambda_traits<R(C::*)(Ts...) const> : public lambda_detail<R, C, constant<bool, false>, Ts...> {};

	////////////////////////////////////////////////
	/// \brief Details of a function
	/// Only retreives the function's return type
	////////////////////////////////////////////////
	template <class R, class... Ts>
	struct fn_detail
	{
		using return_type = R;
	};

	////////////////////////////////////////////////
	/// \brief Traits of a function (\see fn_detail)
	/// \tparam F The function
	////////////////////////////////////////////////
	template <class F>
	struct fn_traits : public fn_traits<decltype(&F::operator())> {};
	template <class T, class R, class... Ts>
	struct fn_traits<R (T::*)(Ts...) const> : public fn_detail<R, Ts...> {};


	////////////////////////////////////////////////
	/// \brief VectorBase is a class that can be a base for a vector
	////////////////////////////////////////////////
	template <class size_type, class T, size_type N, template <class, size_type> class B>
	concept VectorBase =
	requires(B<T, N>& b, size_type i)
	{
		{ static_cast<const B<T, N>&>(b)[i] } -> same_as<const T&>;
		{ b[i] } -> same_as<T&>;
		//{ static_cast<const B<T, N>&>(b).get<i>() } -> MPDetail::same_as<const T&>;
		//{ b.get<i>() } -> MPDetail::same_as<T&>;
	};

	template <class size_type, class T, class C>
	concept Container =
	requires(C& c, size_type i)
	{
		{ c[i] } -> same_as<T&>;
		{ static_cast<const C&>(c)[i] } -> same_as<const T&>;
	};

	// {{{ Functors
	// Arithmetic
	////////////////////////////////////////////////
	/// \brief Add functor
	/// Returns ```x + y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x + y; }
	struct Add
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x + y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x + y; }
	};
	////////////////////////////////////////////////
	/// \brief Substract functor
	/// Returns ```x - y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x - y; }
	struct Sub
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x - y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x - y; }
	};
	////////////////////////////////////////////////
	/// \brief Multiplication functor
	/// Returns ```x * y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x * y; }
	struct Mul
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x * y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x * y; }
	};
	////////////////////////////////////////////////
	/// \brief Division functor
	/// Returns ```x / y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x / y; }
	struct Div
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x / y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x / y; }
	};
	////////////////////////////////////////////////
	/// \brief Modulo functor
	/// Returns ```x % y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x % y; }
	struct Mod
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x % y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x % y; }
	};
	////////////////////////////////////////////////
	/// \brief Additive inverse functor
	/// Returns ```-x```
	/// \tparam X The first type
	////////////////////////////////////////////////
	template <class X>
		requires requires (const X& x) { -x; }
	struct AddInv
	{
		using return_type = typename lambda_traits<decltype([](const X& x){ return -x; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x) const noexcept
		{ return -x; }
	};

	// Increment / Decrement
	////////////////////////////////////////////////
	/// \brief Preincrementation functor
	/// Returns ```++x```
	/// \tparam X The first type
	////////////////////////////////////////////////
	template <class X>
		requires requires (X& x) { ++x; }
	struct PreInc
	{
		using return_type = typename lambda_traits<decltype([](X& x){ return ++x; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(X& x) const noexcept
		{ return ++x; }
	};
	////////////////////////////////////////////////
	/// \brief Predecrementation functor
	/// Returns ```--x```
	/// \tparam X The first type
	////////////////////////////////////////////////
	template <class X>
		requires requires (X& x) { --x; }
	struct PreDec
	{
		using return_type = typename lambda_traits<decltype([](X& x){ return --x; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(X& x) const noexcept
		{ return --x; }
	};

	// Bitwise
	////////////////////////////////////////////////
	/// \brief Bitwise NOT functor
	/// Returns ```~x```
	/// \tparam X The first type
	////////////////////////////////////////////////
	template <class X>
		requires requires (const X& x) { ~x; }
	struct BNot
	{
		using return_type = typename lambda_traits<decltype([](const X& x){ return ~x; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x) const noexcept
		{ return ~x; }
	};
	////////////////////////////////////////////////
	/// \brief Bitwise AND functor
	/// Returns ```x & y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x & y; }
	struct BAnd
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x & y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x & y; }
	};
	////////////////////////////////////////////////
	/// \brief Bitwise OR functor
	/// Returns ```x | y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x | y; }
	struct BOr
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x | y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x | y; }
	};
	////////////////////////////////////////////////
	/// \brief Bitwise XOR functor
	/// Returns ```x ^ y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x ^ y; }
	struct BXor
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x ^ y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x ^ y; }
	};
	////////////////////////////////////////////////
	/// \brief Bitwise LEFT SHIFT functor
	/// Returns ```x << y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x << y; }
	struct BLShift
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x << y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x << y; }
	};
	////////////////////////////////////////////////
	/// \brief Bitwise RIGHT SHIFT functor
	/// Returns ```x >> y```
	/// \tparam X The first type
	/// \tparam Y The second type
	////////////////////////////////////////////////
	template <class X, class Y>
		requires requires (const X& x, const Y& y) { x >> y; }
	struct BRShift
	{
		using return_type = typename lambda_traits<decltype([](const X& x, const Y& y){ return x >> y; })>::return_type;
		[[nodiscard("Arithmetic expression")]] constexpr vector_inline return_type operator()(const X& x, const Y& y) const noexcept
		{ return x >> y; }
	};
	// }}}
} // MPDetail

template <class size_type>
struct MPVector
{
////////////////////////////////////////////////
/// \brief Master struct that defines Vector-related classes
/// \tparam size_type Type to represent a size, use std::size_t if you're working with the STL
////////////////////////////////////////////////

/** @cond */
template <class T, size_type N>
struct __default_base
{
	T data[N];

	constexpr const T& operator[](size_type i) const noexcept
	{ return data[i]; }
	constexpr T& operator[](size_type i) noexcept
	{ return data[i]; }

	template <size_type i>
	constexpr const T& Get() const noexcept
	{ return data[i]; }
	template <size_type i>
	constexpr T& Get() noexcept
	{ return data[i]; }

};
/** @endcond */

struct Settings
{
	////////////////////////////////////////////////////////////
	/// \brief VectorSettings
	/// \brief Settings for the vector class
	/// \brief Settings modify how operations are done (operators, etc...)
	////////////////////////////////////////////////////////////

	bool UseFor = false; ///< Use for loop instead of (force) unrolled loop
	bool NoCE = false; ///< Disables the constexpr ```Get<i>()``` operations, useful for using std::array as a base

	bool UseParallel = false; ///< Use parallelization (The overhead is often not worth the cost)
	size_type NumThreads = 0; ///< Number of threads for parallelization (0 = all cpu threads)

	bool UseSIMD = false; ///< Hints the compiler to use SIMD instruction inside the for loop
	size_type SIMDSafeLen = 0; ///< Minimum distance for splitting the logical iteration space
	size_type SIMDLen = 4; ///< Preferred number of concurrent iterations
};

template <class T, size_type N, template <class, size_type> class VectorBase = __default_base, Settings S = Settings{}>
requires MPDetail::VectorBase<size_type, T, N, VectorBase>
class Vector : public VectorBase<T, N>
{
public:
	////////////////////////////////////////////////
	/// \brief Main Vector class
	/// \tparam T The vector's base type
	/// \tparam N The vector's dimension
	/// \tparam VectorBase The vector's base (\see MPDetail::VectorBase)
	/// \tparam S The vector's settings
	////////////////////////////////////////////////
	using base_type = VectorBase<T, N>;

	////////////////////////////////////////////////
	/// \brief Vector's size
	/// \returns The number of elements in the vector
	////////////////////////////////////////////////
	[[nodiscard("size")]] static consteval size_type size() noexcept
	{ return N; }

	//{{{ Base Constructors/Destructor
	////////////////////////////////////////////////
	/// \brief Default constructor
	/// Initializes the vector using VectorBase's default constructor
	////////////////////////////////////////////////
	[[nodiscard("Constructor")]] vector_inline constexpr Vector() noexcept: base_type() {}
	////////////////////////////////////////////////
	/// \brief Copy constructor
	/// \param v Vector to copy
	////////////////////////////////////////////////
	[[nodiscard("Constructor")]] vector_inline constexpr Vector(Vector& v) noexcept:
		base_type()
	{
		vector_foreach_do(
			base_type::template Get<i>() = v.template Get<i>(),
			base_type::operator[](i) = v.base_type::operator[](i)
		);
	}
	////////////////////////////////////////////////
	/// \brief Const copy constructor
	/// \param v Vector to copy
	////////////////////////////////////////////////
	[[nodiscard("Constructor")]] vector_inline constexpr Vector(const Vector& v) noexcept:
		base_type()
	{
		vector_foreach_do(
			base_type::template Get<i>() = v.template Get<i>(),
			base_type::operator[](i) = v.base_type::operator[](i)
		);
	}
	////////////////////////////////////////////////
	/// \brief Move constructor
	/// \param v Vector to move from
	////////////////////////////////////////////////
	[[nodiscard("Constructor")]] vector_inline constexpr Vector(Vector&& v) noexcept:
		base_type()
	{
		vector_foreach_do(
			base_type::template Get<i>() = MPDetail::move(v.template Get<i>()),
			base_type::operator[](i) = MPDetail::move(v.base_type::operator[](i))
		);
	}
	
	////////////////////////////////////////////////
	/// \brief Destructor
	////////////////////////////////////////////////
	vector_inline constexpr ~Vector() noexcept {}
	// }}}

	//{{{ Container Constructors
	////////////////////////////////////////////////
	/// \brief Container constructor (Copy)
	/// Constructs from a container (\see MPDetail::Container)
	/// \param c The container (its size must be >= N)
	////////////////////////////////////////////////
	template <class Container>
	requires MPDetail::Container<size_type, T, Container>
	[[nodiscard("Constructor")]] vector_inline explicit constexpr Vector(const Container& c) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = c[i],
			base_type::operator[](i) = c[i]
		);
	}

	////////////////////////////////////////////////
	/// \brief Container constructor (Copy)
	/// Constructs from a container (\see MPDetail::Container)
	/// \param c The container (its size must be >= N)
	////////////////////////////////////////////////
	template <class Container>
	requires MPDetail::Container<size_type, T, Container>
	[[nodiscard("Constructor")]] vector_inline explicit constexpr Vector(Container& c) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = c[i],
			base_type::operator[](i) = c[i]
		);
	}
	
	////////////////////////////////////////////////
	/// \brief Container constructor (Move)
	/// Constructs from a container (\see MPDetail::Container)
	/// \param c The container (its size must be >= N)
	////////////////////////////////////////////////
	template <class Container>
	requires MPDetail::Container<size_type, T, Container>
	[[nodiscard("Constructor")]] vector_inline explicit constexpr Vector(Container&& c) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = MPDetail::move(c[i]),
			base_type::operator[](i) = MPDetail::move(c[i])
		);
	}
	//}}}

	//{{{ Accessors
	////////////////////////////////////////////////
	/// \brief Get element
	/// Get the element at ```i```
	/// \param i The index to get the element at
	////////////////////////////////////////////////
	[[nodiscard("Value")]] vector_inline constexpr const T& operator[](size_type i) const noexcept
	{ return base_type::operator[](i); }
	////////////////////////////////////////////////
	/// \brief Get element
	/// Get the element at ```i```
	/// \param i The index to get the element at
	////////////////////////////////////////////////
	[[nodiscard("Value")]] vector_inline constexpr T& operator[](size_type i) noexcept
	{ return base_type::operator[](i); }

	////////////////////////////////////////////////
	/// \brief Get element
	/// Get the element at ```i```
	/// \param i The index to get the element at
	////////////////////////////////////////////////
	template <size_type i>
	[[nodiscard("Value")]] vector_inline constexpr const T& Get() const noexcept
	{ return base_type::template Get<i>(); }
	////////////////////////////////////////////////
	/// \brief Get element
	/// Get the element at ```i```
	/// \param i The index to get the element at
	////////////////////////////////////////////////
	template <size_type i>
	[[nodiscard("Value")]] vector_inline constexpr T& Get() noexcept
	{ return base_type::template Get<i>(); }
	//}}}

	//{{{ Misc utilities
	////////////////////////////////////////////////
	/// \brief Primitive conversion
	/// \tparam R Type to convert the primitives to
	/// \returns A vector .
	////////////////////////////////////////////////
	template <class R, size_type M = N, template <class, size_type> class Base = VectorBase>
	[[nodiscard("Conversion")]] vector_inline constexpr auto
		As() const noexcept
	{
		if constexpr (N >= M)
		{
			Vector<R, M, Base, S> ret;

			vector_foreach_do_v(
					ret.template Get<i>() = static_cast<R>(Get<i>()),
					ret.operator[](i) = static_cast<R>(operator[](i)),
			false, M, __expand(&, this))

			return ret;
		}
		else
		{
			Vector<R, M, Base, S> ret;

			vector_foreach_do_v(
					ret.template Get<i>() = static_cast<R>(Get<i>()),
					ret.operator[](i) = static_cast<R>(operator[](i)),
			false, N, __expand(&, this))

			return ret;
		}
	}
	
	////////////////////////////////////////////////
	/// \brief Sets all elemnts
	/// \param val The value to set all elements to
	////////////////////////////////////////////////
	vector_inline constexpr void Set(const T& val) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = val,
			base_type::operator[](i) = val
		);
	}

	////////////////////////////////////////////////
	/// \brief Apply a function on every elements of the vector
	/// \param f The function to apply ```<T(const T&)>```
	/// \returns A reference to the vector
	////////////////////////////////////////////////
	template <class F>
	Vector& Apply(F&& f) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = f(base_type::template Get<i>()),
			base_type::operator[](i) = f(base_type::operator[](i))
		);

		return *this;
	}

	////////////////////////////////////////////////
	/// \brief Apply a functor on every elements of the vector
	/// \tparam F The functor ```<T(const Vector&)>```
	/// \note The functor F, must be callable, and its ```operator()``` must take a template argument of type size_type
	/// \returns A reference to the vector
	////////////////////////////////////////////////
	template <class F>
	Vector& Apply() noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = F().template operator()<i>(*this),
			base_type::operator[](i) = F().template operator()<i>(*this)
		);

		return *this;
	}

	////////////////////////////////////////////////
	/// \brief Reduces the vector to a single scalar
	/// \param f The reduction function ```<R(const T&, const T&)>```
	/// \returns The scalar
	////////////////////////////////////////////////
	template <class F>
	auto Reduce(F&& f) noexcept
	{
		typename MPDetail::fn_traits<F>::return_type r;
		if constexpr (S.NoCE)
		{
			r = base_type::operator[](0);
		}
		else
		{
			r = base_type::template Get<0>();
		}
		vector_foreach_do_v(
			r = f(r, base_type::template Get<1+i>()),
			r = f(r, base_type::operator[](1+i)),
			false, N-1, __expand(&, this));

		return r;
	}

	////////////////////////////////////////////////
	/// \brief Reduces the vector to a single scalar
	/// \tparam F The functor ```<T(const Vector&, const T&)>```
	/// \note The functor F, must be callable, and its ```operator()``` must take a template argument of type size_type
	/// \returns The scalar
	////////////////////////////////////////////////
	template <class F>
	auto Reduce() noexcept
	{
		T r;
		if constexpr (S.NoCE)
		{
			r = base_type::operator[](0);
		}
		else
		{
			r = base_type::template Get<0>();
		}
		vector_foreach_do_v(
			r = F().template operator()<i+1>(*this, r),
			r = F().template operator()<i+1>(*this, r),
			false, N-1, __expand(&, this));

		return r;
	}
	
	////////////////////////////////////////////////
	/// \brief Find an element
	/// \param f The function ```<bool(const T&, const T&)>```
	/// \tparam Max The maximum number of iterations
	/// \returns The scalar
	////////////////////////////////////////////////
	template <class F, size_type Max = N>
	[[nodiscard("Value")]] vector_inline constexpr T Find(F&& f) const noexcept
	{
		T r;
		if constexpr (S.NoCE)
		{
			r = base_type::operator[](0);
		}
		else
		{
			r = base_type::template Get<0>();
		}
		vector_foreach_do_v(
			r = f(r, Get<i>()) ? Get<i>() : r,
			r = f(r, operator[](i)) ? operator[](i) : r,
		false, Max, __expand(&, this));

		return r;
	}
	////////////////////////////////////////////////
	/// \brief Find an element
	/// \tparam F The functor ```<bool(const Vector&, const T&)>```
	/// \tparam Max The maximum number of iterations
	/// \returns The scalar
	////////////////////////////////////////////////
	template <class F, size_type Max = N>
	[[nodiscard("Value")]] vector_inline constexpr T Find() const noexcept
	{
		T r;
		if constexpr (S.NoCE)
		{
			r = base_type::operator[](0);
		}
		else
		{
			r = base_type::template Get<0>();
		}
		vector_foreach_do_v(
			r = F().template operator()<i>(*this, r) ? Get<i>() : r,
			r = F().template operator()<i>(*this, r) ? operator[](i) : r,
		false, Max, __expand(&, this));

		return r;
	}
	//}}}

	//{{{ Printing
#ifdef VECTOR_USE_STD
	////////////////////////////////////////////////
	/// \brief Format to std::ostream
	////////////////////////////////////////////////
	template <class CharT>
	friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& s, const Vector& v)
	{
		s << '(';
		vector_foreach_do_v(
			s << v.base_type::template Get<i>() << ',' << ' ',
			s << v.base_type::operator[](i) << ',' << ' ',
		true, N-1, &);

		if constexpr (S.NoCE)
		{
			s << v.base_type::operator[](N-1);
		}
		else
		{
			s << v.base_type::template Get<N-1>();
		}

		s << ')';
		return s;
	}
#endif // VECTOR_USE_STD
	//}}}

	// {{{ Assignment
	constexpr Vector& operator=(const Vector& v) noexcept
	{
		vector_foreach_do(
			base_type::template Get<i>() = v.base_type::template Get<i>(),
			base_type::operator[](i) = v.base_type::operator[](i)
		);
		return *this;
	}
	// }}}

	//{{{ Operators
	//{{{ Operations
	/** @cond */
	template <class _T, template <class, size_type> class _VectorBase, template <class, class> class F>
	requires MPDetail::VectorBase<size_type, _T, N, _VectorBase>
	[[nodiscard("Arithmetic operation")]] vector_inline auto constexpr friend Operation(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	-> Vector<typename F<T, _T>::return_type, N, VectorBase, S>
	requires requires (F<T, _T>&& f, const T& x, const _T& y) { f(x, y); }
	{
		F<T, _T> f;
		Vector<typename F<T, _T>::return_type, N, VectorBase, S> ret;

		vector_foreach_do_v
		(
			ret.template Get<i>() = f(v1.template Get<i>(), v2.template Get<i>()),
			ret.operator[](i) = f(v1.operator[](i), v2.base_type::operator[](i)),
			false, N, &
		);

		return ret;
	}

	template <class _T, template <class, size_type> class _VectorBase, template <class, class> class F>
	requires MPDetail::VectorBase<size_type, _T, N, _VectorBase>
	vector_inline auto constexpr friend OperationEq(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	-> Vector<T, N, VectorBase, S>&
	requires requires (F<T, _T>&& f, const T& x, const _T& y) { f(x, y); } // TODO: is_convertible_nothrow
	{
		F<T, _T> f;

		vector_foreach_do_v
		(
			v1.template Get<i>() = f(v1.template Get<i>(), v2.template Get<i>()),
			v1.operator[](i) = f(v1.operator[](i), v2.operator[](i)),
			false, N, &
		);

		return v1;
	}

	// u = v * 4
	template <class _T, template <class, class> class F>
	[[nodiscard("Arithmetic operation")]] vector_inline auto constexpr friend OperationScalar(
			const Vector& v1,
			const _T& x) noexcept
	-> Vector<typename F<T, _T>::return_type, N, VectorBase, S>
	requires requires (F<T, _T>&& f, const T& x, const _T& y) { f(x, y); }
	{
		F<T, _T> f;
		Vector<typename F<T, _T>::return_type, N, VectorBase, S> ret;

		vector_foreach_do_v
		(
			ret.template Get<i>() = f(v1.template Get<i>(), x),
			ret.operator[](i) = f(v1.operator[](i), x),
			false, N, &
		);

		return ret;
	}

	// u *= 4
	template <class _T, template <class, class> class F>
	vector_inline auto constexpr friend OperationScalarEq(
			Vector& v1,
			const _T& x) noexcept
	-> Vector<T, N, VectorBase, S>&
	requires requires (F<T, _T>&& f, const T& x, const _T& y) { f(x, y); } // TODO: is_convertible_nothrow
	{
		F<T, _T> f;

		vector_foreach_do_v
		(
			v1.template Get<i>() = f(v1.template Get<i>(), x),
			v1.operator[](i) = f(v1.operator[](i), x),
			false, N, &
		);

		return v1;
	}

	// u = 4 * v
	template <class _T, template <class, class> class F>
	[[nodiscard("Arithmetic operation")]] vector_inline auto constexpr friend OperationScalar2(
			const _T& x,
			const Vector& v1) noexcept
	-> Vector<typename F<_T, T>::return_type, N, VectorBase, S>
	requires requires (F<_T, T>&& f, const _T& x, const T& y) { f(x, y); }
	{
		F<T, _T> f;
		Vector<typename F<_T, T>::return_type, N, VectorBase, S> ret;

		vector_foreach_do_v
		(
			ret.template Get<i>() = f(x, v1.template Get<i>()),
			ret.operator[](i) = f(x, v1.operator[](i)),
			false, N, &
		);

		return ret;
	}

	// -v
	template <template <class> class F>
	[[nodiscard("Arithmetic operation")]] vector_inline auto constexpr friend Operation1(
			const Vector& v) noexcept
	-> Vector<typename F<T>::return_type, N, VectorBase, S>
	requires requires (F<T>&& f, const T& x) { f(x); }
	{
		F<T> f;
		Vector<typename F<T>::return_type, N, VectorBase, S> ret;

		vector_foreach_do_v
		(
			ret.template Get<i>() = f(v.template Get<i>()),
			ret.operator[](i) = f(v.operator[](i)),
			false, N, &
		);

		return ret;
	}

	// -v
	template <template <class> class F>
	vector_inline auto constexpr friend Operation1Eq(Vector& v) noexcept
	-> Vector&
	requires requires (F<T>&& f, T& x) { f(x); } //TODO: is_convertible_nothrow
	{
		F<T> f;

		vector_foreach_do_v
		(
			v.template Get<i>() = f(v.template Get<i>()),
			v.operator[](i) = f(v.operator[](i)),
			false, N, &
		);

		return v;
	}
	/** @endcond */
	// }}}

	//{{{ x + y
	////////////////////////////////////////////////
	/// \brief Adds two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the sum of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator+(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::Add>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Adds two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the sum of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator+=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::Add>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Adds a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the sum of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator+(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::Add>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Adds a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the sum of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator+=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::Add>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Adds a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the sum of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator+(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::Add>(x, v1);
	}
	//}}}
	//{{{ x - y
	////////////////////////////////////////////////
	/// \brief Substracts two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the difference of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator-(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::Sub>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Substracts two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the difference of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator-=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::Sub>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Substracts a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the difference of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator-(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::Sub>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Substracts a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the difference of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator-=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::Sub>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Substracts a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the difference of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator-(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::Sub>(x, v1);
	}
	//}}}
	//{{{ x * y
	////////////////////////////////////////////////
	/// \brief Multiplies two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the product of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator*(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::Mul>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Multiplies two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the product of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator*=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::Mul>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Multiplies a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the product of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator*(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::Mul>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Multiplies a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the product of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator*=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::Mul>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Multiplies a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the product of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator*(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::Mul>(x, v1);
	}
	//}}}
	//{{{ x / y
	////////////////////////////////////////////////
	/// \brief Divides two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the quotient of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator/(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::Div>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Divides two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the quotient of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator/=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::Div>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Divides a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the quotient of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator/(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::Div>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Divides a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the quotient of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator/=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::Div>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Divides a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the quotient of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator/(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::Div>(x, v1);
	}
	//}}}
	//{{{ -x
	////////////////////////////////////////////////
	/// \brief Get the additive inverse
	/// \param v The vector
	/// \returns A vector containing the additive inverse of ```v```
	/// \note The base type of the resulting vector might differ from v
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator-(
			const Vector& v) noexcept
	{
		return Operation1<MPDetail::AddInv>(v);
	}
	//}}}
	//{{{ ++x
	////////////////////////////////////////////////
	/// \brief Pre increment
	/// \param v The vector
	/// \returns A vector containing the pre incremented ```v```
	////////////////////////////////////////////////
	vector_inline auto constexpr friend operator++(
			Vector& v) noexcept
	{
		return Operation1Eq<MPDetail::PreInc>(v);
	}
	//}}}
	//{{{ x++
	////////////////////////////////////////////////
	/// \brief Post increment
	/// \param v The vector
	/// \returns A vector containing the post incremented ```v```
	////////////////////////////////////////////////
	vector_inline auto constexpr friend operator++(
			Vector& v, int) noexcept
	{
		const auto cpy = v;
		++v;
		return cpy;
	}
	//}}}
	//{{{ --x
	////////////////////////////////////////////////
	/// \brief Pre decrement
	/// \param v The vector
	/// \returns A vector containing the pre decremented ```v```
	////////////////////////////////////////////////
	vector_inline auto constexpr friend operator--(
			Vector& v) noexcept
	{
		
		return Operation1Eq<MPDetail::PreDec>(v);
	}
	//}}}
	//{{{ x--
	////////////////////////////////////////////////
	/// \brief Post decrement
	/// \param v The vector
	/// \returns A vector containing the post decremented ```v```
	////////////////////////////////////////////////
	vector_inline auto constexpr friend operator--(
			Vector& v, int) noexcept
	{
		const auto cpy = v;
		--v;
		return cpy;
	}
	//}}}
	//{{{ x == y
	////////////////////////////////////////////////
	/// \brief Checks if two vector are equal
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns true if v1 and v2's elements are the same
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline bool constexpr friend operator==(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
		requires requires (const T& t, const _T& _t) { t == _t; }
	{
		bool eq = true;
		vector_foreach_do_v(
			eq &= v1.template Get<i>() == v2.template Get<i>(),
			eq &= v1.operator[](i) == v2.operator[](i),
			false, N, &);
		return eq;
	}
	// }}}
	// {{{ x != y
	////////////////////////////////////////////////
	/// \brief Checks if two vector are different
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns false if v1 and v2's elements are not the same
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline bool constexpr friend operator!=(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
		requires requires (const T& t, const _T& _t) { t == _t; }
	{
		bool eq = false;
		vector_foreach_do_v(
			eq |= v1.template Get<i>() != v2.template Get<i>(),
			eq |= v1.operator[](i) != v2.operator[](i),
			false, N, &);
		return eq;
	}

	//}}}
	//{{{ ~x
	////////////////////////////////////////////////
	/// \brief Bitwise NOT
	/// \param v The vector
	/// \returns A vector containing the negation (bitwise) of v's elements
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator~(const Vector& v) noexcept
	{
		return Operation1<MPDetail::BNot>(v);
	}
	//}}}
	//{{{ a & b
	////////////////////////////////////////////////
	/// \brief Bitwise AND two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the bitwise and of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator&(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::BAnd>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise AND two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the bitwise and of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator&=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::BAnd>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise AND a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the bitwise and of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator&(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::BAnd>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise AND a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the bitwise and of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator&=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::BAnd>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise AND a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the bitwise and of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator&(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::BAnd>(x, v1);
	}
	//}}}
	//{{{ x | y
	////////////////////////////////////////////////
	/// \brief Bitwise OR two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the bitwise or of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator|(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::BOr>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise OR two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the bitwise or of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator|=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::BOr>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise OR a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the bitwise or of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator|(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::BOr>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise OR a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the bitwise or of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator|=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::BOr>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise OR a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the bitwise or of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator|(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::BOr>(x, v1);
	}
	//}}}
	//{{{ x ^ y
	////////////////////////////////////////////////
	/// \brief Bitwise XOR two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the bitwise xor of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator^(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::BXor>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise XOR two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the bitwise xor of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator^=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::BXor>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise XOR a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the bitwise xor of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator^(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::BXor>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise XOR a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the bitwise xor of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator^=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::BXor>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise XOR a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the bitwise xor of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator^(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::BXor>(x, v1);
	}
	//}}}
	//{{{ x << y
	////////////////////////////////////////////////
	/// \brief Bitwise LSHIFT two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the bitwise lshift of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator<<(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::BLShift>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise LSHIFT two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the bitwise lshift of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator<<=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::BLShift>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise LSHIFT a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the bitwise lshift of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator<<(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::BLShift>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise LSHIFT a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the bitwise lshift of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator<<=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::BLShift>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise LSHIFT a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the bitwise lshift of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator<<(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::BLShift>(x, v1);
	}
	//}}}
	//{{{ x >> y
	////////////////////////////////////////////////
	/// \brief Bitwise RSHIFT two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A vector containing the bitwise rshift of all elements from v1 and v2
	/// \note The base type of the resulting vector might differ from v1 or v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator>>(
			const Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return Operation<_T, _VectorBase, MPDetail::BRShift>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise RSHIFT two vector
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns A reference to v1, containing the bitwise rshift of all elements from v1 and v2
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	vector_inline auto constexpr friend operator>>=(
			Vector& v1,
			const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		return OperationEq<_T, _VectorBase, MPDetail::BRShift>(v1, v2);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise RSHIFT a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A vector containing the bitwise rshift of all elements of v1 with x
	/// \note The base type of the resulting vector might differ from v1 or x
	////////////////////////////////////////////////
	template <class _T>
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator>>(
			const Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalar<_T, MPDetail::BRShift>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise RSHIFT a vector with a scalar
	/// \param v1 The vector
	/// \param x The scalar
	/// \returns A reference to v1, containing the bitwise rshift of all elements of v1 with x
	////////////////////////////////////////////////
	template <class _T>
	vector_inline auto constexpr friend operator>>=(
			Vector& v1,
			const _T& x) noexcept
	{
		return OperationScalarEq<_T, MPDetail::BRShift>(v1, x);
	}
	////////////////////////////////////////////////
	/// \brief Bitwise RSHIFT a scalar with a vector
	/// \param x The scalar
	/// \param v1 The vector
	/// \returns A vector containing the bitwise rshift of x with all elements of v1
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline auto constexpr friend operator>>(
			const T& x,
			const Vector& v1) noexcept
	{
		return OperationScalar2<T, MPDetail::BRShift>(x, v1);
	}
	//}}}
	//}}}

	//{{{ Iterators
	struct Iterator
	{
		////////////////////////////////////////////////
		/// \brief Iterator class
		////////////////////////////////////////////////
#ifdef VECTOR_USE_STD
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
#endif // VECTOR_USE_STD
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		
		Vector* ptr;
		size_type i;

		////////////////////////////////////////////////
		/// \brief Constructor
		/// \param _ptr Pointer to a Vector instance
		/// \param _i Index in vector
		////////////////////////////////////////////////
		vector_inline constexpr Iterator(Vector* _ptr, size_type _i) noexcept:
			ptr(_ptr), i(_i) {}
		////////////////////////////////////////////////
		/// \brief Copy Constructor
		/// \param it Iterator to copy
		////////////////////////////////////////////////
		vector_inline constexpr Iterator(const Iterator& it) noexcept:
			ptr(it.ptr), i(it.i) {}
		////////////////////////////////////////////////
		/// \brief Destructor
		////////////////////////////////////////////////
		vector_inline constexpr ~Iterator() noexcept {}

		////////////////////////////////////////////////
		/// \brief Assignment
		/// \param it The iterator to assign this to
		/// \returns A reference to the iterator after assignment
		////////////////////////////////////////////////
		vector_inline constexpr Iterator& operator=(const Iterator& it) noexcept
		{ i(it.i), ptr(it.ptr); return *this; }

		////////////////////////////////////////////////
		/// \brief Dereference
		/// \return A refence to the element represented by the iterator
		////////////////////////////////////////////////
		[[nodiscard("Value")]] vector_inline constexpr reference operator*() const noexcept
		{ return ptr->operator[](i); }
		////////////////////////////////////////////////
		/// \brief Dereference
		/// \return A pointer to the element represented by the iterator
		////////////////////////////////////////////////
		[[nodiscard("Pointer")]] vector_inline constexpr pointer operator->() const noexcept
		{ return &(ptr->operator[](i)); }

		////////////////////////////////////////////////
		/// \brief Pre increment
		/// \return A reference to the pre incremented iterator
		////////////////////////////////////////////////
		vector_inline constexpr Iterator& operator++() noexcept
		{ ++i; return *this; }
		////////////////////////////////////////////////
		/// \brief Post increment
		/// \return The iterator before incrementation
		////////////////////////////////////////////////
		vector_inline constexpr Iterator& operator++(int) noexcept
		{ const Iterator tmp = *this; this->operator++(); return tmp; }

		////////////////////////////////////////////////
		/// \brief Pre decrement
		/// \return A reference to the pre decremented iterator
		////////////////////////////////////////////////
		vector_inline constexpr Iterator& operator--() noexcept
		{ ++i; return *this; }
		////////////////////////////////////////////////
		/// \brief Post decrement
		/// \return The iterator before decrementation
		////////////////////////////////////////////////
		vector_inline constexpr Iterator& operator--(int) noexcept
		{ const Iterator tmp = *this; this->operator--(); return tmp; }

		////////////////////////////////////////////////
		/// \brief Check for equal
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if x and y point to the same index in the same vector
		////////////////////////////////////////////////
		vector_inline constexpr friend bool operator==(const Iterator& x, const Iterator& y) noexcept
		{ return x.i == y.i && x.ptr == y.ptr; }
		////////////////////////////////////////////////
		/// \brief Check for not equal
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if x and y do not point to the same index or not the same vector
		////////////////////////////////////////////////
		vector_inline constexpr friend bool operator!=(const Iterator& x, const Iterator& y) noexcept
		{ return x.i != y.i || x.ptr != y.ptr; }

		////////////////////////////////////////////////
		/// \brief Get incremented iterator
		/// \param it Iterator
		/// \param n How much to increment
		/// \return An iterator corresponding to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator+(const Iterator& it, size_type n) noexcept
		{
			Iterator cpy = it;
			cpy.i += n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Get incremented iterator
		/// \param n How much to increment
		/// \param it Iterator
		/// \return An iterator corresponding to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator+(size_type n, const Iterator& it) noexcept
		{
			Iterator cpy = it;
			cpy.i += n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Increment iterator
		/// \param it Iterator
		/// \param n How much to increment
		/// \return A reference to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend Iterator& operator+=(Iterator& it, size_type n) noexcept
		{
			it.i += n;
			return it;
		}
		////////////////////////////////////////////////
		/// \brief Increment iterator
		/// \param n How much to increment
		/// \param it Iterator
		/// \return A reference to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend Iterator& operator+=(size_type n, Iterator& it) noexcept
		{
			it.i += n;
			return it;
		}

		////////////////////////////////////////////////
		/// \brief Get decremented iterator
		/// \param it Iterator
		/// \param n How much to decrement
		/// \return An iterator corresponding to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator-(const Iterator& it, size_type n) noexcept
		{
			Iterator cpy = it;
			cpy.i -= n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Get decremented iterator
		/// \param n How much to decrement
		/// \param it Iterator
		/// \return An iterator corresponding to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator-(size_type n, const Iterator& it) noexcept
		{
			Iterator cpy = it;
			cpy.i -= n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Decrement iterator
		/// \param it Iterator
		/// \param n How much to decrement
		/// \return A reference to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend Iterator& operator-=(Iterator& it, size_type n) noexcept
		{
			it.i -= n;
			return it;
		}
		////////////////////////////////////////////////
		/// \brief Decrement iterator
		/// \param n How much to decrement
		/// \param it Iterator
		/// \return A reference to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend Iterator& operator-=(size_type n, Iterator& it) noexcept
		{
			it.i -= n;
			return it;
		}

		////////////////////////////////////////////////
		/// \brief Adds iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return An iterator correspondig to ```x + y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator+(const Iterator& x, const Iterator& y) noexcept
		{
			Iterator r(x);
			r.it += y.it;

			return r;
		}
		////////////////////////////////////////////////
		/// \brief Adds iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return A reference to ```x``` correspondig to ```x + y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator+=(Iterator& x, const Iterator& y) noexcept
		{
			x.it += y.it;

			return x;
		}
		////////////////////////////////////////////////
		/// \brief Substracts iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return An iterator correspondig to ```x - y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator-(const Iterator& x, const Iterator& y) noexcept
		{
			Iterator r(x);
			r.it -= y.it;

			return r;
		}
		////////////////////////////////////////////////
		/// \brief Substracts iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return A reference to ```x``` correspondig to ```x - y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend Iterator operator-=(Iterator& x, const Iterator& y) noexcept
		{
			x.it -= y.it;

			return x;
		}

		////////////////////////////////////////////////
		/// \brief Compare (<) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x < y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator<(const Iterator& x, const Iterator& y) noexcept
		{
			return x.it < y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (<=) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x <= y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator<=(const Iterator& x, const Iterator& y) noexcept
		{
			return x.it <= y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (>) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x > y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator>(const Iterator& x, const Iterator& y) noexcept
		{
			return x.it > y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (>=) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x >= y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator>=(const Iterator& x, const Iterator& y) noexcept
		{
			return x.it >= y.it;
		}

		////////////////////////////////////////////////
		/// \brief Access element
		/// \param n Offset from elemented referenced by iterator
		/// \return The element at offset ```n``` from iterator
		////////////////////////////////////////////////
		[[nodiscard("Value")]] vector_inline constexpr reference operator[](size_type n) const noexcept
		{
			return ptr->operator[](i + n);
		}
	};

	struct CIterator
	{
		////////////////////////////////////////////////
		/// \brief Const iterator class
		////////////////////////////////////////////////
#ifdef VECTOR_USE_STD
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
#endif // VECTOR_USE_STD
		using value_type = T;
		using pointer = const T*;
		using reference = const T&;
		
		Vector* ptr;
		size_type i;

		////////////////////////////////////////////////
		/// \brief Constructor
		/// \param _ptr Pointer to a Vector instance
		/// \param _i Index in vector
		////////////////////////////////////////////////
		vector_inline constexpr CIterator(Vector* _ptr, size_type _i) noexcept:
			ptr(_ptr), i(_i) {}
		////////////////////////////////////////////////
		/// \brief Copy Constructor
		/// \param it Iterator to copy
		////////////////////////////////////////////////
		vector_inline constexpr CIterator(const CIterator& it) noexcept:
			ptr(it.ptr), i(it.i) {}
		////////////////////////////////////////////////
		/// \brief Destructor
		////////////////////////////////////////////////
		vector_inline constexpr ~CIterator() noexcept {}

		////////////////////////////////////////////////
		/// \brief Assignment
		/// \param it The iterator to assign this to
		/// \returns A reference to the iterator after assignment
		////////////////////////////////////////////////
		vector_inline constexpr CIterator& operator=(const CIterator& it) noexcept
		{ i(it.i), ptr(it.ptr); return *this; }

		////////////////////////////////////////////////
		/// \brief Dereference
		/// \return A refence to the element represented by the iterator
		////////////////////////////////////////////////
		[[nodiscard("Value")]] vector_inline constexpr reference operator*() const noexcept
		{ return ptr->operator[](i); }
		////////////////////////////////////////////////
		/// \brief Dereference
		/// \return A pointer to the element represented by the iterator
		////////////////////////////////////////////////
		[[nodiscard("Pointer")]] vector_inline constexpr pointer operator->() const noexcept
		{ return &(ptr->operator[](i)); }

		////////////////////////////////////////////////
		/// \brief Pre increment
		/// \return A reference to the pre incremented iterator
		////////////////////////////////////////////////
		vector_inline constexpr CIterator& operator++() noexcept
		{ ++i; return *this; }
		////////////////////////////////////////////////
		/// \brief Post increment
		/// \return The iterator before incrementation
		////////////////////////////////////////////////
		vector_inline constexpr CIterator& operator++(int) noexcept
		{ const CIterator tmp = *this; this->operator++(); return tmp; }

		////////////////////////////////////////////////
		/// \brief Pre decrement
		/// \return A reference to the pre decremented iterator
		////////////////////////////////////////////////
		vector_inline constexpr CIterator& operator--() noexcept
		{ ++i; return *this; }
		////////////////////////////////////////////////
		/// \brief Post decrement
		/// \return The iterator before decrementation
		////////////////////////////////////////////////
		vector_inline constexpr CIterator& operator--(int) noexcept
		{ const CIterator tmp = *this; this->operator--(); return tmp; }

		////////////////////////////////////////////////
		/// \brief Check for equal
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if x and y point to the same index in the same vector
		////////////////////////////////////////////////
		vector_inline constexpr friend bool operator==(const CIterator& x, const CIterator& y) noexcept
		{ return x.i == y.i && x.ptr == y.ptr; }
		////////////////////////////////////////////////
		/// \brief Check for not equal
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if x and y do not point to the same index or not the same vector
		////////////////////////////////////////////////
		vector_inline constexpr friend bool operator!=(const CIterator& x, const CIterator& y) noexcept
		{ return x.i != y.i || x.ptr != y.ptr; }

		////////////////////////////////////////////////
		/// \brief Get incremented iterator
		/// \param it CIterator
		/// \param n How much to increment
		/// \return An iterator corresponding to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator+(const CIterator& it, size_type n) noexcept
		{
			Iterator cpy = it;
			cpy.i += n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Get incremented iterator
		/// \param n How much to increment
		/// \param it CIterator
		/// \return An iterator corresponding to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator+(size_type n, const CIterator& it) noexcept
		{
			Iterator cpy = it;
			cpy.i += n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Increment iterator
		/// \param it CIterator
		/// \param n How much to increment
		/// \return A reference to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend CIterator& operator+=(Iterator& it, size_type n) noexcept
		{
			it.i += n;
			return it;
		}
		////////////////////////////////////////////////
		/// \brief Increment iterator
		/// \param n How much to increment
		/// \param it CIterator
		/// \return A reference to ```it``` incremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend CIterator& operator+=(size_type n, CIterator& it) noexcept
		{
			it.i += n;
			return it;
		}

		////////////////////////////////////////////////
		/// \brief Get decremented iterator
		/// \param it CIterator
		/// \param n How much to decrement
		/// \return An iterator corresponding to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator-(const CIterator& it, size_type n) noexcept
		{
			Iterator cpy = it;
			cpy.i -= n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Get decremented iterator
		/// \param n How much to decrement
		/// \param it CIterator
		/// \return An iterator corresponding to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator-(size_type n, const CIterator& it) noexcept
		{
			Iterator cpy = it;
			cpy.i -= n;
			return cpy;
		}
		////////////////////////////////////////////////
		/// \brief Decrement iterator
		/// \param it CIterator
		/// \param n How much to decrement
		/// \return A reference to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend CIterator& operator-=(Iterator& it, size_type n) noexcept
		{
			it.i -= n;
			return it;
		}
		////////////////////////////////////////////////
		/// \brief Decrement iterator
		/// \param n How much to decrement
		/// \param it CIterator
		/// \return A reference to ```it``` decremented ```n``` times
		////////////////////////////////////////////////
		vector_inline constexpr friend CIterator& operator-=(size_type n, CIterator& it) noexcept
		{
			it.i -= n;
			return it;
		}

		////////////////////////////////////////////////
		/// \brief Adds iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return An iterator correspondig to ```x + y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator+(const CIterator& x, const CIterator& y) noexcept
		{
			Iterator r(x);
			r.it += y.it;

			return r;
		}
		////////////////////////////////////////////////
		/// \brief Adds iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return A reference to ```x``` correspondig to ```x + y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator+=(Iterator& x, const CIterator& y) noexcept
		{
			x.it += y.it;

			return x;
		}
		////////////////////////////////////////////////
		/// \brief Substracts iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return An iterator correspondig to ```x - y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator-(const CIterator& x, const CIterator& y) noexcept
		{
			Iterator r(x);
			r.it -= y.it;

			return r;
		}
		////////////////////////////////////////////////
		/// \brief Substracts iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return A reference to ```x``` correspondig to ```x - y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend CIterator operator-=(Iterator& x, const CIterator& y) noexcept
		{
			x.it -= y.it;

			return x;
		}

		////////////////////////////////////////////////
		/// \brief Compare (<) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x < y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator<(const CIterator& x, const CIterator& y) noexcept
		{
			return x.it < y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (<=) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x <= y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator<=(const CIterator& x, const CIterator& y) noexcept
		{
			return x.it <= y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (>) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x > y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator>(const CIterator& x, const CIterator& y) noexcept
		{
			return x.it > y.it;
		}
		////////////////////////////////////////////////
		/// \brief Compare (>=) iterators
		/// \param x First iterator
		/// \param y Second iterator
		/// \return true if ```x >= y```
		////////////////////////////////////////////////
		[[nodiscard("Arithmetic expression")]] vector_inline constexpr friend bool operator>=(const CIterator& x, const CIterator& y) noexcept
		{
			return x.it >= y.it;
		}

		////////////////////////////////////////////////
		/// \brief Access element
		/// \param n Offset from elemented referenced by iterator
		/// \return The element at offset ```n``` from iterator
		////////////////////////////////////////////////
		[[nodiscard("Value")]] vector_inline constexpr reference operator[](size_type n) const noexcept
		{
			return ptr->operator[](i + n);
		}
	};

	[[nodiscard("Iterator")]] vector_inline constexpr Iterator begin() noexcept
	{ return Iterator(this, 0); }
	[[nodiscard("Iterator")]] vector_inline constexpr Iterator end() noexcept
	{ return Iterator(this, N); }

	[[nodiscard("Iterator")]] vector_inline constexpr CIterator cbegin() noexcept
	{ return CIterator(this, 0); }
	[[nodiscard("Iterator")]] vector_inline constexpr CIterator cend() noexcept
	{ return CIterator(this, N); }
	//}}}

	//{{{ Mathematic utilities
#ifdef VECTOR_USE_STD
	////////////////////////////////////////////////
	/// \brief Euclidian norm (Norm 2)
	/// \tparam R The type to calculate the length in
	/// \param Simplify If true, will divide the values to keep them in [-1, 1] before calculating their squares
	/// \returns The Euclidian norm of the vector
	////////////////////////////////////////////////
	template <class R = T, bool Simplify = true>
	[[nodiscard("Arithmetic expression")]] vector_inline constexpr R Length() const noexcept
	{
		if constexpr (Simplify == true)
		{
			const T max = Find<decltype([]<size_type i>(const Vector& vec, const T& r)
			{
				if constexpr (S.NoCE)
				{ return std::abs(r) > std::abs(vec.operator[](i)); }
				else
				{ return std::abs(r) > std::abs(vec.template Get<i>()); }
			})>();

			if (max == T()) [[unlikely]]
				return R();

			const R rmax = static_cast<R>(max);
			R ret = R();

			R x;
			vector_foreach_do_v(
				__expand(x = static_cast<R>(std::abs(Get<i>())) / rmax, ret += x*x),
				__expand(x = static_cast<R>(std::abs(operator[](i))) / rmax, ret += x*x),
			false, N, __expand(&, this));

			return rmax * std::sqrt(ret);
		}
		else
		{
			R ret = R();

			vector_foreach_do(
				ret += Get<i>()*Get<i>(),
				ret += operator[](i)*operator[](i)
			)

			return std::sqrt(ret);
		}
	}
	
	////////////////////////////////////////////////
	/// \brief Euclidian norm without the third coordinate
	/// \tparam R The type to calculate the length in
	/// \tparam Simplify If true, will divide the values to keep them in [-1, 1] before calculating their squares
	/// \returns The Euclidian norm of the vector (with ```z=0```)
	////////////////////////////////////////////////
	template <class R = T, bool Simplify = true>
	[[nodiscard("Arithmetic expression")]] vector_inline constexpr R Length2D() const noexcept
	{
		if constexpr (Simplify == true)
		{
			const T max = Find<decltype([]<size_type i>(const Vector& vec, const T& r)
			{
				if constexpr (S.NoCE)
				{ return std::abs(r) > std::abs(vec.operator[](i)); }
				else
				{ return std::abs(r) > std::abs(vec.template Get<i>()); }
			}), N-1>();

			if (max == T()) [[unlikely]]
				return R();

			const R rmax = static_cast<R>(max);
			R ret = R();

			R x;
			vector_foreach_do_v(
				__expand(x = static_cast<R>(std::abs(Get<i>())) / rmax, ret += x*x),
				__expand(x = static_cast<R>(std::abs(operator[](i))) / rmax, ret += x*x),
			false, N-1, __expand(&, this));

			return rmax * std::sqrt(ret);
		}
		else
		{
			R ret = R();

			vector_foreach_do_v(
				ret += Get<i>()*Get<i>(),
				ret += operator[](i)*operator[](i),
			false, N-1, __expand(&, this))

			return std::sqrt(ret);
		}
	}

	////////////////////////////////////////////////
	/// \brief Manhattan norm (Norm 1)
	/// \returns The Manhattan norm of the vector
	////////////////////////////////////////////////
	[[nodiscard("Arithmetic expression")]] vector_inline constexpr T Manhattan() const noexcept
	{
		T ret = T();

		vector_foreach_do(
			ret += std::abs(Get<i>()),
			ret += std::abs(operator[](i))
		)

		return ret;
	}

	////////////////////////////////////////////////
	/// \brief Norm of the vector
	/// \tparam Norm The norm's index
	/// \tparam R The type to calculate the length in
	/// \tparam Simplify If true, will divide the values to keep them in [-1, 1] before calculating their squares
	/// \returns The norm of the vector
	////////////////////////////////////////////////
	template <size_type NormID = 2, class R = T, bool Simplify = true>
	[[nodiscard("Arithmetic expression")]] vector_inline constexpr auto Norm() const noexcept
	{
		if constexpr (NormID == 1)
		{
			return Manhattan<T>();
		}
		else if constexpr (NormID == 2)
		{
			return Length<R, Simplify>();
		}
		else
		{
			if constexpr (Simplify == true)
			{
				const T max = Find<decltype([]<size_type i>(const Vector& vec, const T& r)
				{
					if constexpr (S.NoCE)
					{ return std::abs(r) > std::abs(vec.operator[](i)); }
					else
					{ return std::abs(r) > std::abs(vec.template Get<i>()); }
				})>();

				if (max == T()) [[unlikely]]
					return R();

				const R rmax = static_cast<R>(max);
				R ret = R();

				vector_foreach_do_v(
					__expand(ret += std::pow(static_cast<R>(std::abs(Get<i>())) / rmax, NormID)),
					__expand(ret += std::pow(static_cast<R>(std::abs(operator[](i))) / rmax, NormID)),
				false, N, __expand(&, this))

				return rmax * std::pow(ret, static_cast<R>(1)/static_cast<R>(NormID));
			}
			else
			{
				R ret = R();

				vector_foreach_do_v(
					ret += Get<i>()*Get<i>(),
					ret += operator[](i)*operator[](i),
				false, N-1, __expand(&, this))

				return std::sqrt(ret);
			}
		}
	}
#endif // VECTOR_USE_STD

	////////////////////////////////////////////////
	/// \brief Squared Euclidian norm
	/// \tparam R The type to calculate the length in
	/// \returns The squared Euclidian norm of the vector
	////////////////////////////////////////////////
	template <class R = T>
	[[nodiscard("Arithmetic expression")]] vector_inline constexpr R LengthSqr() const noexcept
	{
		R ret = R();

		vector_foreach_do(
			ret += Get<i>()*Get<i>(),
			ret += operator[](i)*operator[](i)
		)

		return ret;
	}

	////////////////////////////////////////////////
	/// \brief Dot product
	/// \param v1 The first vector
	/// \param v2 The second vector
	/// \returns The dot product ```v dot u```
	////////////////////////////////////////////////
	template <class _T, template <class, size_type> class _VectorBase>
	[[nodiscard("Arithmetic expression")]] friend vector_inline constexpr auto Dot(const Vector& v1, const Vector<_T, N, _VectorBase, S>& v2) noexcept
	{
		using R = typename MPDetail::lambda_traits<decltype([](const T& x, const _T& y) { return x*y; })>::return_type;
		R ret = R();

		vector_foreach_do_v(
			ret += v1.template Get<i>() * v2.template Get<i>(),
			ret += v1.operator[](i) * v2.operator[](i),
		false, N, &)

		return ret;
	}
	//}}}
private:
	/** @cond */
	template <size_type i, class U, class... Us>
	vector_inline constexpr void ctor_impl(U&& u, Us&&... us) noexcept
	{
		base_type::template Get<i>() = MPDetail::forward<U>(u);
		ctor_impl<i+1>(us...);
	}
	template <size_type i, class U>
	vector_inline constexpr void ctor_impl(U&& u) noexcept
	{
		base_type::template Get<i>() = MPDetail::forward<U>(u);
	}
	/** @endcond */
public:

	////////////////////////////////////////////////
	/// \brief Constructor
	/// \param ts The N values to construct the vector with
	////////////////////////////////////////////////
	template <class... Ts>
	vector_inline explicit constexpr Vector(Ts&&... ts) noexcept
	{
		static_assert(sizeof...(Ts) == N, "Invalid number of arguments");

		if constexpr (S.NoCE)
		{
			size_type i = 0;
			((base_type::operator[](i++) = MPDetail::forward<Ts>(ts)), ...);
		}
		else
		{
			ctor_impl<0>(ts...);
		}
	}

}; // Vector

}; // MPVector


/*
#ifdef VECTOR_USE_FMT
template <class size_type, class T, size_type N, template <class, size_type> class VectorBase, MPVector<size_type>::Settings S>
struct fmt::formatter<MPVector<size_type>::Vector<T, N, VectorBase<T, N>, S>>
{
//TODO...
};
#endif // VECTOR_USE_FMT
*/

#undef __expand
#undef ASSERT_HERE
#undef vector_inline
#undef vector_foreach_do_v
#undef vector_foreach_do

#undef VECTOR_USE_STD
#undef VECTOR_USE_FMT

#endif // VECTOR_HPP
