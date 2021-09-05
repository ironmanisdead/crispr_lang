# Crispr-lang: integrated tools for C11 on linux and windows

## Overview:
  Crispr-lang is a language-in-progress that integrates multithreading and networking cross-platform

### Requirements:
- A working GNU-compatible system
- Git
- GNU Make
- bash
- GCC or clang (with clang extra steps are required. [See here.](#custom-installation))

### Installation:
  To install cextensions, use `make install`, and it should install everything into /usr/local/lib64. Note that sudo permissions may be required to install into that directory.

### Custom Installation
  To install into a different directory, run with <pre>make INSTALLDIR=*directory* install</pre> or to use a custom compiler use <pre>make CC=*compiler* install</pre>
