# Домашнє завдання №9. Система управління проектами з CMake + Conan 2

## Опис завдання
Багатомодульна система управління проектами з використанням CMake та Conan 2.
Система складається з двох основних компонентів: бібліотеки core та CLI додатку.

## Основні класи

### core::Task
 - Поля: `id`, `title`, `priority`, `status` 
 - JSON серіалізація: `to_json`, `from_json`
 - Enum: `Priority` (`Low`, `Medium`, `High`), `Status` (`Open`, `InProgess`, `Completed`, `Canceled`)

### core::Project
 - Поля: `id`, `name`, `description`, `tasks`
 - JSON серіалізація: `to_json`, `from_json`
 - Методи:
    - `Task& addTask(Task task)`
    - `Task* findTask(int taskId)`

### core::Storage
 - Зберігає усі проекти
 - Методи:
    - `load()` - завантаження з JSON
    - `save()` - збереження у JSON
    - `Project& createProject(const std::string& name, const std::string& description)` - додає проект у сховище
    - `Task& addTask(int projectId, const std::string& title, Priority priority)` - додає завдання до проекту

## Команди CLI
```
./ProjectManager create --name "My Project" --description "Test project"
./ProjectManager list
./ProjectManager add-task --project-id 1 --title "Task 1" --priority high
./ProjectManager list-tasks --project-id 1
./ProjectManager update-task --task-id 1 --status in-progress
./ProjectManager export --project-id 1 --format json
./ProjectManager import --file projects.json
```

## Компіляція та запуск
```bash
conan install . --build missing
cmake -B build
cmake --build build
```
Виконучий файл згенерується за шляхом `buid/scr/app/`