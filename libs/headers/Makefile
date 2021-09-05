.PHONY: All.$(H_EXT) clean reset

All.$(H_EXT):
	.extra/depcheck

clean:
	$(if $(wildcard All.$(H_EXT)),rm All.$(H_EXT))
	$(if $(wildcard .dirhash),rm .dirhash)

reset: clean All.$(H_EXT)
