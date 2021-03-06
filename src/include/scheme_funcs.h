#ifndef __SCHEME_FUNCS__
#define __SCHEME_FUNCS__

#include "symbol.h"

#define car(obj) obj->value.tuple.car
#define cdr(obj) obj->value.tuple.cdr


#define caar(obj) car(car(obj))
#define cadr(obj) car(cdr(obj))
#define cdar(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))

#define caaar(obj) car(car(car(obj)))
#define caadr(obj) car(car(cdr(obj)))
#define cadar(obj) car(cdr(car(obj)))
#define caddr(obj) car(cdr(cdr(obj)))
#define cdaar(obj) cdr(car(car(obj)))
#define cdadr(obj) cdr(car(cdr(obj)))
#define cddar(obj) cdr(cdr(car(obj)))
#define cdddr(obj) cdr(cdr(cdr(obj)))

#define false interp->boolean.false
#define true interp->boolean.true
#define eof_object interp->eof_object
#define peek_scanner interp->scanner->scanner
#define peek_previous interp->scanner->previous

object_type *cons(interp_core_type *interp, object_type *car,object_type *cdr);
object_type *quote(interp_core_type *interp, object_type *obj);


bool is_primitive(interp_core_type *interp, object_type *obj);
bool is_tagged_list(interp_core_type *interp, object_type *obj);
bool is_quoted(interp_core_type *interp, object_type *obj);
bool is_self_evaluating(interp_core_type *interp, object_type *obj);
bool is_symbol(interp_core_type *interp, object_type *obj);
bool is_true(interp_core_type *interp, object_type *obj);
bool is_tuple(interp_core_type *interp, object_type *obj);
bool is_closure(interp_core_type *interp, object_type *obj);
bool is_empty_list(interp_core_type *interp,object_type *obj);


/* List of primitives to bind */
extern binding_type primitive_list[];

#endif
