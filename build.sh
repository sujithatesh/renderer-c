#!/bin/bash

code="$PWD"
opts="-g -lwayland-client"
cd . > /dev/null
clang $opts $code/wayland.c $code/xdg-shell-protocol.c -o bar
cd $code > /dev/null
