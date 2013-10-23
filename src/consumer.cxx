#include <potholes/consumer.h>
#include <potholes/analysis.h>

#include "potholes/transform.h"



     potholes::ConsumerFactory::ConsumerFactory(clang::Rewriter& rewriter, potholes::Analysis& analysis, potholes::RewriteCallback& cb) : 
      analysis(analysis), callback(cb) {
        }

        clang::ASTConsumer *potholes::ConsumerFactory::newASTConsumer() {
            CompositeConsumer * consumer = new CompositeConsumer(analysis, callback);
            return consumer;
        }
        
        potholes::CompositeConsumer::CompositeConsumer(potholes::Analysis& analysis, potholes::RewriteCallback& callback) : analysis(analysis), callback(callback) { 
            // registers analysis
        }
        
        void potholes::CompositeConsumer::Initialize(clang::ASTContext& Context) { 
          // Call the initialization for each transform
        
            callback.setSourceMgr(Context.getSourceManager(), Context.getLangOpts());
            
            /*clang::SourceManager & sm = Context.getSourceManager();
            const clang::LangOptions & lopts = Context.getLangOpts();
            rewriter.setSourceMgr(sm, lopts);
            */
            std::set<potholes::Transform *> & transforms  = analysis.getTransforms();
            std::set<potholes::Transform *>::iterator cit;
            for (cit = transforms.begin() ; cit != transforms.end() ; cit++ ) {
                
                potholes::Transform * transform = *cit;
                transform->Initialize(Context);
            }
            
        }
        
        bool potholes::CompositeConsumer::HandleTopLevelDecl(clang::DeclGroupRef d) { 
            // call top level declaration for each transform
            // std::cout << "Processing Top Level Declarations" << std::endl;
            
             
            std::set<potholes::Transform *> & transforms  = analysis.getTransforms();
            std::set<potholes::Transform *>::iterator cit;
            for (cit = transforms.begin() ; cit != transforms.end() ; cit++ ) {
                
                potholes::Transform * transform = *cit;
                transform->HandleTopLevelDecl(d);
            }
            
            return true;
        }