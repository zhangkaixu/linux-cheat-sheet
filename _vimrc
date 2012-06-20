syntax on
set backspace=indent,eol,start
set nocp
set ts=4
set is
set smarttab
set shiftwidth=4
set autoindent
set expandtab
set number
set hlsearch
autocmd FileType make setlocal noexpandtab

filetype plugin on
filetype indent on


color torte

map <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q --languages=c++ .<CR>

"hehe
let mapleader = ";"
map <leader>tl :TlistToggle<CR>
let Tlist_Exit_OnlyWindow=1
map <leader>fl :NERDTree<CR>


"latex
set shellslash
set grepprg=grep\ -nH\ $*
let g:tex_flavor='latex'
set iskeyword+=:



