<prog> ::= PROGRAM <prog-name> VAR <dec-list> BEGIN <stmt-list> END.
<prog-name> ::= id
<dec-list> ::= <dec> | { ; <dec> }
<dec> ::= <id-list> : <type>
<type> ::= INTEGER
<id-list> ::= id | { , id }
<stmt-list> ::= <stmt> | { ; <stmt> }
<stmt> ::= <assign> | <read> | <write> | <for>
<assign> ::= id := <exp>
<exp> ::= <term> { +- <term> }
<term> ::= <factor> { / DIV <factor> }
<factor> ::= id | int | ( <exp> )
<read> ::= READ ( <id-list> )
<write> ::= WRITE ( <id-list> )
<for> ::= FOR <index-exp> DO <body>
<index-exp> ::= id := <exp> TO <exp>
<body> ::= <stmt> | BEGIN <stmt-list> END
