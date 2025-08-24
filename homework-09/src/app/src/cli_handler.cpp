#include "app/cli_handler.h"
#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <iostream>

namespace app{

CLIHandler::CLIHandler(core::Storage& storage_) : storage(storage_) {}

static core::Priority parsePriority(const std::string& s){
    auto ls = s;
    for(auto& c : ls) c = static_cast<char>(::tolower(c));
    if(ls == "high") return core::Priority::High;
    if(ls == "meduim") return core::Priority::Medium;
    return core::Priority::Low;
}

static core::Status parseStatus(const std::string& s){
    auto ls = s;
    for(auto& c : ls) c = static_cast<char>(::tolower(c));
    if(ls == "canceled") return core::Status::Canceled;
    if(ls == "completed") return core::Status::Completed;
    if(ls == "in-progress") return core::Status::InProgress;
    return core::Status::Open;
}

int CLIHandler::run(int argc, char* argv[]){
    CLI::App app{"ProjectManager"};
    app.require_subcommand(1);

    std::string name, desc;
    auto* create = app.add_subcommand("create", "Create a new project");
    create->add_option("--name", name)->required();
    create->add_option("--description", desc)->default_val("");

    auto* list = app.add_subcommand("list", "List projects");

    int projectId_add;
    std::string title, priority;
    auto* addTask = app.add_subcommand("add-task", "Add task to a project");
    addTask->add_option("--project-id", projectId_add)->required();
    addTask->add_option("--title", title)->required();
    addTask->add_option("--priority", priority)->default_val("low");

    int projectId_list;
    auto* listTasks = app.add_subcommand("list-tasks", "List tasks for a project");
    listTasks->add_option("--project-id", projectId_list)->required();

    int taskId_upd;
    std::string status;
    auto* updateTask = app.add_subcommand("update-task", "Update task status");
    updateTask->add_option("--task-id", taskId_upd)->required();
    updateTask->add_option("--status", status)->default_val("completed");

    int projectId_exp;
    std::string format = "json";
    auto* exportFile = app.add_subcommand("export", "Export a project");
    exportFile->add_option("--project-id", projectId_exp)->required();
    exportFile->add_option("--format", format)->default_val("json");

    std::string file;
    auto* importFile = app.add_subcommand("import", "Import project from JSON file");
    importFile->add_option("--file", file)->required();

    try{
        app.parse(argc, argv);
    } catch(const CLI::ParseError& e){
        return app.exit(e);
    }

    try{
        if(*create) return CLIcreate(name, desc);
        if(*list) return CLIlist();
        if(*addTask) return CLIaddTask(projectId_add, title, priority);
        if(*listTasks) return CLIlistTasks(projectId_list);
        if(*updateTask) return CLIupdateTask(taskId_upd, status);
        if(*exportFile) return CLIexport(projectId_exp, format);
        if(*importFile) return CLIimport(file);
    } catch(const std::exception& e){
        spdlog::error("Error: {}", e.what());
        std::cerr << fmt::format("Error: {}\n", e.what());
        return 1;
    }

    return 0;
}

int CLIHandler::CLIcreate(const std::string& name, const std::string& description){
    auto& p = storage.createProject(name, description);
    fmt::print("Created project {}: {}\n", p.id, p.name);
    return 0;
}

int CLIHandler::CLIlist(){
    const auto& ps = storage.getProjects();
    if(ps.empty()){
        fmt::print("No projects.\n");
        return 0;
    }

    fmt::print("Projects:\n");
    for(const auto& p : ps){
        fmt::print(" [{}] {} - {} ({} task(s))\n",
                    p.id, p.name, p.description, p.tasks.size());
    }
    return 0;
}

int CLIHandler::CLIaddTask(int projectId, const std::string& name, const std::string& priority){
    auto pr = parsePriority(priority);
    auto& t = storage.addTask(projectId, name, pr);
    fmt::print("Added task {} to project {}.\n", t.id, projectId);
    return 0;
}

int CLIHandler::CLIlistTasks(int projectId){
    auto tasks = storage.listTasks(projectId);
    if(tasks.empty()){
        fmt::print("No tasks.\n");
        return 0;
    }
    fmt::print("Tasks for project {}:\n", projectId);
    for(const auto& t : tasks){
        fmt::print(" [{}] {} | priority = {} | status = {}\n",
                    t.id, t.title, core::toString(t.priority), core::toString(t.status));
    }
    return 0;
}

int CLIHandler::CLIupdateTask(int taskId, const std::string& status){
    auto st = parseStatus(status);
    bool ok = storage.updateTask(taskId, st);
    if(!ok){
        fmt::print("Task {} not found.\n", taskId);
        return 2;
    }
    fmt::print("Task {} updatet to {}.\n", taskId, core::toString(st));
    return 0;
}

int CLIHandler::CLIexport(int projectId, std::string& format){
    if(format != "json") throw std::runtime_error("Only JSON export is supported");
    auto j = storage.exportJson(projectId);
    fmt::print("{}\n", j.dump(2));
    return 0;
}

int CLIHandler::CLIimport(const std::string& file){
    storage.importJson(file);
    fmt::print("Import completed.\n");
    return 0;
}

}