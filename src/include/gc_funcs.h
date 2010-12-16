#ifndef _GC_
#define _GC_


object_type *alloc_object(interp_core_type *interp, object_type_enum obj_type);

object_type *alloc_vector(interp_core_type *interp, uint64_t len);
object_type *create_string(interp_core_type *interp, const char *str);

char *alloc_string(interp_core_type *interp, size_t len);
interp_core_type *alloc_interp();
scanner_stack_type *alloc_scanner();

/*void gc_all(interp_core_type *interp);*/


object_type *create_primitive(interp_core_type *interp, 
			      fn_type primitive,
			      bool eval_first, bool eval_end);

object_type *create_string(interp_core_type *interp, const char *str);

object_type *clone(interp_core_type *interp, object_type *obj);

#endif
