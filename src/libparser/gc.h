#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);
object_type *clone(interp_core_type *interp, object_type *obj);

void free_list(object_type *obj);
void free_marked(interp_core_type *interp, gc_mark_type mark);
void gc_all(interp_core_type *interp);

void mark_perm(interp_core_type *interp, object_type *obj);
void mark_reachable(interp_core_type *interp, object_type *obj, gc_mark_type mark);
gc_mark_type next_mark(interp_core_type *interp);


#endif
