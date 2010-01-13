#ifndef __SCHEME_FUNCS__
#define __SCHEME_FUNCS__


#define car(obj) obj->value.tuple.car
#define cdr(obj) obj->value.tuple.cdr


#define caar(obj) car(car(obj))
#define cdar(obj) car(cdr(obj))
#define cadr(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))



#endif
