syn clear

hi DarkBlueFace term=NONE cterm=NONE ctermfg=DarkBlue  ctermbg=NONE gui=NONE guifg=DarkBlue  guibg=NONE
hi GreenFace    term=NONE cterm=NONE ctermfg=Green     ctermbg=NONE gui=NONE guifg=Green     guibg=NONE
hi YellowFace   term=NONE cterm=NONE ctermfg=Yellow    ctermbg=NONE gui=NONE guifg=Yellow    guibg=NONE
hi BlueFace     term=NONE cterm=NONE ctermfg=Blue      ctermbg=NONE gui=NONE guifg=Blue      guibg=NONE
hi RedFace      term=NONE cterm=NONE ctermfg=Red       ctermbg=NONE gui=NONE guifg=Red       guibg=NONE
hi BrownFace    term=NONE cterm=NONE ctermfg=Brown     ctermbg=NONE gui=NONE guifg=Brown     guibg=NONE
hi MagentaFace  term=NONE cterm=NONE ctermfg=Magenta   ctermbg=NONE gui=NONE guifg=Magenta   guibg=NONE
hi CyanFace     term=NONE cterm=NONE ctermfg=Cyan      ctermbg=NONE gui=NONE guifg=Cyan      guibg=NONE
hi GreyFace     term=NONE cterm=NONE ctermfg=Grey      ctermbg=NONE gui=NONE guifg=Grey      guibg=NONE

syn match   apimsLabel "\^\k*\|#\k*"
syn match   apimsLTypeVar "%\k\+"
syn match   apimsVar "[a-z]\k*"
syn region  apimsString	start=+"+ skip=+\\\\\|\\"\|\\$+ end=+"+ end='$'
syn match   apimsOperation "->\|<=\|>=\|=\|+\|-\|\*\|/\|and\|or\|not\|&\|<<\|>>\|:"
syn match   apimsProcVar "[A-Z]\k*"
syn match   apimsGTypeVar "\$\k\+"
syn match   apimsValue "true\|false\|[0-9]\+"
syn match   apimsSymbol "(\|)\|\[\|]\|{\|}\|;\||"
syn keyword apimsKeyword new rec global local sync guisync guivalue if then else where as pure of
syn match   apimsMacro "^#.*"
syn keyword apimsSType Bool Int String
syn match   apimsComment  "//.*"

" Assign coloration
hi link apimsKeyword           BlueFace
hi link apimsSymbol            BlueFace
hi link apimsValue             DarkBlueFace
hi link apimsString            DarkBlueFace
hi link apimsOperation         BlueFace
hi link apimsProcVar           CyanFace
hi link apimsSType             GreenFace
hi link apimsGTypeVar          YellowFace
hi link apimsLTypeVar          BrownFace
hi link apimsLabel             DarkBlueFace
hi link apimsComment           GreyFace
hi link apimsVar               CyanFace
hi link apimsMacro             RedFace

" Set the current syntax name
let b:current_syntax = "apims"
