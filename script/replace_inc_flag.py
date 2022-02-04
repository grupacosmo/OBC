# supresses warnings coming from library headers
# by turning them into 'system headers'
# this is done by overriding '-I/path/' flags with '-isystem/path/'

Import("projenv")

include_flags = []
for path in projenv["CPPPATH"]:
    if path != projenv["PROJECT_INCLUDE_DIR"]:
        include_flags.append(["-isystem", path])

projenv.Append(
    CXXFLAGS = include_flags
)