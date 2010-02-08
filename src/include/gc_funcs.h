#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);
object_type *clone(interp_core_type *interp, object_type *obj);

char* alloc_string(interp_core_type *interp, size_t len);

void gc_all(interp_core_type *interp);



#endif
