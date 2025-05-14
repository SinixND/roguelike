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
badd +546 src/App/Game/Game.cpp
badd +2 src/App/Game/systems/AISystem.h
badd +45 src/App/Game/systems/AISystem.cpp
badd +234 src/App/Game/systems/ActionSystem.cpp
badd +67 src/App/Game/systems/MoveSystem.cpp
badd +10 src/App/Game/systems/PathSystem.cpp
badd +7 ~/roguelike/src/App/Game/systems/ActionSystem.h
badd +34 src/App/Game/systems/CollisionSystem.cpp
badd +369 src/App/Game/systems/PathfinderSystem.cpp
badd +14 src/App/Game/systems/AttackSystem.cpp
badd +5 src/App/Game/systems/PathSystem.h
badd +211 ~/roguelike/src/App/Game/entities/Hero.cpp
argglobal
%argdel
edit src/App/Game/Game.cpp
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
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
exe 'vert 1resize ' . ((&columns * 106 + 106) / 212)
exe 'vert 2resize ' . ((&columns * 105 + 106) / 212)
argglobal
balt src/App/Game/systems/AISystem.cpp
setlocal foldmethod=indent
setlocal foldexpr=vimtex#fold#level(v:lnum)
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=99
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal nofoldenable
484
sil! normal! zo
524
sil! normal! zo
530
sil! normal! zo
545
sil! normal! zo
let s:l = 546 - ((31 * winheight(0) + 28) / 57)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 546
normal! 017|
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
let s:l = 197 - ((1 * winheight(0) + 28) / 57)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 197
normal! 030|
wincmd w
exe 'vert 1resize ' . ((&columns * 106 + 106) / 212)
exe 'vert 2resize ' . ((&columns * 105 + 106) / 212)
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
