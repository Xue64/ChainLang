//
// Created by Rufelle on 10/02/2023.
//

#ifndef CHAINC_ARGS_HANDLER_H
#define CHAINC_ARGS_HANDLER_H

#include <vector>
#include <string>
#include "runtime.h"
namespace chain {
    void handle(const std::vector<std::string> &args) {
        try {
            Runtime(args.at(1));
        } catch (std::invalid_argument & e){
            std::cerr << "<!> The Chain Virtual Machine cannot read non '.exec' files. Please load the appropriate file.\n";
            exit(3);
        } catch (std::ios_base::failure & e){
            std::cerr << "<!> Instance of file '" << args.at(1) << "' not found.\n";
        }

    }
}


#endif //CHAINC_ARGS_HANDLER_H
