   
   mkmethod(mkatype(TOKEN: Int (20-22)), "f", mkformals(),
          mkstmtlist(mkexprstmt(mkbinaryop(Equal,
                                           mknameorargslist(mkident("a"),
                                           mklistexpr(mkident("i"))))),
                     mkifstmt(mkbinaryop(Greater, mkident("x")),
                              mkexprstmt(mkfuncall("f", mkexprlist())),
                     mkstmtlist()),
                     h(i)))
