
#include <potholes/extract.h>





            void potholes::ExtractScop::Locations::addLocation(clang::SourceLocation location) {
                locations.insert(location);
            }

            std::set<clang::SourceLocation> & potholes::ExtractScop::Locations::getLocations() {
                return locations;
            }
     

        // typedef std::set<clang::SourceLocation> LocationDeclarations;




        isl_ctx * potholes::ExtractScop::context() {
            if (sm.begin() == sm.end()) return NULL;
            ScopMap::iterator smit = sm.begin();
            pet_scop * scop = smit->second->scop;
            isl_space * space = isl_set_get_space(scop->context);
            return isl_local_space_get_ctx(isl_local_space_from_space(space));
            
        }
        
        potholes::Scop * potholes::ExtractScop::GetScop(std::string filename) {

            ScopMap::iterator smit = sm.find(filename);
            if (smit != sm.end()) {
                return smit->second;
            }

            return NULL;
        }
        
       potholes::Scop * potholes::ExtractScop::GetScop(unsigned index) {

           if (index > sm.size() -1) {
               return NULL;
           } else { 
               ScopMap::iterator smit = sm.begin();
               while (smit != sm.end()) {
                 
                   if (index == 0) { 
                         
                       return smit->second;
                   }
                   smit++;
                   index--;
               }

           }
           
            
            return NULL;
        }
        
        
         unsigned potholes::ExtractScop::GetScopCount() { 
                return sm.size();
         }

        
        clang::SourceLocation potholes::ExtractScop::find_insert_location(clang::SourceManager & sm, std::string filename) {
            
                bool candidate_found = false;
                clang::SourceLocation candidate_location;
                
                pet_scop * scop = GetScop(getAbsolutePath(filename))->scop;
                if (scop) {           
                    unsigned start = scop->start;
                    unsigned candidate_offset = 0;
         
                 
                    
                    clang::SourceManager::fileinfo_iterator fit;

                    std::set<clang::SourceLocation> locations = lm.getLocations(); 
                    std::set<clang::SourceLocation>::iterator slit;
                    
                    for (slit = locations.begin() ; slit != locations.end() ; slit++) {
                        std::string location_filename = sm.getFilename(*slit);
                        if (location_filename == filename) { 
                            candidate_found = true;
                            if (candidate_offset == 0) { 
                             candidate_location = sm.getLocForStartOfFile(sm.getFileID(*slit));
                            }
                            unsigned file_offset = sm.getFileOffset(*slit);
                            if ((file_offset < start) && (file_offset > candidate_offset)) {
                                candidate_offset = file_offset;
                                candidate_location = *slit;
                            }
                            
                        }
                        
                    }
                    
                  /*  for (fit = sm.fileinfo_begin(); fit != sm.fileinfo_end(); fit++) {
                        const clang::FileEntry * fe = fit->first;

                        if (fe) {
                            if (std::string(fe->getName()) == getAbsolutePath(*pit)) {

                                clang::SourceLocation file_start = sm.getLocForStartOfFile(sm.translateFile(fe));
                                clang::SourceLocation fs = file_start.getLocWithOffset(start);
                                clang::SourceLocation fe = file_start.getLocWithOffset(finish);

                                rewriter.ReplaceText(clang::SourceRange(fs, fe), pth_generate_scop_function_invocation(scop));
                            }
                        }
                    } */


                }
                
                if (!candidate_found) {
                    throw std::exception();
                }
                
                return candidate_location;
                
        }

        potholes::ExtractScop::ExtractScop(isl_ctx * ctx, ScopMap & sm, Locations & lm) : ctx(ctx),  sm(sm), lm(lm) {
        };

            void potholes::ExtractScop::initialize(clang::tooling::CompilationDatabase * _db) {
                db = _db;
            }
        
        potholes::ExtractScop::Locations & potholes::ExtractScop::getLocations() {

            return lm;
        }

       
        
        void potholes::ExtractScop::operator() (std::string filename)
        {
          

            std::string absolute_filename = clang::tooling::getAbsolutePath(filename);
            std::vector<clang::tooling::CompileCommand> cc = db->getCompileCommands(absolute_filename);


            std::vector<clang::tooling::CompileCommand>::iterator ccit;
            std::vector<std::string>::iterator clt;

            std::vector<std::string> include_paths;


            int pet_options_argc = 0;
            const char ** pet_options_argv = NULL;

            for (ccit = cc.begin(); ccit != cc.end(); ccit++) {

                std::vector<std::string>& cl = ccit->CommandLine;
                for (clt = cl.begin(); clt != cl.end(); clt++) {
                    if ((clt->substr(0, 2) == "-I") || (clt->substr(0,2) == "-D")){

                        pet_options_argv = (const char **) (realloc(pet_options_argv, ++pet_options_argc * sizeof (char *)));
                        pet_options_argv[pet_options_argc - 1] = strdup(clt->substr(2, std::string::npos).c_str());
                    }
                }
            }

            isl_ctx * pet_ctx = build_options(pet_options_argc, pet_options_argv);
            pet_scop * scop;
            scop = pet_scop_extract_from_C_source(pet_ctx, filename.c_str(), NULL);
            if (scop) {
                //std::cout << "Found Scop " << absolute_filename << "\n";
                ScopPair pair(absolute_filename, new potholes::Scop(scop, filename));
                sm.insert(pair);
               // std::cout << "Insert : Map has " << sm.size() << "entries" << "\n";
            }
        }
  
