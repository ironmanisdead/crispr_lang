null:=
space:=$(null) $(null)
tab:=$(null)	$(null)
slash:=\$(null)
open:=(
close:=)
colon:=:
semi:=;
comma:=,
hashtag:=$(subst ,,#)
define newline


endef
isundef=$(filter undefined default,$(origin $(1)))
