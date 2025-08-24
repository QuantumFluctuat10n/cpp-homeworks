#pragma once
#include <core/storage.h>
#include <string>

namespace app{

class CLIHandler{
public:
    CLIHandler(core::Storage& storage_);
    int run(int argc, char* argv[]);

private:
    core::Storage& storage;
    int CLIcreate(const std::string& name, const std::string& description);
    int CLIlist();
    int CLIaddTask(int projectId, const std::string& name, const std::string& priority);
    int CLIlistTasks(int projectId);
    int CLIupdateTask(int taskId, const std::string& status);
    int CLIexport(int projectId, std::string& format);
    int CLIimport(const std::string& file);
};

}