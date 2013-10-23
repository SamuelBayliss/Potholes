
#include <isl/map.h>
#include <isl/flow.h>
#include <isl/schedule.h>

#include <potholes/ast.h>
#include <potholes/parallel.h>

#include <iostream>
#include <cassert>

isl_union_map * pth_compute_dependencies(pth_scop * scop) { 


        isl_union_map * reads  = pet_scop_collect_reads(scop->scop);
        isl_union_map * writes = pet_scop_collect_writes(scop->scop);
        isl_union_map * schedule = pet_scop_collect_schedule(scop->scop);
        
        // Dependencies with no source are live-in
        isl_union_map * live_in;
        // RAW dependencies
        isl_union_map * raw_dependencies;
        // WAW dependencies are coalesced from must and may dependencies includes war dependencies. 
        isl_union_map * waw_must_dependencies;
        isl_union_map * waw_may_dependencies;
    
        isl_union_map * empty = isl_union_map_empty(isl_union_map_get_space(writes));
        
        int success;
        // Read After Write Deps
        success = isl_union_map_compute_flow(isl_union_map_copy(reads), isl_union_map_copy(writes),empty, isl_union_map_copy(schedule), &raw_dependencies, NULL, &live_in, NULL);
        
    
        // Write After Write and Read Deps
        success = isl_union_map_compute_flow(isl_union_map_copy(writes), isl_union_map_copy(writes), isl_union_map_copy(reads), isl_union_map_copy(schedule), &waw_must_dependencies, &waw_may_dependencies, NULL, NULL);
        
        (void)(success);
        
        isl_union_map * waw_war_dependencies = isl_union_map_union(waw_must_dependencies, waw_may_dependencies);
        waw_war_dependencies = isl_union_map_coalesce(waw_war_dependencies);
   
        isl_union_map * dependencies = isl_union_map_union(raw_dependencies, waw_war_dependencies);
        dependencies = isl_union_map_coalesce(dependencies);
        
        
 
        return dependencies;
}

isl_union_map * pth_update_dependencies(isl_union_map * dependencies, isl_map * tiling){ 
    return dependencies;
}

isl_schedule * pth_compute_schedule(pth_scop *, isl_map * tiling) {
    isl_schedule * schedule;
    return schedule;
    
}

/*pth_scop * pth_apply_tiling(pth_scop * scop, pth_stmt * statement, isl_map * tiling) { 
    return scop;
}
isl_map *  pth_create_tiling(isl_set * domain, unsigned dim, unsigned tile_size) {
    return NULL;
}
*/

int pth_apply_schedule(isl_map *schedule, void *user) {
        pth_scop * scop = (pth_scop *)(user);
        
        isl_space * schedule_space = isl_map_get_space(schedule);
    
        if (isl_space_has_tuple_id(schedule_space, isl_dim_in)) { 
                isl_id * schedule_tuple_id = isl_space_get_tuple_id(schedule_space, isl_dim_in);
            for (int i = 0 ; i < scop->scop->n_stmt ; i++) {
                pth_stmt * stmt = scop->scop->stmts[i];
                isl_space * domain_space = isl_set_get_space(stmt->domain);
                isl_id * domain_tuple_id = isl_space_get_tuple_id(domain_space, isl_dim_set);
                if (domain_tuple_id == schedule_tuple_id) { 
               
                    
                        

    
        isl_map_free(stmt->schedule);
        stmt->schedule = isl_map_copy(schedule);
        
        
                    
                    
                    return 0;
                }
               
            }
            
        } 
  
        return -1;
    }

pth_scop * pth_apply_schedule(pth_scop * scop, isl_schedule * schedule) {
    
    // For each map in the union map, look for appropriate schedule and substitute in scop
    isl_union_map * schedule_map = isl_schedule_get_map(schedule);
    int success = isl_union_map_foreach_map(schedule_map, pth_apply_schedule, scop);
    
    assert(success == 0);
    return scop;
}

int band_function(isl_band * band, void * user) { 
    
    //isl_band_foreach_member
    
    isl_ctx * ctx = isl_band_get_ctx(band);
    isl_printer * printer = isl_printer_to_str(ctx);
    printer = isl_printer_start_line(printer);
    printer = isl_printer_print_band(printer, band);
     printer = isl_printer_end_line(printer);
    std::cout << isl_printer_get_str(printer) << std::endl;
    isl_printer_free(printer);
    return 0;
}

isl_printer * print_tree_indent(isl_printer * printer, isl_val_list * indent, int i) {
       isl_ctx * ctx = isl_printer_get_ctx(printer);
   for(int j = 0 ; j < isl_val_list_n_val(indent) ; j++) {
              isl_val * val = isl_val_list_get_val(indent, j);
             printer =  isl_printer_print_val(printer, val);
               printer =  isl_printer_print_str(printer, ".");
          }
          printer = isl_printer_print_val(printer, isl_val_int_from_ui(ctx, i));
          printer = isl_printer_print_str(printer, "\t\t\t");
         
             return printer;
}

struct index_list { 
    unsigned i;
    struct index_list * next;
};

struct index_list * index_list_alloc() {
    struct index_list * list = (struct index_list *) malloc(sizeof(struct index_list));
    list->next = NULL;
    return list;
}

