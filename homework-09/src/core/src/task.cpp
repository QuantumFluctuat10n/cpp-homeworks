#include "core/task.h"

namespace core{

std::string toString(Priority priority){
    switch(priority){
        case Priority::Low: return "low";
        case Priority::Medium: return "medium";
        case Priority::High: return "high";
    }
    return "low";
}
std::string toString(Status status){
    switch(status){
        case Status::Open: return "open";
        case Status::InProgress: return "in progress";
        case Status::Completed: return "completed";
        case Status::Canceled: return "canceled";
    }
    return "open";
}

Priority fromStringPriority(const std::string& s){
    if(s == "high") return Priority::High;
    if(s == "medium") return Priority::Medium;
    return Priority::Low;
}
Status fromStringStatus(const std::string& s){
    if(s == "in_progress") return Status::InProgress;
    if(s == "completed") return Status::Completed;
    if(s == "canceled") return Status::Canceled;
    return Status::Open;
}

}