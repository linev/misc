#!/bin/bash

src=/home/linev/git/root

find $src -name LinkDef.h >files.txt
find $src -name LinkDef1.h >>files.txt
find $src -name LinkDef2.h >>files.txt
find $src -name LinkDef3.h >>files.txt
find $src -name LinkDef4.h >>files.txt
find $src -name LinkDef5.h >>files.txt

