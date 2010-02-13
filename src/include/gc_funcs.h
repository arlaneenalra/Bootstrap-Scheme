#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

char* alloc_string(interp_core_type *interp, size_t len);
interp_core_type *alloc_interp();
scanner_stack_type *alloc_scanner();

/*void gc_all(interp_core_type *interp);*/



#endif
