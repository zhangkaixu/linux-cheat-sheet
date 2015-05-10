"set encoding=utf-8
"set langmenu=zh_CN.UTF-8
"language message zh_CN.UTF-8

syntax on
set backspace=indent,eol,start
set nocp
set ts=4 sw=4 sts=4 tw=100
set is
set smarttab
set shiftwidth=4
set autoindent
set expandtab
set number
set hlsearch
"set showtabline=2
autocmd FileType make setlocal noexpandtab

filetype plugin on
filetype indent on


color torte
hi Normal ctermbg=NONE


set statusline=%F\ [%{&fenc}\ %{&ff}\ L%l/%L\ C%c]\ %=%{strftime('%Y-%m-%d\ %H:%M')}
set laststatus=2

map <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q --languages=c++ .<CR>

"hehe
let mapleader = ";"
map <leader>tl :TlistToggle<CR>
let Tlist_Exit_OnlyWindow=1
map <leader>fl :NERDTree<CR>



map <F8> :w <CR>
imap <F8> <ESC> :w<CR>
map <F9> :call Save_And_Make()<CR>
imap <F9> <ESC> :call Save_And_Make()<CR>
map <F10> :call Save_And_MakeB()<CR>
imap <F10> <ESC> :call Save_And_MakeB()<CR>

"tab
map <space> gt
map <backspace> gT
map <leader><space> <c-w>w

" quick higglight
map <leader>h *<c-o>

func! Save_And_Make()
    exec "w"
    exec "make"
endfunction

func! Save_And_MakeB()
    exec "w"
    exec "make -B"
endfunction
