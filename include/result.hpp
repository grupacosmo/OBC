/// Error handling utility.
///
/// 'Result<T, E>' type is used for returning and handling errors.
/// It is a tagged union that has 2 possible variants:
/// * 'Ok<T>' - representing success and containing a value
/// * 'Err<E>' - representing failure and containing an error value
///
/// The state of a result can be checked with 'is_ok' and 'is_err' methods.
///
/// There are multiple methods that extract the value contained in a
/// Result<T, E>. If the Result is Err then:
/// * 'unwrap' - panics with generic message,
/// * 'expect' - panics with provided message,
/// * 'unwrap_or_else' - returns result of executing provided function.
///
/// 'unwrap_err' may be used to extract contained error value.
///
/// 'Unit' type can be used in place of T for functions that may fail
///  but do not return a value.
///
/// # Examples
///
/// ```
/// Result<int, int> make_result() {
///     if (failure) { return Err{-1}; }
///     return Ok{1};
/// }
/// ```
///
/// ```
/// Result<File, ErrorKind> open_file(const char* path);
/// Result<File, ErrorKind> create_file(const char* path);
///
/// File file = open_file(path).unwrap_or_else([&](ErrorKind kind) {
///     switch (kind) {
///         case ErrorKind::NotFound:
///             return create_file(path).expect("Cannot create file");
///         case ErrorKind::PermissionDenied:
///             panic("Permission denied");
///     }
/// });
/// ```

#ifndef OBC_RESULT_HPP
#define OBC_RESULT_HPP

#include <utility>

#include "branch_prediction.hpp"
#include "error.hpp"

namespace obc {

struct Unit {
};

namespace detail {

template <typename T>
struct Singleton {
    static_assert(!std::is_void_v<T>, "use 'Unit' instead of 'void'");
    T value;
};

}  // namespace detail

template <typename T>
struct Ok : detail::Singleton<T> {
};

template <typename T>
Ok(T) -> Ok<T>;

template <typename E>
struct Err : detail::Singleton<E> {
};

template <typename E>
Err(E) -> Err<E>;

namespace detail {

// compiler crashed when I tried to overload destructors with 'requires'
// so here we are with ugly tricks
template <
    typename T,
    typename E,
    bool = std::is_trivially_destructible_v<T>&&
        std::is_trivially_destructible_v<E>>
struct ResultBase {
    union {
        Ok<T> ok;
        Err<E> err;
    };
    bool is_ok_;

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    ResultBase(Ok<T>&& ok) : ok{std::move(ok)}, is_ok_{true} {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    ResultBase(Err<E>&& err) : err{std::move(err)}, is_ok_{false} {}

    ResultBase(const ResultBase&) = default;
    ResultBase& operator=(const ResultBase&) = default;

    ResultBase(ResultBase&&) noexcept = default;
    ResultBase& operator=(ResultBase&&) noexcept = default;

    ~ResultBase() = default;
};

template <typename T, typename E>
struct ResultBase<T, E, false> {
    union {
        Ok<T> ok;
        Err<E> err;
    };
    bool is_ok_;

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    ResultBase(Ok<T>&& ok) : ok{std::move(ok)}, is_ok_{true} {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    ResultBase(Err<E>&& err) : err{std::move(err)}, is_ok_{false} {}

    ResultBase(const ResultBase&) = delete;
    ResultBase& operator=(const ResultBase&) = delete;

    ResultBase(ResultBase&&) noexcept = delete;
    ResultBase& operator=(ResultBase&&) noexcept = delete;

    ~ResultBase()
    {
        if (is_ok_) {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
            if constexpr (!std::is_trivially_destructible_v<T>) { ok.~Ok(); }
        }
        else {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
            if constexpr (!std::is_trivially_destructible_v<E>) { err.~Err(); }
        }
    }
};

}  // namespace detail

template <typename T, typename E>
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
class [[nodiscard]] Result : detail::ResultBase<T, E> {
    using Base = detail::ResultBase<T, E>;

    // clang-format off

    static constexpr bool is_trivial_copy_constructor =
        std::is_trivially_copy_constructible_v<Ok<T>>
        && std::is_trivially_copy_constructible_v<Err<E>>;

    static constexpr bool is_trivial_move_constructor =
        std::is_trivially_move_constructible_v<Ok<T>>
        && std::is_trivially_move_constructible_v<Err<E>>;

    static constexpr bool is_trivial_copy_assignment = 
        std::is_trivially_copy_assignable_v<Ok<T>>
        && std::is_trivially_copy_constructible_v<Ok<T>>
        && std::is_trivially_destructible_v<Ok<T>>
        && std::is_trivially_copy_assignable_v<Err<E>>
        && std::is_trivially_copy_constructible_v<Err<E>>
        && std::is_trivially_destructible_v<Err<E>>;

    static constexpr bool is_trivial_move_assignment = 
        std::is_trivially_move_assignable_v<Ok<T>>
        && std::is_trivially_move_constructible_v<Ok<T>>
        && std::is_trivially_destructible_v<Ok<T>>
        && std::is_trivially_move_assignable_v<Err<E>>
        && std::is_trivially_move_constructible_v<Err<E>>
        && std::is_trivially_destructible_v<Err<E>>;

