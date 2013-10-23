

#include <isl/ctx.h>
#include <isl/aff.h>
#include <isl/arg.h>
#include <isl/options.h>

#include <pet.h>
#include <options.h>


struct options {
	struct isl_options	*isl;
	struct pet_options	*pet;
	char			*input;
};

ISL_ARGS_START(struct options, options_args)
ISL_ARG_CHILD(struct options, isl, "isl", &isl_options_args, "isl options")
ISL_ARG_CHILD(struct options, pet, NULL, &pet_options_args, "pet options")
ISL_ARG_ARG(struct options, input, "input", NULL)
ISL_ARGS_END

ISL_ARG_DEF(options, struct options, options_args)

        
isl_ctx * build_options(int n_path, const char ** paths) { 
    struct options * options;
         
    options = options_new_with_defaults();
         
    isl_ctx * pet_ctx = isl_ctx_alloc_with_options(&options_args, options);
    
 //   unsigned flags = ISL_ARG_ALL;
    
 //   argc = options_parse(options, argc, argv, flags);
    options->pet->n_path = n_path;
    options->pet->paths = paths;
    
    return pet_ctx;
}
         