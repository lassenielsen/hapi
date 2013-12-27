" APIMS filetypes
if exists("did_load_filetypes")                       " APIMS
finish                                                " APIMS
endif                                                 " APIMS
augroup filetypedetect                                " APIMS
au! BufRead,BufNewFile *.mps	setfiletype apims     " APIMS
au! BufRead,BufNewFile *.pi	setfiletype apims     " APIMS
augroup END                                           " APIMS
