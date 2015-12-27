set nocompatible " Make Vim behave in a more useful way. If you set this option in your vimrc file, you should probably put it at the very start. For Compatibility.
" Note that creating a vimrc file will cause the 'compatible' option to be off by default.
filetype indent plugin on " Required.
colorscheme koehler
" Section: Options
" ----------------
syntax on " Enable syntax highlighting
syntax enable
set autoindent " Copy indent from current line when starting a new line
set cmdheight=2 " Number of screen lines to use for the command-line. Helps avoiding hit-enter prompts
set complete-=i " Searching includes can be slow ~tpope
set confirm " Instead of failing a command because of unsaved changes
"set cursorline
set encoding=utf-8 " The encoding displayed.
set expandtab " Turns tabs into spaces
set fileencoding=utf-8 " The encoding written to file
set hidden " 
set hlsearch " Highlights words as you search
set ignorecase " The case of normal letters is ignored
set incsearch " While typing a search show where the pattern as it was typed so far matches The matched string is highlighted
set laststatus=2 " Always display the status line, even if only one window is displayed *Airline!
set mouse=a " Enable the use of the mouse. Only works for certain terminals (xterm, MS-DOS, Win32... etc.)
set nostartofline " Stop certain movements from always going to the 1st character of a line, affected gg and G
set number " Display line numbers
set ruler " Show line + col number of the cursor pos separated by comma if there's room, the rel pos of the displayed text in file is shown on far right
set shiftwidth=2 " Indentation settings for using 2 spaces instead of tabs.
set showmatch " When a bracket is inserted, briefly jump to the matching one the jump is only done if the match can be seen on the screen
set showcmd " Show (partial) command in the last line of the screen set this option off if your terminal is slow in visual made shows selected area
set smartcase " Case insensitive searches become sensitive with capitals
set smartindent " Do smart autoindenting when starting a new line
set smarttab " Shiftwidth at the start of the line, softtabstop everywhere else
set softtabstop=2 " Number of spaces that a <Tab> counts for while performing editing operations, like inserting a <Tab> or using <BS>
set splitbelow
set splitright
set t_Co=256
set t_vb= " Disables visual bell entirely
set tabstop=2 " 
set ttimeoutlen=50 " Make Esc work faster, the time in milliseconds that is waited for a key code or mapped key sequence to complete
set ttyfast " Indicates fast terminal connection More chars will be sent to screen for redrawing instead of using ins/del line cmds
set wildmenu " When 'wildmenu' is on, command-line completion operates in an enhanced mode
"set notimeout ttimeout " Quickly time out on keycodes not on mappings (Not sure if I want this)
set backspace=indent,eol,start " Makes backspace work
" Undo with as many levels as you like stored into file to keep undos across sessions
set undodir=~/.vim/undodir
set undofile
