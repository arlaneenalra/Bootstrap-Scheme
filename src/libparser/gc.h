#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);
object_type *clone(interp_core_type *interp, object_type *obj);

void free_list(object_type *obj);
void mark_free(interp_core_type *interp, object_type *obj);
void gc_all(interp_core_type *interp);





#endif
