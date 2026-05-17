#!/usr/bin/env python
import os
import sys

env = SConscript("godot-cpp/SConstruct") # type: ignore

env.Append(CPPPATH=["src/", "src/thirdparty/eigen-5.0.0/", "src/thirdparty/nlohmann/","src/thirdparty/mnist-loader/include/"])
if env["target"] == "template_release":
    env.Append(CCFLAGS=["/MT", "/EHsc"])
else:
    env.Append(CCFLAGS=["/MTd", "/EHsc"])

sources = Glob("src/*.cpp") # type: ignore
sources += Glob("src/**/*.cpp") # type: ignore
sources += Glob("src/neural_network/**/*.cpp") # type: ignore
sources += Glob("src//thirdparty/mnist-loader/src/*.cc") # type: ignore

lib_filename = "{}ai_digit_recognizer{}{}".format(env.subst('$SHLIBPREFIX'), env["suffix"], env.subst('$SHLIBSUFFIX'))

library = env.SharedLibrary(
    "project/bin/{}".format(lib_filename),
    source=sources,
)

Default(library) # type: ignore
