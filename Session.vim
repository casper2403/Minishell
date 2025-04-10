let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/Documents/Minishell
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +1 src/free_functions.c
badd +1 src/main.c
badd +1 src/minishell.h
badd +101 src/process.c
argglobal
%argdel
$argadd src/free_functions.c
$argadd src/main.c
$argadd src/minishell.h
$argadd src/process.c
edit src/main.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
3wincmd h
wincmd w
wincmd w
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
exe 'vert 1resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 2resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 3resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 4resize ' . ((&columns * 119 + 239) / 479)
argglobal
if bufexists(fnamemodify("src/main.c", ":p")) | buffer src/main.c | else | edit src/main.c | endif
if &buftype ==# 'terminal'
  silent file src/main.c
endif
balt src/free_functions.c
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 59) / 119)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/minishell.h", ":p")) | buffer src/minishell.h | else | edit src/minishell.h | endif
if &buftype ==# 'terminal'
  silent file src/minishell.h
endif
balt src/main.c
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 59) / 119)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
wincmd w
argglobal
if bufexists(fnamemodify("src/process.c", ":p")) | buffer src/process.c | else | edit src/process.c | endif
if &buftype ==# 'terminal'
  silent file src/process.c
endif
balt src/minishell.h
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 102 - ((101 * winheight(0) + 59) / 119)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 102
normal! 05|
wincmd w
argglobal
if bufexists(fnamemodify("src/free_functions.c", ":p")) | buffer src/free_functions.c | else | edit src/free_functions.c | endif
if &buftype ==# 'terminal'
  silent file src/free_functions.c
endif
balt src/process.c
setlocal foldmethod=manual
setlocal foldexpr=0
setlocal foldmarker={{{,}}}
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldenable
silent! normal! zE
let &fdl = &fdl
let s:l = 6 - ((5 * winheight(0) + 59) / 119)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 6
normal! 0
wincmd w
3wincmd w
exe 'vert 1resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 2resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 3resize ' . ((&columns * 119 + 239) / 479)
exe 'vert 4resize ' . ((&columns * 119 + 239) / 479)
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
