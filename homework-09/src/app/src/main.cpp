#include "app/cli_handler.h"
#include "core/storage.h"
#include <spdlog/spdlog.h>

int main(int argc, char* argv[]) {
    try{
        core::Storage storage{"projects.json"};
        app::CLIHandler cli{storage};
        return cli.run(argc, argv);
    } catch(const std::exception& e){
        spdlog::critical("Error: {}", e.what());
        return 1;
    }
}
