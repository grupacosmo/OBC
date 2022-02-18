#ifndef CCL_BRANCH_PREDICTION_HPP
#define CCL_BRANCH_PREDICTION_HPP

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CCL_LIKELY(x) __builtin_expect((x), 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CCL_UNLIKELY(x) __builtin_expect((x), 0)

#endif