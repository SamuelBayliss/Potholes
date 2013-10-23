#include <potholes/rewrite.h>

#include <potholes/transform.h>
#include <potholes/analysis.h>
        potholes::RewriteCallback::RewriteCallback(clang::Rewriter & rewriter,
                potholes::Analysis& analysis, std::ostream& os) : rewriter(rewriter), analysis(analysis) {
                     pth_generate_initialize(analysis.extractor.context());    
                    
        };

        void potholes::RewriteCallback::setSourceMgr(clang::SourceManager & sm, const clang::LangOptions& opts) {
                 
 
            rewriter.setSourceMgr(sm, opts);
              
        }

        potholes::Analysis::Files potholes::RewriteCallback::getTransformedFiles() {
            return transformed_files;
        }
        
        void potholes::RewriteCallback::emitCode(std::string& filename, std::ostream & os) {

            //    RewriteConsumerSet::iterator sit;
            //  for (sit = consumers.begin() ; sit != consumers.end() ; sit++) {

            //            clang::Rewriter & rewriter = (*sit)->getRewriter();
            clang::SourceManager & sm = rewriter.getSourceMgr();



            clang::SourceManager::fileinfo_iterator fit;
            for (fit = sm.fileinfo_begin(); fit != sm.fileinfo_end(); fit++) {
                const clang::FileEntry * fe = fit->first;

          //      std::cout << fe->getName() << "\n";
                
                potholes::Scop * scop = analysis.extractor.GetScop(getAbsolutePath(fe->getName()));
                if (scop) {
                    if (fe) {
                       // std::cerr << std::string(fe->getName()) << std::endl;
//                        clang::FileID fid = sm.translateFile(fe);

                        const clang::RewriteBuffer * buf = rewriter.getRewriteBufferFor(sm.translateFile(fe));
                        if (buf) {
                           
                       //     llvm::sys::Path source_name();
                            llvm::SmallString<8> name(llvm::StringRef(fe->getName()));
                            const llvm::StringRef stem = llvm::sys::path::stem(name.str());
                            llvm::sys::path::remove_filename(name);
//                            const llvm::StringRef file_name = llvm::sys::path::filename(name.str());
                            const llvm::StringRef extension = llvm::sys::path::extension(llvm::StringRef(fe->getName()));
                            
                            
                         /*   std::cerr << std::string(name) << "\n";
                            source_name.eraseComponent();
                            source_name.appendComponent("vivado");
                            std::string error;
                            source_name.createDirectoryOnDisk(true, &error);
                            source_name.appendComponent(file_name);
                            std::cerr << error << "\n";
                          */
                            std::string output_file_name = std::string(name.str()) + "/" + std::string(stem) + "_vivado" + std::string(extension);
                     
                            transformed_files.push_back(output_file_name);
                           // std::cout << "output_file_name is " << output_file_name << "\n";
                            
                         std::ofstream  output(output_file_name.c_str(), std::ios::out);
                         if (output.is_open()) { 
                            output << std::string(buf->begin(), buf->end());
                            output.close();
                         }
                        
                        }

                    }

                }
            }
            /* std::set<clang::SourceLocation> & slocs = (*sit)->getLocations();
             std::set<clang::FileID> fids;
           
             std::set<clang::SourceLocation>::iterator slit;
             for (slit = slocs.begin() ; slit != slocs.end() ; slit++) {
                std::cout << "Getting Source Location" << slit->printToString(sm) << std::endl;
             }
             */

            //  std::transform(slocs.begin(), slocs.end(), std::inserter(fids, fids.begin()), GetFileIDs(sm));

            //   std::for_each(fids.begin(), fids.end(), PrintFile(os, rewriter));
            // }


        }

        void potholes::RewriteCallback::run() {
            // go through all the transforms registered in analysis 
            
              
            std::set<potholes::Transform *> & transforms  = analysis.getTransforms();
            std::set<potholes::Transform *>::iterator cit;
            for (cit = transforms.begin() ; cit != transforms.end() ; cit++ ) {
                
                potholes::Transform * transform = *cit;
                transform->ApplyTransformation(rewriter);
            }
            
            std::string prefix = "potholes";
             emitCode(prefix, std::cout);
        };
    