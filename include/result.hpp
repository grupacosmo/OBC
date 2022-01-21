#ifndef OBC_RESULT_HPP
#define OBC_RESULT_HPP

#include <utility>

#include "branch_prediction.hpp"
#include "error.hpp"

namespace obc {

struct Unit {
};

template <typename T>
struct Ok {
    T value;
};

template <typename T>
Ok(T) -> Ok<T>;

template <typename E>
struct Err {
    E value;
};

template <typename E>
Err(E) -> Err<E>;

template <typename T, typename E>
class Result {
    union {
        Ok<T> ok;
        Err<E> err;
    };
    bool is_ok_;

   public:
    Result(Ok<T>&& ok) : ok{std::move(ok)}, is_ok_{true} {}
    Result(Err<E>&& err) : err{std::move(err)}, is_ok_{false} {}

    Result(const Result& other) { construct(other); }
    Result(Result&& other) { construct(std::move(other)); }

    Result& operator=(const Result& other)
    {
        assign(other);
        return *this;
    }

    Result& operator=(Result&& other)
    {
        assign(std::move(other));
        return *this;
    }

    ~Result()
    {
        if (is_ok()) { ok.~Ok(); }
        else {
            err.~Err();
        }
    }

    bool is_ok() const { return is_ok_; }
    bool is_err() const { return !is_ok_; }

    T& unwrap() & { return unwrap_impl(*this); }
    T&& unwrap() && { return unwrap_impl(std::move(*this)); }
    const T& unwrap() const& { return unwrap_impl(*this); }
    const T&& unwrap() const&& { return unwrap_impl(std::move(*this)); }

    E& unwrap_err() & { return unwrap_err_impl(*this); }
    E&& unwrap_err() && { return unwrap_err_impl(std::move(*this)); }
    const E& unwrap_err() const& { return unwrap_err_impl(*this); }
    const E&& unwrap_err() const&& { return unwrap_err_impl(std::move(*this)); }

   private:
    template <typename R>
    void construct(R&& other)
    {
        is_ok_ = other.is_ok_;
        if (is_ok_) { new (&ok) auto(std::forward<R>(other).ok); }
        else {
            new (&err) auto(std::forward<R>(other).err);
        }
    }

    template <typename R>
    void assign(R&& other)
    {
        if (other.is_ok_) {
            if (is_ok_) { ok = std::forward<R>(other).ok; }
            else {
                err.~Err();
                new (&ok) auto(std::forward<R>(other).ok);
            }
        }
        else {
            if (is_ok_) {
                ok.~Ok();
                new (&err) auto(std::forward<R>(other).err);
            }
            else {
                err = std::forward<R>(other).err;
            }
        }

        is_ok_ = other.is_ok_;
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