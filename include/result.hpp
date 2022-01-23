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

// compiler crashed when I tried to overload destructors
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

    ResultBase(Ok<T>&& ok) : ok{std::move(ok)}, is_ok_{true} {}
    ResultBase(Err<E>&& err) : err{std::move(err)}, is_ok_{false} {}

    ~ResultBase() = default;
};

template <typename T, typename E>
struct ResultBase<T, E, false> {
    union {
        Ok<T> ok;
        Err<E> err;
    };
    bool is_ok_;

    ResultBase(Ok<T>&& ok) : ok{std::move(ok)}, is_ok_{true} {}
    ResultBase(Err<E>&& err) : err{std::move(err)}, is_ok_{false} {}

    ~ResultBase()
    {
        if (is_ok_) {
            if constexpr (!std::is_trivially_destructible_v<T>) { ok.~Ok(); }
        }
        else {
            if constexpr (!std::is_trivially_destructible_v<E>) { err.~Err(); }
        }
    }
};

}  // namespace detail

template <typename T, typename E>
class Result : detail::ResultBase<T, E> {
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

    // clang-format on

   public:
    Result(Ok<T>&& ok) : Base{std::move(ok)} {}
    Result(Err<E>&& err) : Base{std::move(err)} {}

    // clang-format off

    Result(const Result& other)
        requires
            is_trivial_copy_constructor
        = default;

    Result(const Result& other)
        requires
            !is_trivial_copy_constructor
            && std::is_copy_constructible_v<Ok<T>>
            && std::is_copy_constructible_v<Err<E>>
    {
        construct(other);
    }

    Result(Result&& other)
        requires
            is_trivial_move_constructor
        = default;

    Result(Result&& other)
        requires
            !is_trivial_move_constructor
            && std::is_move_constructible_v<Ok<T>>
            && std::is_move_constructible_v<Err<E>>
    {
        construct(std::move(other));
    }

    Result& operator=(const Result& other)
        requires
            is_trivial_copy_assignment
        = default;

    Result& operator=(const Result& other)
        requires
            !is_trivial_copy_assignment
            && std::is_copy_assignable_v<Ok<T>>
            && std::is_copy_assignable_v<Err<E>>
    {
        assign(other);
        return *this;
    }

    Result& operator=(Result&& other)
        requires
            is_trivial_move_assignment
        = default;

    Result& operator=(Result&& other)
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
        if (OBC_UNLIKELY(self.is_err())) { OBC_PANIC("unwrap"); }
        return std::forward<Self>(self).ok.value;
    }

    template <typename Self>
    static auto&& unwrap_err_impl(Self&& self)
    {
        if (OBC_UNLIKELY(self.is_ok())) { OBC_PANIC("unwrap_err"); }
        return std::forward<Self>(self).err.value;
    }
};

}  // namespace obc

#endif
