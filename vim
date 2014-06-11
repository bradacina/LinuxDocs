
Common commands in Vim
=======================
Search
-----
/<text> -> find text
/<text>\c -> \c means case insensitive (can appear anywhere in the pattern)

Buffers
-------
:ls -> list current buffers
C-^ -> switch to alternate buffer
N C-^ -> switch to buffer number N
:e #N -> switch to buffer number N
:bn -> go to next buffer
:bp -> go to prev buffer
:bf -> go to first buffer
:bl -> go to last buffer
:bd -> delete buffer
:b B -> switch to buffer with name B
'%' marks the buffer in the current window
'a' marks a buffer that is active in some window somewhere
'+' marks a buffer that is modified but not saved

Windows
--------
C-W s -> split current window horizontally
C-W v -> split current window vertically
C-W T -> move current window to a new tab
C-W n -> create new window

C-W h,j,k,l or left,right,up,down -> moves you to the window in that direction
C-W W -> move to the window above/left
C-W w -> move to the widnow below/right
C-W p -> move to previous active window
C-W ^ -> split the window and edit alternate buffer
N C-W ^ -> split the window and edit buffer number N

<todo: window resizign and rearenging>

Tabs
-----
:tabedit filename -> opens a new tab to edit the file
:tabclose -> closes the current tab
gt -> go to next tab
:tabn -> go to next tab
gT -> go to prev tab
:tabp -> go to prev tab
N gt -> go to tab number

Misc
----
:echo @% -> echo the directory where the file resides
:ec @% -> echo the directory where the file resides
:cd <dir> -> set the working directory
:cd %:h -> set the working directory to the directory where the file being edited resides
:pwd -> print working directory

NetRW
========
Allows browsing of files and folders on either local machine or remote machine
(preview, editing, etc. )

start vim with a folder path (eg. vim . , or vim /some/path )
:Exp -> explores current working directory
:Exp <path> -> explores the given path


CtrlP plugin
=================
Installation
------------
cd ~/.vim
git clone https://github.com/kien/ctrlp.vim.git bundle/ctrlp.vim

in ~/.vimrc add:
set runtimepath^=~/.vim/bundle/ctrlp.vim

in vim run:
 :helptags ~/.vim/bundle/ctrlp.vim/doc

restart vim and run:
 :help ctrlp.txt
