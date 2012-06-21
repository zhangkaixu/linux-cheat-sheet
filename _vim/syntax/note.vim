syntax match Comment /^>>.*/
syntax match Cursor /#[^\ ]*/
syntax match Special /\[[^\]]*\]/
syntax match Underlined /\[\[[^\]]*\]\]/
syntax match Identifier /“[^”]*”/
syntax match Type /[^\ :]\+:/
syntax match Label /《[^》]*》/
syntax match ColorColumn /:\d\{4}[年/\-]\d\{2}[月\/\-]\d\{2}[日]\?/
syntax match Number /\d\{4}[年/\-]\d\{2}[月\/\-]\d\{2}[日]\?/

map <leader>date :r! date +"\%Y-\%m-\%d"<CR>