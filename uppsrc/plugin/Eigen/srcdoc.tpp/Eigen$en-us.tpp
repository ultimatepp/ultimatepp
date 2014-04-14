topic "Eigen";
[ $$0,0#00000000000000000000000000000000:Default]
[a83;*R6 $$1,0#31310162474203024125188417583966:caption]
[{_}%EN-US 
[s1; [+184 Eigen]&]
[s0;# [^http`:`/`/eigen`.tuxfamily`.org`/index`.php`?title`=Main`_Page^2 Eigen 
3][2  is a C`+`+ template library for linear algebra, matrices, 
vectors, numerical solvers and related algorithms.]&]
[s0;#2 &]
[s0;# [2 Eigen is very easy to use. For example, to multiply two matrices:]&]
[s0;#2 &]
[s0;# [C2 -|Matrix2d a, b;]&]
[s0; [C2 -|a << 1, 2,-|-|-|// Fill the data]&]
[s0; [C2 -|-|3, 4;]&]
[s0; [C2 -|b << 5, 6,]&]
[s0; [C2 -|-|7, 8;]&]
[s0; [C2 -|Matrix2d res `= a`*b;-|// Just multiply them using `*]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [2 Eigen package is a wrapper of Eigen 3.1.2 library. It includes 
the library and helper functions to integrate better Eigen with 
U`+`+. Starting from the 3.1.1 version, it is licensed under 
the ][^http`:`/`/www`.mozilla`.org`/MPL`/2`.0`/^2 MPL2][2 , which 
is a simple weak copyleft license. Common questions about the 
MPL2 are answered in the official ][^http`:`/`/www`.mozilla`.org`/MPL`/2`.0`/FAQ`.html^2 M
PL2 FAQ][2 .]&]
[s0;#2 &]
[s0;# [2 Bazaar/Eigen package includes a sample package (Eigen`_demo) 
to ease its use for U`+`+ users. It has many samples from Eigen 
library and nonlinear equation solving and optimization like 
][^http`:`/`/www`.itl`.nist`.gov`/div898`/strd`/nls`/data`/eckerle4`.shtml^2 Eckerl
e4][2  and ][^http`:`/`/www`.itl`.nist`.gov`/div898`/strd`/nls`/data`/thurber`.shtml^2 T
hurber][2  (see the graphs).]&]
[s0;#2 &]
[s0;= [2 
@@image:1272&1104
(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)
][2    ][2 
@@image:1290&1104
(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)
]&]
[s0;#2 &]
[s0;#2 &]
[s0;# [2 Eigen packages have been prepared by ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=647`&^2 d
olik.rce][2  and ][^http`:`/`/www`.ultimatepp`.org`/forum`/index`.php`?t`=usrinfo`&id`=648`&^2 k
oldo][2 .]&]
[s0;#/+75 ]]