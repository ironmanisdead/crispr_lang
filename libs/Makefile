PHON:=headers clean reset scrape unmake remake .reset install uninstall
.DEFAULT_GOAL:=all
not=$(if $(1),,true)
include .extra/rulemk/helper.mk
sources=$(wildcard *.$(C_EXT))
mk2:=$(wildcard *.mk.2)
isdep:=$(call not,$(filter nodep,$(MAKECMDGOALS)))

ifneq ($(if $(MAKECMDGOALS),$(and $(filter-out $(PHON),$(MAKECMDGOALS)),$(isdep)),true),)
 include $(patsubst %.$(C_EXT),.%.mk,$(wildcard *.$(C_EXT)))
endif

headers:
	.extra/depcheck
	$(if $(filter nodown,$(MAKECMDGOALS)),,cd headers && make All.$(H_EXT))

.PHONY: all nodep nodown $(PHON)

all: .shared.so .static.a

nodep: #means makefile won't generate dependencies for objects

nodown: #means makefile won't descend into another directory

include: .include #makes include symlink
	.extra/symmake

.include:
	mkdir .include
	ln -sfv ../headers .include/$(current_head)

install: headers
	.extra/install
	MAKELEVEL=$(MAKELEVEL) make nodown all

uninstall:
	.extra/uninstall

.%.mk: %.$(C_EXT) | headers include
	.extra/depgen $*

.shared.so: $(patsubst %.$(C_EXT),.%.o,$(sources))
	$(if $^,$(CC) -shared -Wl$(comma)-z$(comma)defs -o .shared.so $^ $(LDLIBS) $(EXLIBS))

.static.a: $(patsubst %.$(C_EXT),.%.o,$(sources))
	$(if $(wildcard .static.a),rm .static.a)
	$(if $^,ar rcs .static.a $^)

scrape:
	$(if $(wildcard .shared.so),rm .shared.so)
	$(if $(wildcard .static.a),rm .static.a)
	$(if $(wildcard .*.o),rm .*.o)
	$(if $(wildcard .*.mk),rm .*.mk)

reset: scrape
	cd headers && make reset
	MAKELEVEL=$(MAKELEVEL) make nodown all

clean: scrape
	cd headers && make clean

remake: clean
	MAKELEVEL=$(MAKELEVEL) make nodown all
