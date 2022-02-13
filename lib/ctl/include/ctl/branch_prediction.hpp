#ifndef CTL_BRANCH_PREDICTION_HPP
#define CTL_BRANCH_PREDICTION_HPP

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CTL_LIKELY(x) __builtin_expect((x), 1)

// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define CTL_UNLIKELY(x) __builtin_expect((x), 0)

#endif