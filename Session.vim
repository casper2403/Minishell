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
badd +109 src/builtin/builtin_export.c
badd +13 src/args_counting.c
badd +112 src/executor.c
badd +1 src/executor_utils.c
badd +35 src/free_functions.c
badd +84 src/main.c
badd +119 src/parser.c
badd +1 src/parser_utils.c
badd +1 src/process_redirection.c
badd +1 src/redirections.c
badd +1 src/signals.c
badd +1 src/split_arguments.c
badd +1 src/split_arguments_utils.c
badd +1 src/split_input.c
badd +1 src/split_input_utils.c
badd +79 src/minishell.h
badd +1 src/builtin/builtin_cd.c
badd +1 src/builtin/builtin_cd_utils.c
badd +1 src/builtin/builtin_echo.c
badd +1 src/builtin/builtin_env.c
badd +1 src/builtin/builtin_exit.c
badd +1 src/builtin/builtin_pwd.c
badd +1 src/builtin/builtin_unset.c
argglobal
%argdel
$argadd src/builtin/builtin_cd.c
$argadd src/builtin/builtin_cd_utils.c
$argadd src/builtin/builtin_echo.c
$argadd src/builtin/builtin_env.c
$argadd src/builtin/builtin_exit.c
$argadd src/builtin/builtin_export.c
$argadd src/builtin/builtin_pwd.c
$argadd src/builtin/builtin_unset.c
edit src/executor.c
argglobal
if bufexists(fnamemodify("src/executor.c", ":p")) | buffer src/executor.c | else | edit src/executor.c | endif
if &buftype ==# 'terminal'
  silent file src/executor.c
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
let s:l = 211 - ((68 * winheight(0) + 59) / 119)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 211
normal! 064|
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
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
