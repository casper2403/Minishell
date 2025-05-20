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
badd +1 src/builtin/builtin_cd.c
badd +38 src/builtin/builtin_cd_utils.c
badd +1 src/args_counting.c
badd +1 src/copyenv_and_getvarname.c
badd +74 src/executor.c
badd +61 src/executor_utils.c
badd +1 src/executor_utils2.c
badd +1 src/executor_utils3.c
badd +1 src/executor_utils4.c
badd +43 src/executor_utils5.c
badd +1 src/free_functions.c
badd +93 src/main.c
badd +1 src/parser.c
badd +1 src/parser_utils.c
badd +1 src/parser_utils2.c
badd +1 src/process_redirection.c
badd +1 src/redirections.c
badd +32 src/signals.c
badd +1 src/split_arguments.c
badd +1 src/split_arguments_utils.c
badd +1 src/split_input.c
badd +1 src/split_input_utils.c
badd +1 src/split_input_utils2.c
badd +195 src/minishell.h
badd +1 src/builtin/builtin_echo.c
badd +1 src/builtin/builtin_env.c
badd +1 src/builtin/builtin_exit.c
badd +1 src/builtin/builtin_export.c
badd +1 src/builtin/builtin_export_utils.c
badd +1 src/builtin/builtin_export_utils2.c
badd +1 src/builtin/builtin_pwd.c
badd +1 src/builtin/builtin_unset.c
argglobal
%argdel
$argadd src/main.c
edit src/builtin/builtin_cd.c
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
wincmd _ | wincmd |
vsplit
2wincmd h
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
exe 'vert 1resize ' . ((&columns * 0 + 95) / 191)
exe 'vert 2resize ' . ((&columns * 94 + 95) / 191)
exe 'vert 3resize ' . ((&columns * 95 + 95) / 191)
argglobal
balt src/builtin/builtin_cd_utils.c
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
let s:l = 65 - ((0 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 65
normal! 031|
wincmd w
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
let s:l = 80 - ((32 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 80
normal! 05|
wincmd w
argglobal
if bufexists(fnamemodify("src/main.c", ":p")) | buffer src/main.c | else | edit src/main.c | endif
if &buftype ==# 'terminal'
  silent file src/main.c
endif
balt src/executor_utils.c
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
let s:l = 94 - ((33 * winheight(0) + 24) / 49)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 94
normal! 016|
wincmd w
3wincmd w
exe 'vert 1resize ' . ((&columns * 0 + 95) / 191)
exe 'vert 2resize ' . ((&columns * 94 + 95) / 191)
exe 'vert 3resize ' . ((&columns * 95 + 95) / 191)
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
