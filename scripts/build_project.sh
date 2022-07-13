#!/bin/sh
# Copyright (c) 2022 Emmanuel Arias

rm -r generated
./third_party/premake/bin/linux/premake5 --file=premake5.lua gmake
