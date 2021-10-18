" Crispr PAL (Portable Assembly Language) syntax file

"Quit if another custom crispr_pal syntax is loaded
if exists("b:current_syntax")
	finish
endif

"ToDo Comments
syntax keyword CrisprTodo TODO XXX FIXME NOTE

syntax keyword CrisprOp noop lock stop halt
			\mov jmp put pop
			\frame ret heap cal call

syntax match CrisprReg "%\vr[gcl][0-5]"
syntax match CrisprInt "$\v<(0|[1-9]\d*)>\ze($|[^.])"
syntax match CrisprFloat "$\v<\d+\.\d+>"
syntax match CrisprHex "$\v<0x([0-9a-fA-F]|[1-9a-fA-F][0-9a-fA-F]*)>"
syntax match CrisprOct "$\v<0[1-7][0-7]*>"

syntax region CrisprComment start=/\/\// end=/\n/
syntax region CrisprComment start=/\/\*/ end=/\*\//

highlight default link CrisprOp Operator
highlight default link CrisprInt Number
highlight default link CrisprFloat Number
highlight default link CrisprHex Number
highlight default link CrisprOct Number
highlight default link CrisprComment Comment
highlight default link CrisprReg Special