struct index_list * index_list_deep_copy(struct index_list * list) {
    if(list == NULL) return NULL;
    struct index_list * copy_list = index_list_alloc();
    copy_list->i = list->i;
    copy_list->next = index_list_deep_copy(list->next);
    return copy_list;
}

struct index_list * index_list_insert(struct index_list * list, unsigned i) {
   struct index_list * new_list = index_list_alloc();
    new_list->i = i;
    new_list->next = list;
    return new_list;
}

isl_printer * index_list_print(isl_printer * printer, struct index_list * list, bool top_level) {
    if (list != NULL) {
         isl_ctx * ctx = isl_printer_get_ctx(printer);
         isl_val * val = isl_val_int_from_ui(ctx,list->i);
      
        // printer = isl_printer_print_str(printer, "fff");
         
        
         printer = index_list_print(printer, list->next, false);
          
           printer =  isl_printer_print_val(printer, val);
           if(!top_level) isl_printer_print_str(printer, ".");
           isl_val_free(val);
    }
    return printer;
}

/*isl_printer * print_band(isl_printer * printer) {
    
}*/

int recursive_band_list(isl_printer * printer, isl_band_list * list, struct index_list * indent) {
    isl_ctx * ctx = isl_band_list_get_ctx(list);
    
       int band_count = isl_band_list_n_band(list);
       printer = isl_printer_start_line(printer);
    
    
       for (int i = 0 ; i < band_count ; i++) {
     
          isl_band * band = isl_band_list_get_band(list, i);
          int band_member_count = isl_band_n_member(band);
          
          isl_val * val = isl_val_int_from_ui(ctx,band_member_count);
      

          index_list * cpy = index_list_deep_copy(indent);
          cpy = index_list_insert(cpy, i);
          printer = isl_printer_print_val(printer, val);
          printer = isl_printer_print_str(printer, "\t");
          isl_val_free(val);
          printer = index_list_print(printer, cpy, true);
          printer = isl_printer_print_band(printer, band);
           for (int j = 0 ; j < band_member_count ; j++ ) {
              int is_zero_distance = isl_band_member_is_zero_distance(band, j);
              isl_val * val = isl_val_int_from_ui(ctx, j);
              printer = isl_printer_print_str(printer, "\t");
          
              
              
              if (is_zero_distance) {
                    printer = isl_printer_print_str(printer, "is_zero ");
              } else { 
                   printer = isl_printer_print_str(printer, "is_not_zero ");
              }
              printer = isl_printer_print_val(printer, val);
              printer = isl_printer_print_str(printer, "\t");
           }
          
        //  printer = isl_printer_print_int(printer, band_count);
          isl_band_list * child_list = isl_band_get_children(band); 
          
          printer = isl_printer_end_line(printer);
      
           recursive_band_list(printer, child_list, cpy);

       }
    // foreach band in list. 
        // print the band
    
    // foreach band in the list
        // get the children
    
    
       return 0;
}

pth_scop * pth_apply_tiling(pth_scop * scop, unsigned tile_size) { 
  

    isl_union_map * dependencies = pth_compute_dependencies(scop);
    isl_union_set * domains = pet_scop_collect_domains(scop->scop);
    isl_schedule * schedule = isl_union_set_compute_schedule(domains, isl_union_map_copy(dependencies), dependencies);
        
    isl_ctx * ctx = isl_union_map_get_ctx(dependencies);
    isl_printer * printer = isl_printer_to_str(ctx);
    printer = isl_printer_print_schedule(printer, schedule);

  
   // int success = isl_schedule_foreach_band(schedule, band_function, NULL);
    
     printer = isl_printer_start_line(printer);
      printer = isl_printer_print_str(printer, "********"); 
        printer = isl_printer_end_line(printer);
      
       isl_band_list * bands = isl_schedule_get_band_forest(schedule);
       isl_val_list * indent = isl_val_list_alloc(ctx, 10);
       recursive_band_list(printer, bands, NULL);
     
            printer = isl_printer_start_line(printer);
      printer = isl_printer_print_str(printer, "********"); 
        printer = isl_printer_end_line(printer);
        
        isl_union_map * schedule_map = isl_schedule_get_map(schedule);
        printer = isl_printer_print_union_map(printer, schedule_map);
          std::cout << isl_printer_get_str(printer) << std::endl;
    isl_printer_free(printer);
    
    // This is broken!!
       //scop = pth_apply_schedule(scop, schedule_map);
 
    /*
    for (int i = 0 ; i < scop->scop->n_stmt ; i++ ) {
        pth_stmt * stmt = scop->scop->stmts[i];
        isl_space * space = isl_set_get_space(stmt->domain);
        int no_dims = isl_space_dim(space, isl_dim_out);
        
        for (int dim = 0; dim < no_dims ; dim++) {
                isl_set * domain = stmt->domain;
                isl_map * tiling = pth_create_tiling(domain, dim, tile_size);
                scop = pth_apply_tiling(scop, stmt, tiling);
                dependencies = pth_update_dependencies(dependencies, tiling);
        }
        
    }
    // compute schedule
        isl_union_set * domains = pet_scop_collect_domains(scop->scop);
      
        isl_schedule * schedule = isl_union_set_compute_schedule(domains, dependencies, dependencies);
        
        isl_union_map * schedule_map = isl_schedule_get_map(schedule);
        
    // distribute new schedule

        scop = pth_apply_schedule(scop, schedule_map);
     * */
        return scop;   
}
