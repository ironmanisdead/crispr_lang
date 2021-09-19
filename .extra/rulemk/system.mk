ifneq ($(call isundef,LDLIBS),)
 LDLIBS:=-lc
endif
ifneq ($(call isundef,EXLIBS),)
 EXLIBS:=-lpthread
endif
ifneq ($(call isundef,C_VER),)
 C_VER:=c17
endif
ifneq ($(call isundef,C_EXT),)
 C_EXT:=c
endif
ifneq ($(call isundef,H_EXT),)
 H_EXT:=h
endif
ifneq ($(call isundef,current_lib),)
 current_lib:=crispr_api
endif
ifneq ($(call isundef,current_head),)
 current_head:=CrisprLang
endif
