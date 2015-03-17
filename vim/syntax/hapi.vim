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

syn match   hapiLabel "\^\k*\|#\k*"
syn match   hapiLTypeVar "%\k\+"
syn match   hapiVar "[a-z]\k*"
syn region  hapiString	start=+"+ skip=+\\\\\|\\"\|\\$+ end=+"+ end='$'
syn match   hapiOperation "->\|<=\|>=\|=\|+\|-\|\*\|/\|and\|or\|not\|&\|<<\|>>\|:"
syn match   hapiProcVar "[A-Z]\k*"
syn match   hapiGTypeVar "\$\k\+"
syn match   hapiValue "true\|false\|[0-9]\+"
syn match   hapiSymbol "(\|)\|\[\|]\|{\|}\|;\||"
syn keyword hapiKeyword new rec global local sync guisync guivalue if then else where as pure of
syn match   hapiMacro "^#.*"
syn keyword hapiSType Bool Int String
syn match   hapiComment  "//.*"

" Assign coloration
hi link hapiKeyword           BlueFace
hi link hapiSymbol            BlueFace
hi link hapiValue             DarkBlueFace
hi link hapiString            DarkBlueFace
hi link hapiOperation         BlueFace
hi link hapiProcVar           CyanFace
hi link hapiSType             GreenFace
hi link hapiGTypeVar          YellowFace
hi link hapiLTypeVar          BrownFace
hi link hapiLabel             DarkBlueFace
hi link hapiComment           GreyFace
hi link hapiVar               CyanFace
hi link hapiMacro             RedFace

" Set the current syntax name
let b:current_syntax = "hapi"
