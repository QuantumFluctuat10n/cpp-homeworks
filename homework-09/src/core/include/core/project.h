#pragma once
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "core/task.h"

namespace core{

class Project{
public:
    Project() = default;
    Project(int id_, const std::string& name_, const std::string& description_);
    int id{};
    std::string name;
    std::string description;
    std::vector<Task> tasks;

    Task& addTask(Task task);
    Task* findTask(int taskId);
};

inline void to_json(nlohmann::json& j, const Project& p){
    j = nlohmann::json{
        {"id", p.id},
        {"name", p.name},
        {"description", p.description},
        {"tasks", p.tasks}
    };
}
inline void from_json(const nlohmann::json& j, Project& p){
    j.at("id").get_to(p.id);
    j.at("name").get_to(p.name);
    p.description = j.value("description", "");
    p.tasks.clear();
    if(j.contains("tasks")) j.at("tasks").get_to(p.tasks);
}

}