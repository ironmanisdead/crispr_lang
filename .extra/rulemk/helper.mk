include .extra/rulemk/auto.mk
include .extra/rulemk/system.mk
ifneq ($(call isundef,installabs),)
 ifneq ($(call isundef,installdir),)
  installdir:=/usr/local
 endif
 installabs:=$(abspath $(installdir))
endif
ifneq ($(call isundef,CC),)
 CC:=gcc
endif
ifneq ($(call isundef,LDFLAGS),)
 LDFLAGS:=-Wl,--disable-new-dtags
endif
ifneq ($(call isundef,OLEVEL),)
 OLEVEL:=3
endif
ifneq ($(call isundef,CFLAGS),)
 CFLAGS:=$(if $(call isundef,EX_flags),,$(EX_flags) )-Wmissing-declarations\
	 -std=$(C_VER) -O$(OLEVEL) -Wall -Wextra -Wpedantic
endif
ifneq ($(call isundef,DEF_lib$(current_lib)),)
 DEF_lib$(current_lib):=-DDLL_EXPORT_LIB_$(current_lib)=1
endif
ifneq ($(call isundef,OFLAGS),)
 OFLAGS:=-fPIC
endif
export installdir
export installabs
export targetfile
export current_lib
export current_head
export CC
export LDFLAGS
export LDLIBS
export EXLIBS
export C_VER
export C_EXT
export H_EXT
export OLEVEL
export CFLAGS
export OFLAGS
export DEF_lib$(current_lib)
