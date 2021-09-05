.DEFAULT_GOAL:=libs
not=$(if $(1),,true)
targetfile=$*
include .extra/rulemk/helper.mk
ifneq ($(findstring /$(shell pwd),/$(installabs)),)
 $(error cannot install lib$(current_lib) inside source directory)
endif

PHON:=libs remake unmake scrape clean reset install uninstall deps
.PHONY:tests nodep nodown $(PHON)
SOURCES:=$(wildcard *.$(C_EXT))
RULES:=$(patsubst %.$(C_EXT),.%.mk,$(SOURCES))
OBJECTS:=$(patsubst %.$(C_EXT),%.o,$(SOURCES))
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))

ifneq ($(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),)
 no:=$(shell .extra/namecheck)
 ifneq ($(.SHELLSTATUS),0)
  $(error Makefile detected an invalid name)
 endif
 include $(RULES)
endif

libs:
	.extra/depcheck
	$(if $(filter nodown,$(MAKECMDGOALS)),,cd libs && make)

libs/include:
	cd libs && make include

install:
	.extra/install

uninstall:
	.extra/uninstall

tests: $(OBJECTS)

nodep: #means make will not generate dependencies

nodown: #means make will not descend into subdirectories to remake things

.%.mk: | libs/include
	.extra/depgen

unmake: #removes all test object files
	.extra/rulemk/rmobj

remake: unmake #remakes all test object files
	MAKELEVEL=$(MAKELEVEL) make tests

scrape: unmake #removes all test object files and also generated rules
	.extra/rulemk/rmgen

clean: scrape
	cd libs && make clean

reset: scrape
	cd libs && make reset
	MAKELEVEL= make nodown tests
