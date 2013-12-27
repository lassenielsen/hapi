install:
	mkdir -p ~/.vim/syntax
	cp syntax/apims.vim ~/.vim/syntax/
	touch ~/.vim/filetype.vim
	(grep -v "\" APIMS" ~/.vim/filetype.vim; cat filetype.vim) > local_filetype.vim
	cp local_filetype.vim ~/.vim/filetype.vim

clean:
	rm -f local_filetype.vim

default:
	@echo "Type make install to enable apims syntax highlighting in vim for the current user."
