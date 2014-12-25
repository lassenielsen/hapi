" HAPI filetypes
if exists("did_load_filetypes")                       " HAPI
finish                                                " HAPI
endif                                                 " HAPI
augroup filetypedetect                                " HAPI
au! BufRead,BufNewFile *.mps	setfiletype hapi     " HAPI
au! BufRead,BufNewFile *.pi	setfiletype hapi     " HAPI
augroup END                                           " HAPI
