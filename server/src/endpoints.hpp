#include "dependencies/CppHttp/include/CppHttp.hpp"
#include "dependencies/CppHttp/include/nlohmann/json.hpp"
#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include <vector>
#include <fstream>

using returnType = std::tuple<CppHttp::Net::ResponseType, std::string, std::optional<std::vector<std::string>>>;
using json = nlohmann::json;

static std::vector<std::string> todos;

returnType Index(CppHttp::Net::Request req) {
    std::ifstream file("templates/index.html");

    if (!file.is_open()) {
        return { CppHttp::Net::ResponseType::NOT_FOUND, "404 Not Found", {} };
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return { CppHttp::Net::ResponseType::HTML, content, {} };
}

returnType AddTodo(CppHttp::Net::Request req) {
    json body;

	try {
		body = json::parse(req.m_info.body);
	}
	catch (json::exception& e) {
		return std::make_tuple(CppHttp::Net::ResponseType::BAD_REQUEST, e.what(), std::nullopt);
	}

    if (body.find("todo") == body.end()) {
        return { CppHttp::Net::ResponseType::BAD_REQUEST, "Missing 'todo' field", {} };
    }

    std::string todo = body["todo"].get<std::string>();

    todos.push_back(body["todo"]);

    std::string html = "<div id='e" + std::to_string(todos.size() - 1) + "' class='flex flex-row bg-blue-500 min-h-[10vh] w-[35vw] rounded-[10pt] items-center text-wrap overflow-hidden'><div class='flex flex-row items-center relative w-[100%] h-[100%]'><p class='pl-[10px] py-[12px] text-white text-wrap whitespace-normal overflow-auto break-all'>" + todo + "</p><button class='absolute right-[0px] ml-[10px] mr-[10px]' hx-target='#e" + std::to_string(todos.size() - 1) + "' hx-swap='outerHTML' hx-delete='/remove' hx-vals='{\"todo\": " + std::to_string(todos.size() - 1) + "}' hx-ext='json-enc'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 512 512' id='checkbox' class='w-[4vh] h-[4vh]'><path d='M405.333 106.667v298.666H106.667V106.667h298.666m0-42.667H106.667C83.198 64 64 83.198 64 106.667v298.666C64 428.802 83.198 448 106.667 448h298.666C428.802 448 448 428.802 448 405.333V106.667C448 83.198 428.802 64 405.333 64z'></path></svg></button></div></div>";

    return { CppHttp::Net::ResponseType::HTML, html, {} };
}

returnType RemoveTodo(CppHttp::Net::Request req) {
    json body;

    std::cout << req << std::endl;

	try {
		body = json::parse(req.m_info.body);
	}
	catch (json::exception& e) {
		return std::make_tuple(CppHttp::Net::ResponseType::BAD_REQUEST, e.what(), std::nullopt);
	}

    if (body.find("todo") == body.end()) {
        return { CppHttp::Net::ResponseType::BAD_REQUEST, "Missing 'todo' field", {} };
    }

    int todo = body["todo"].get<int>();

    if (todo < 0 || todo >= todos.size()) {
        return { CppHttp::Net::ResponseType::BAD_REQUEST, "Invalid 'todo' field", {} };
    }

    todos.erase(todos.begin() + todo);

    return { CppHttp::Net::ResponseType::OK, "", {} };
}

returnType GetTodos(CppHttp::Net::Request req) {
    std::string todosHtml;

    int i = 0;
    for (auto& todo : todos) {
        todosHtml += "<div id='" + std::to_string(i) + "' class='flex flex-row bg-blue-500 min-h-[10vh] w-[35vw] rounded-[10pt] items-center text-wrap overflow-hidden'><div class='flex flex-row items-center relative w-[100%] h-[100%]'><p class='pl-[10px] py-[12px] text-white text-wrap whitespace-normal overflow-auto break-all'>" + todo + "</p><button class='absolute right-[0px] ml-[10px] mr-[10px]' onClick='removeTodo(this)'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 512 512' id='checkbox' class='w-[4vh] h-[4vh]'><path d='M405.333 106.667v298.666H106.667V106.667h298.666m0-42.667H106.667C83.198 64 64 83.198 64 106.667v298.666C64 428.802 83.198 448 106.667 448h298.666C428.802 448 448 428.802 448 405.333V106.667C448 83.198 428.802 64 405.333 64z'></path></svg></button></div></div>";
        ++i;
    }

    return { CppHttp::Net::ResponseType::HTML, todosHtml, {} };
}