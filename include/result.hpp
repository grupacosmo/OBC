#ifndef OBC_RESULT_HPP
#define OBC_RESULT_HPP

#include <utility>

#include "branch_prediction.hpp"
#include "error.hpp"

namespace obc {

struct Unit {};

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

    // TODO copy and move ctors and assignment operators

    ~Result()
    {
        if (is_ok()) { ok.~Ok(); }
        else { err.~Err(); }
    }

    bool is_ok() const { return is_ok_; }

    bool is_err() const { return !is_ok_; }

    T& unwrap() &
    {
        if (OBC_UNLIKELY(is_err())) { panic(); }
        return ok.value;
    }

    const T& unwrap() const&
    {
        if (OBC_UNLIKELY(is_err())) { panic(); }
        return ok.value;
    }

    T&& unwrap() &&
    {
        if (OBC_UNLIKELY(is_err())) { panic(); }
        return std::move(ok.value);
    }
    
    const T&& unwrap() const&&
    {
        if (OBC_UNLIKELY(is_err())) { panic(); }
        return std::move(ok.value);
    }

    E& unwrap_err() &
    {
        if (OBC_UNLIKELY(is_ok())) { panic(); }
        return err.value;
    }

    const E& unwrap_err() const&
    {
        if (OBC_UNLIKELY(is_ok())) { panic(); }
        return err.value;
    }

    E&& unwrap_err() &&
    {
        if (OBC_UNLIKELY(is_ok())) { panic(); }
        return std::move(err.value);
    }
    
    const E&& unwrap_err() const&&
    {
        if (OBC_UNLIKELY(is_ok())) { panic(); }
        return std::move(err.value);
    }
};

} // namespace obbc

#endif