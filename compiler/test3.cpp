$stmt< int `name(x`) = 12; >$;

//$expr< 5 >$;
//$expr< 5.4 >$;
//$expr< true >$;
//$expr< false >$;
//$expr< x >$;
//$expr< 1 + 2 - 3 * 4 / 5 >$;
//$expr< -x >$;
//$expr< !x >$;
//$expr< x[2] >$;
//$expr< x = 42 >$;
//$expr< x[2] = 42 >$;
//$expr< &x >$;
//$expr< *x >$;
//$expr< f() >$;
//$expr< f(5) >$;
//$expr< f(1,2,3,4,5) >$;
//$expr< `f () >$;
//$stmt< 5; >$;
//$stmt< int x; >$;
//$stmt< int x = 5; >$;
//$stmt< int* x; >$; //FIX!!!
//$stmt< int** x; >$ //FIX!!!
//$stmt< double x; >$;
//$stmt< double* x; >$; //FIX!!!
//$stmt< matrix m; >$; //FIX!!!
//$expr< int x >$; //IGNORED - Should give parse error
//$expr< int x; >$;//IGNORED - Should give parse error
//$stmt< x = 1; >$;
//$stmt< {} >$;
//$stmt< {int x;} >$;
//$stmt< {int x; x = 2;} >$;
//$stmt< {int x; x = 2;} foo(); >$;
//$stmt< if(x) x=1; >$;
//$stmt< if(c) {x=1;} else x=2;>$;
//$stmt< while(c) f(); >$;
//$stmt< for(int i=0; i<5; i++) x=i; >$;


//$stmt< foo(); bar(); bee(); zee(1,2,4,`expr(a`)); >$;
//$method< int foo() { } 
//int bar() {}>$;
