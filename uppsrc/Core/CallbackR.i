#define A_
#define A_a_
#define a_
#define first_

#include "CallbackN.i"

#define classA    class A1
#define A_        A1
#define A_a_      A1 a1
#define a_        a1

#include "CallbackN.i"

#define classA    class A1, class A2
#define A_        A1, A2
#define A_a_      A1 a1, A2 a2
#define a_        a1, a2

#include "CallbackN.i"

#define classA    class A1, class A2, class A3
#define A_        A1, A2, A3
#define A_a_      A1 a1, A2 a2, A3 a3
#define a_        a1, a2, a3

#include "CallbackN.i"

#define classA    class A1, class A2, class A3, class A4
#define A_        A1, A2, A3, A4
#define A_a_      A1 a1, A2 a2, A3 a3, A4 a4
#define a_        a1, a2, a3, a4

#include "CallbackN.i"

#define classA    class A1, class A2, class A3, class A4, class A5
#define A_        A1, A2, A3, A4, A5
#define A_a_      A1 a1, A2 a2, A3 a3, A4 a4, A5 a5
#define a_        a1, a2, a3, a4, a5

#include "CallbackN.i"

#undef Res
#undef Cb_