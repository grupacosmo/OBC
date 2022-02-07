#ifndef OBC_BRANCH_PREDICTION_HPP
#define OBC_BRANCH_PREDICTION_HPP

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define OBC_LIKELY(x) __builtin_expect((x), 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define OBC_UNLIKELY(x) __builtin_expect((x), 0)

#endif