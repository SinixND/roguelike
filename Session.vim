let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/roguelike
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +534 src/App/Game/Game.cpp
badd +57 ~/roguelike/src/App/Game/systems/ActionSystem.cpp
badd +142 src/App/Game/systems/AISystem.cpp
badd +0 ~/roguelike/src/App/Game/entities/Hero.h
argglobal
%argdel
edit src/App/Game/systems/AISystem.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd _ | wincmd |
split
1wincmd k
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe 'vert 1resize ' . ((&columns * 86 + 86) / 173)
exe '2resize ' . ((&lines * 18 + 20) / 40)
exe 'vert 2resize ' . ((&columns * 86 + 86) / 173)
exe '3resize ' . ((&lines * 18 + 20) / 40)
exe 'vert 3resize ' . ((&columns * 86 + 86) / 173)
argglobal
setlocal foldmethod=indent
setlocal foldexpr=vimtex#fold#level(v:lnum)
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
30
normal! zo
39
normal! zo
107
normal! zo
let s:l = 142 - ((23 * winheight(0) + 18) / 37)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 142
normal! 041|
wincmd w
argglobal
if bufexists(fnamemodify("src/App/Game/systems/AISystem.cpp", ":p")) | buffer src/App/Game/systems/AISystem.cpp | else | edit src/App/Game/systems/AISystem.cpp | endif
if &buftype ==# 'terminal'
  silent file src/App/Game/systems/AISystem.cpp
endif
balt src/App/Game/Game.cpp
setlocal foldmethod=indent
setlocal foldexpr=vimtex#fold#level(v:lnum)
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal nofoldenable
let s:l = 89 - ((7 * winheight(0) + 9) / 18)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 89
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("~/roguelike/src/App/Game/entities/Hero.h", ":p")) | buffer ~/roguelike/src/App/Game/entities/Hero.h | else | edit ~/roguelike/src/App/Game/entities/Hero.h | endif
if &buftype ==# 'terminal'
  silent file ~/roguelike/src/App/Game/entities/Hero.h
endif
balt ~/roguelike/src/App/Game/systems/ActionSystem.cpp
setlocal foldmethod=indent
setlocal foldexpr=vimtex#fold#level(v:lnum)
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal nofoldenable
let s:l = 40 - ((8 * winheight(0) + 9) / 18)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 40
normal! 0
wincmd w
exe 'vert 1resize ' . ((&columns * 86 + 86) / 173)
exe '2resize ' . ((&lines * 18 + 20) / 40)
exe 'vert 2resize ' . ((&columns * 86 + 86) / 173)
exe '3resize ' . ((&lines * 18 + 20) / 40)
exe 'vert 3resize ' . ((&columns * 86 + 86) / 173)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
set hlsearch
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
