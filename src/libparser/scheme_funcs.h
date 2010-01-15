#ifndef __SCHEME_FUNCS__
#define __SCHEME_FUNCS__


#define car(obj) obj->value.tuple.car
#define cdr(obj) obj->value.tuple.cdr


#define caar(obj) car(car(obj))
#define cdar(obj) car(cdr(obj))
#define cadr(obj) cdr(car(obj))
#define cddr(obj) cdr(cdr(obj))


object_type *cons(interp_core_type *interp, object_type *car,object_type *cdr);
object_type *quote(interp_core_type *interp, object_type *obj);

void bind_symbol(interp_core_type *interp, object_type *obj, object_type *value);
object_type *get_binding(interp_core_type *interp, object_type *sym);


bool is_primitive(interp_core_type *interp, object_type *obj);
bool is_tagged_list(interp_core_type *interp, object_type *obj);
bool is_quoted(interp_core_type *interp, object_type *obj);
bool is_self_evaluating(interp_core_type *interp, object_type *obj);
bool is_symbol(interp_core_type *interp, object_type *obj);

/* Primitives */
object_type *prim_define(interp_core_type *interp, object_type *args);
object_type *prim_quit(interp_core_type *interp, object_type *args);

typedef struct binding {
    char *symbol;
    primitive_type primitive;
} binding_type;


/* List of primitives to bind */
extern binding_type primitive_list[];

#endif
