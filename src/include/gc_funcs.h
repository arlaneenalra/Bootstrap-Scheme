#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

object_type *alloc_vector(interp_core_type *interp, uint64_t len);
object_type *create_string(interp_core_type *interp, char *str);

char *alloc_string(interp_core_type *interp, size_t len);
interp_core_type *alloc_interp();
scanner_stack_type *alloc_scanner();


void gc_register_root(interp_core_type *interp, object_type **root_ptr);
void gc_mark_perm(interp_core_type *interp, object_type *obj);

void gc_destroy(gc_core_type *gc);
void gc_stats(gc_core_type *gc);

/*void gc_all(interp_core_type *interp);*/

void gc_protect(interp_core_type *interp);
void gc_unprotect(interp_core_type *interp);


#endif
