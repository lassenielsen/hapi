syn clear

if &background == "dark"
else
  hi apimsKeywordFace             term=NONE cterm=NONE ctermfg=DarkBlue  ctermbg=NONE gui=NONE guifg=DarkBlue   guibg=NONE
  hi apimsValueFace               term=NONE cterm=NONE ctermfg=DarkGreen ctermbg=NONE gui=NONE guifg=DarkGreen  guibg=NONE
  hi apimsOperationFace           term=NONE cterm=BOLD ctermfg=DarkBlue  ctermbg=NONE gui=BOLD guifg=DarkBlue   guibg=NONE
  hi apimsLabelFace               term=NONE cterm=BOLD ctermfg=DarkGreen ctermbg=NONE gui=BOLD guifg=DarkGreen  guibg=NONE
  hi apimsGTypeFace               term=NONE cterm=NONE ctermfg=Blue      ctermbg=NONE gui=NONE guifg=Purple     guibg=NONE
  hi apimsLTypeFace               term=NONE cterm=NONE ctermfg=Yellow    ctermbg=NONE gui=NONE guifg=Orange     guibg=NONE
  hi apimsDTypeFace               term=NONE cterm=NONE ctermfg=Red       ctermbg=NONE gui=NONE guifg=Red        guibg=NONE
  hi apimsCommentFace             term=NONE cterm=NONE ctermfg=Magenta   ctermbg=NONE gui=NONE guifg=Magenta    guibg=NONE
endif

syn keyword apimsKeyword end nu rec def in and or not link sync guisync guivalue if then else Bool Int String
syn match   apimsValue "[0-9]*\|true\|false"
syn region  apimsString	start=+"+ skip=+\\\\\|\\"\|\\$+ end=+"+ end='$'
syn keyword apimsValueKeyword true false
syn match   apimsOperation "<<\|>>\|((\|))\|=>"
syn match   apimsProcVar "\(\s\|^\)[A-Z]\k*"
syn match   apimsGTypeVar "\$\k*"
syn keyword apimsGType Gend
syn match   apimsLTypeVar "%\k*\|of\|@"
syn keyword apimsLType Lend
syn match   apimsDTypeVar "§\k*"
syn match   apimsLabel "\^\k*\|#\k*"

" Comments hilighting 
"  single line, empty line comments

""syn region apimsValueString  start="\"" end="\"" contains=apimsvalueString 
syn match apimsComment  "//.*"

" Assign coloration
hi link apimsKeyword           apimsKeywordFace
hi link apimsProcVar           apimsKeywordFace
hi link apimsValueKeyword      apimsValueFace
hi link apimsString            apimsValueFace
hi link apimsValue             apimsValueFace
hi link apimsOperation         apimsOperationFace
hi link apimsGTypeVar          apimsGTypeFace
hi link apimsGType             apimsGTypeFace
hi link apimsLTypeVar          apimsLTypeFace
hi link apimsLType             apimsLTypeFace
hi link apimsDTypeVar          apimsDTypeFace
hi link apimsLabel             apimsLabelFace
hi link apimsComment           apimsCommentFace

" Set the current syntax name
let b:current_syntax = "apims"
