" Crispr PAL (Portable Assembly Language) syntax file

"Quit if another custom crispr_pal syntax is loaded
if exists("b:current_syntax")
	finish
endif

"ToDo Comments
syntax match CrisprTodo "\v<(TODO|FIXME|NOTE|XXX)>:{,1}" contained

syntax keyword CrisprOp noop lock stop halt 
			\mov jmp put pop frame 
			\ret cal call

syntax keyword CrisprHeap glo fam loc contained

syntax keyword CrisprOp heap nextgroup=CrisprHeap skipwhite

syntax match CrisprReg "%\vr[gcl][0-5]"
syntax match CrisprDec "$\v<(0|[1-9]\d*)>\ze($|[^.])"
syntax match CrisprFloat "$\v<\d+\.\d+>"
syntax match CrisprHex "$\v<0x[0-9a-fA-F]+>"
syntax match CrisprOct "$\v<0[0-7]+>"

syntax region CrisprComment start=/\/\// end=/\n/ contains=CrisprTodo
syntax region CrisprComment start=/\/\*/ end=/\*\// contains=CrisprTodo

highlight default link CrisprOp Operator
highlight default link CrisprDec Number
highlight default link CrisprFloat Number
highlight default link CrisprHex Number
highlight default link CrisprOct Number
highlight default link CrisprComment Comment
highlight default link CrisprReg Special
highlight default link CrisprHeap Special
highlight default link CrisprTodo Todo
