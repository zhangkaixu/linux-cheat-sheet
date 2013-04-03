set encoding=utf-8
set langmenu=zh_CN.UTF-8
language message zh_CN.UTF-8

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
set showtabline=2
autocmd FileType make setlocal noexpandtab

filetype plugin on
filetype indent on


color torte
hi Normal ctermbg=NONE



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



map <F8> :w <CR>
imap <F8> <ESC> :w<CR>
map <F9> :call Save_And_Make()<CR>
imap <F9> <ESC> :call Save_And_Make()<CR>

func! Save_And_Make()
    exec "w"
    exec "make"
endfunction
