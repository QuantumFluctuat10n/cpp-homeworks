#include "core/storage.h"
#include "core/project.h"
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include <fstream>

namespace core{

Storage::Storage(std::filesystem::path jsonPath_) : jsonPath(jsonPath_) {
    try{
        load();
    } catch(const std::exception& e){
        spdlog::warn("Failed to load JSON on init {}", e.what());
    }
}

std::vector<Project> Storage::getProjects(){
    return projects;
}

void Storage::load(){
    projects.clear();
    if(!std::filesystem::exists(jsonPath)){
        spdlog::info("JSON file '{}' not found, creating new one.", jsonPath.string());
        reindexIds();
        return;
    }

    std::ifstream ifs(jsonPath);
    if(!ifs){
        throw std::runtime_error(fmt::format("Can't open file for reading: {}", jsonPath.string()));
    }

    nlohmann::json j;
    ifs >> j;
    if(!j.is_array()){
        throw std::runtime_error("JSON root must be an array of projects.");
    }

    projects = j.get<std::vector<Project>>();
    reindexIds();
    spdlog::info("Loaded {} project(s) from '{}'.", projects.size(), jsonPath.string());
}

void Storage::save() const{
    nlohmann::json j = projects;
    std::ofstream ofs(jsonPath);
    if(!ofs){
        throw std::runtime_error(fmt::format("Can't open file for writing: {}", jsonPath.string()));
    }
    ofs << j.dump(2);
    spdlog::info("Saved {} project(s) to '{}'.", projects.size(), jsonPath.string());
}

void Storage::reindexIds(){
    int maxProjId = 0, maxTaskId = 0;
    for(const auto& p : projects){
        if(p.id > maxProjId) maxProjId = p.id;
        
        for(const auto& t : p.tasks) if(t.id > maxTaskId) maxTaskId = t.id;
    }

    nextProjectId = ++maxProjId;
    nextTaskId = ++maxTaskId;
}

Project& Storage::createProject(const std::string& name_, const std::string& description_){
    int id = nextProjectId++;
    projects.emplace_back(id, name_, description_);
    spdlog::info("Created project {} {}", id, name_);
    save();

    return projects.back();
}

Project* Storage::getProject(int projectId){
    for(auto& p : projects) if(p.id == projectId) return &p;
    return nullptr;
}

Task& Storage::addTask(int projectId, const std::string& title_, Priority Priority_){
    auto* proj = getProject(projectId);
    if(!proj) throw std::runtime_error(fmt::format("Project {} not found", projectId));
    Task t;
    t.id = nextTaskId++;
    t.title = title_;
    t.priority = Priority_;
    t.status = Status::Open;

    auto& ref = proj->addTask(std::move(t));
    spdlog::info("Added task {} to project {}", ref.id, projectId);
    save();
    return ref;
}

std::pair<Project*, Task*> Storage::findTaskGlobal(int taskId){
    for(auto& p : projects){
        if(auto* t = p.findTask(taskId)) return {&p, t};
    }
    return {nullptr, nullptr};
}

bool Storage::updateTask(int taskId, Status status){
    auto [proj, task] = findTaskGlobal(taskId);
    if(!task) return false;
    task->status = status;
    spdlog::info("Task {} set status {}", taskId, toString(status));
    save();
    return true;

}
std::vector<Task> Storage::listTasks(int projectId) {
    auto* p = getProject(projectId);
    if(!p) throw std::runtime_error(fmt::format("Project {} not found", projectId));
    save();
    return p->tasks;
}

nlohmann::json Storage::exportJson(int projectId) {
    auto* p = getProject(projectId);
    if(!p) throw std::runtime_error(fmt::format("Project {} not found", projectId));
    return nlohmann::json(*p);
}
void Storage::importJson(const std::filesystem::path& file){
    std::ifstream ifs(file);
    if(!ifs) throw std::runtime_error(fmt::format("Can't open file {}", file.string()));
    nlohmann::json j;
    ifs >> j;

    std::vector<Project> incoming;
    if(j.is_array()){
        incoming = j.get<std::vector<Project>>();
    } else if(j.is_object()){
        incoming.push_back(j.get<Project>());
    } else throw std::runtime_error("Failed to import JSON");

    for(auto& p : incoming){
        int newProjId = nextProjectId++;
        Project np(newProjId, p.name, p.description);
        for(const auto& t : p.tasks){
            Task nt = t;
            nt.id = nextTaskId++;
            np.addTask(nt);
        }
        projects.push_back(std::move(np));
        spdlog::info("Imported project '{}' as id", p.name, newProjId);
    }
    save();
}

}