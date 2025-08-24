#pragma once
#include <vector>
#include <filesystem>
#include <nlohmann/json.hpp>
#include "core/project.h"
#include "core/task.h"

namespace core{

class Storage {
public:
    Storage(std::filesystem::path jsonPath_ = "projects.json");

    void load();
    void save() const;

    std::vector<Project> getProjects();

    Project& createProject(const std::string& name, const std::string& description);
    Project* getProject(int projectId);

    Task& addTask(int projectId, const std::string& title, Priority priority);
    bool updateTask(int taskId, Status status);
    std::vector<Task> listTasks(int projectId);

    nlohmann::json exportJson(int projectId);
    void importJson(const std::filesystem::path& file);

private:
    std::filesystem::path jsonPath;
    std::vector<Project> projects;
    int nextProjectId{1};
    int nextTaskId{1};

    void reindexIds();

    std::pair<Project*, Task*> findTaskGlobal(int taskId);
};

} 