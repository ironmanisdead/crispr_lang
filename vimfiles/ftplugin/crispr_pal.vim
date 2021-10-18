" Sets settings for Crispr Assembly
if exists("b:did_ftplugin")
	finish
endif

setlocal iskeyword+=?,!,@-@,#
setlocal tabstop=8
setlocal softtabstop=8
setlocal shiftwidth=8
setlocal completefunc=syntaxcomplete#Complete
