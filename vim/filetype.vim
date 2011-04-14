" my filetype file
if exists("did_load_filetypes")
finish
endif
augroup filetypedetect
autocmd! BufRead,BufNewFile *.mps,*.mps2		setfiletype apims
augroup END
