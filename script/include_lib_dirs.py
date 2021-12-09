# overrides platformio's way of adding directories to be searched for header files
# by default its is done with -I option
# this script changes it to -isystem
# this is done to suppress compiler warnings for library files

Import("env", "projenv")

options = []
for path in env["CPPPATH"]:
    options.append(["-isystem", path])

projenv.Append(
    CXXFLAGS = options
)
