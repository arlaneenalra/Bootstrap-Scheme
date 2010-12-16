#ifndef _SYMBOL_
#define _SYMBOL_

#include "core.h"

typedef struct binding {
    char *symbol;
    fn_type primitive;
    bool eval_first;
    bool eval_end;
} binding_type;

object_type *create_symbol(interp_core_type *interp, char *str);
void bind_argument_list(interp_core_type *interp, object_type *sym_list, 
			object_type *value_list);
void bind_symbol(interp_core_type *interp, object_type *obj, 
		 object_type *value, object_type **env);
void bind_symbol_list(interp_core_type *interp, 
		      binding_type *binding_list, object_type **env);
object_type *get_binding(interp_core_type *interp, object_type *sym);


#endif
