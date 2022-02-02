/// The SourceLocation class represents the location in the source code.
/// It includes filename, function name and line number.
///
/// SourceLocation can be used as a function's default parameter to
/// automatically get the location of the function call.
///
/// # Examples
///
/// ```
/// void log(SourceLocation loc = SourceLocation::current())
/// {
///     std::printf(%s:%s:%d, loc.file(), loc.function(), loc.line());
/// }
///
/// int main()
/// {
///     log(); // prints main.cpp:main:8
/// }
/// ```

#ifndef OBC_SOURCE_LOCATION_HPP
#define OBC_SOURCE_LOCATION_HPP

namespace obc {

class SourceLocation {
    const char* file_;
    const char* function_;
    int line_;

   public:
    constexpr SourceLocation() : file_{"unknown"}, function_{file_}, line_{0} {}

    static constexpr SourceLocation current(
        const char* file = __builtin_FILE(),
        const char* function = __builtin_FUNCTION(),
        int line = __builtin_LINE())
    {
        SourceLocation loc;
        loc.file_ = file;
        loc.function_ = function;
        loc.line_ = line;
        return loc;
    }

    constexpr const char* file() const { return file_; }
    constexpr const char* function() const { return function_; }
    constexpr int line() const { return line_; }
};

}  // namespace obc

#endif
