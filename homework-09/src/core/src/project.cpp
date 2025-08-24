#include "core/project.h"

namespace core{

Project::Project(int id_, const std::string& name_, const std::string& description_) : id(id_), name(name_), description(description_) {}

Task& Project::addTask(Task task){
    tasks.push_back(std::move(task));
    return tasks.back();
}

Task* Project::findTask(int taskId){
    for(auto& t : tasks) if(t.id == taskId) return &t;
    return nullptr;
}

}