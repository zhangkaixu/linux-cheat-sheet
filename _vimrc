""" vundle
set nocompatible              " be iMproved, required
filetype off                  " required

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
"Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
"Plugin 'git://git.wincent.com/command-t.git'
" git repos on your local machine (i.e. when working on your own plugin)
"Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
"Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

Plugin 'vim-airline/vim-airline'
Plugin 'vim-airline/vim-airline-themes'

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line


let g:airline_powerline_fonts = 1  " 显式三角形箭头
let g:airline#extensions#tabline#enabled = 1 " 顶上的tabline

let g:airline_theme='bubblegum'

set laststatus=2  "永远显示状态栏
set t_Co=256      "在windows中用xshell连接打开vim可以显示色彩

set encoding=utf-8
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


"doxygen
let g:doxygenToolkit_author="Zhang Kaixu"
let g:load_doxygen_syntax=1

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

map <leader>s :w<CR>
imap jj <ESC>

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


