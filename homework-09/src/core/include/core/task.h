#pragma once
#include <string>
#include <nlohmann/json.hpp>

namespace core{

enum class Priority{
    Low,
    Medium,
    High
};

enum class Status{
    Open,
    InProgress,
    Completed,
    Canceled
};

std::string toString(Priority priority);
std::string toString(Status status);

Priority fromStringPriority(const std::string& s);
Status fromStringStatus(const std::string& s);

class Task{
public:
    Task() = default;
    int id{};
    std::string title;
    Priority priority;
    Status status;
};

inline void to_json(nlohmann::json& j, const Task& t){
    j = nlohmann::json{
        {"id", t.id},
        {"title", t.title},
        {"priority", toString(t.priority)},
        {"status", toString(t.status)}
    };
}

inline void from_json(const nlohmann::json& j, Task& t){
    j.at("id").get_to(t.id);
    j.at("title").get_to(t.title);

    std::string pr = j.value("priority", "low");
    std::string st = j.value("status", "open");
    t.priority = fromStringPriority(pr);
    t.status = fromStringStatus(st);
}

}