   public:
    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Result(Ok<T>&& ok) : Base{std::move(ok)} {}

    // NOLINTNEXTLINE(hicpp-explicit-conversions)
    Result(Err<E>&& err) : Base{std::move(err)} {}

    Result(const Result& other) requires is_trivial_copy_constructor = default;

    Result(const Result& other)
        requires
            !is_trivial_copy_constructor
            && std::is_copy_constructible_v<Ok<T>>
            && std::is_copy_constructible_v<Err<E>>
    {
        construct(other);
    }

    Result(Result&& other) noexcept requires is_trivial_move_constructor
        = default;

    Result(Result&& other) noexcept
        requires
            !is_trivial_move_constructor
            && std::is_move_constructible_v<Ok<T>>
            && std::is_move_constructible_v<Err<E>>
    {
        construct(std::move(other));
    }

    Result& operator=(const Result& other)
        requires is_trivial_copy_assignment = default;

    Result& operator=(const Result& other)
        requires
            !is_trivial_copy_assignment
            && std::is_copy_assignable_v<Ok<T>>
            && std::is_copy_assignable_v<Err<E>>
    {
        assign(other);
        return *this;
    }

    Result& operator=(Result&& other) noexcept
        requires is_trivial_move_assignment = default;

    Result& operator=(Result&& other) noexcept
        requires
            !is_trivial_move_assignment
            && std::is_move_assignable_v<Ok<T>>
            && std::is_move_assignable_v<Err<E>>
    {
        assign(std::move(other));
        return *this;
    }

    // clang-format on

    ~Result() = default;

    bool is_ok() const { return this->is_ok_; }
    bool is_err() const { return !this->is_ok_; }

    T& unwrap() & { return unwrap_impl(*this); }
    T&& unwrap() && { return unwrap_impl(std::move(*this)); }
    const T& unwrap() const& { return unwrap_impl(*this); }
    const T&& unwrap() const&& { return unwrap_impl(std::move(*this)); }

    E& unwrap_err() & { return unwrap_err_impl(*this); }
    E&& unwrap_err() && { return unwrap_err_impl(std::move(*this)); }
    const E& unwrap_err() const& { return unwrap_err_impl(*this); }
    const E&& unwrap_err() const&& { return unwrap_err_impl(std::move(*this)); }

    template <typename F>
    T unwrap_or_else(F f) const&
    {
        return unwrap_or_else_impl(*this, f);
    }

    template <typename F>
    T unwrap_or_else(F f) &&
    {
        return unwrap_or_else_impl(std::move(*this), f);
    }

    T& expect(const char* msg) & { return expect_impl(*this, msg); }

    T&& expect(const char* msg) &&
    {
        return expect_impl(std::move(*this), msg);
    }

    const T& expect(const char* msg) const& { return expect_impl(*this, msg); }

    const T&& expect(const char* msg) const&&
    {
        return expect_impl(std::move(*this), msg);
    }

   private:
    void destroy_ok()
    {
        if constexpr (!std::is_trivially_destructible_v<Ok<T>>) {
            this->ok.~Ok();
        }
    }

    void destroy_err()
    {
        if constexpr (!std::is_trivially_destructible_v<Err<E>>) {
            this->err.~Err();
        }
    }

    template <typename R>
    void construct(R&& other)
    {
        this->is_ok_ = other.is_ok_;
        if (this->is_ok_) { new (&this->ok) auto(std::forward<R>(other).ok); }
        else {
            new (&this->err) auto(std::forward<R>(other).err);
        }
    }

    template <typename R>
    void assign(R&& other)
    {
        if (other.is_ok_) {
            if (this->is_ok_) { this->ok = std::forward<R>(other).ok; }
            else {
                destroy_err();
                new (&this->ok) auto(std::forward<R>(other).ok);
                this->is_ok_ = true;
            }
        }
        else {
            if (this->is_ok_) {
                destroy_ok();
                new (&this->err) auto(std::forward<R>(other).err);
                this->is_ok_ = false;
            }
            else {
                this->err = std::forward<R>(other).err;
            }
        }
    }

    template <typename Self>
    static auto&& unwrap_impl(Self&& self)
    {
        if (OBC_UNLIKELY(self.is_err())) { panic("unwrap"); }
        return std::forward<Self>(self).ok.value;
    }

    template <typename Self>
    static auto&& unwrap_err_impl(Self&& self)
    {
        if (OBC_UNLIKELY(self.is_ok())) { panic("unwrap_err"); }
        return std::forward<Self>(self).err.value;
    }

    template <typename Self, typename F>
    static T unwrap_or_else_impl(Self&& self, F f)
    {
        return self.is_ok() ? std::forward<Self>(self).ok.value
                            : f(std::forward<Self>(self).err.value);
    }

    template <typename Self>
    static auto&& expect_impl(Self&& self, const char* msg)
    {
        if (OBC_UNLIKELY(self.is_err())) { panic(msg); }
        return std::forward<Self>(self).ok.value;
    }
};

}  // namespace obc

#endif
