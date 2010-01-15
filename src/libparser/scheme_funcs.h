#ifndef __SCHEME_FUNCS__
#define __SCHEME_FUNCS__


#define car(obj) obj->value.tuple.car
#define cdr(obj) obj->value.tuple.cdr


#define caar(obj) car(car(obj))
#define cdar(obj) car(cdr(obj))
#define cadr(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))

#define caaar(obj) car(car(car(obj)))
#define cdaar(obj) car(car(cdr(obj)))
#define cadar(obj) car(cdr(car(obj)))
#define cddar(obj) car(cdr(cdr(obj)))
#define caadr(obj) cdr(car(car(obj)))
#define cdadr(obj) cdr(car(cdr(obj)))
#define caddr(obj) cdr(cdr(car(obj)))
#define cdddr(obj) cdr(cdr(cdr(obj)))


object_type *cons(interp_core_type *interp, object_type *car,object_type *cdr);
object_type *quote(interp_core_type *interp, object_type *obj);

void bind_symbol(interp_core_type *interp, object_type *obj, object_type *value);
object_type *get_binding(interp_core_type *interp, object_type *sym);


bool is_primitive(interp_core_type *interp, object_type *obj);
bool is_tagged_list(interp_core_type *interp, object_type *obj);
bool is_quoted(interp_core_type *interp, object_type *obj);
bool is_self_evaluating(interp_core_type *interp, object_type *obj);
bool is_symbol(interp_core_type *interp, object_type *obj);
bool is_true(interp_core_type *interp, object_type *obj);

/* Primitives */

typedef struct binding {
    char *symbol;
    primitive_type primitive;
} binding_type;


/* List of primitives to bind */
extern binding_type primitive_list[];

#endif
