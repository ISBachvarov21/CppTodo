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

    std::string todosHtml;

    for (auto& todo : todos) {
        todosHtml += "<div class='flex bg-blue-500 min-h-[10vh] w-[35vw] rounded-[10pt] justify-center items-center text-wrap overflow-hidden'> <p class='px-[16px] py-[12px] text-white text-wrap whitespace-normal overflow-auto' style='word-break: break-all;'>" + todo + "</p></div>";
    }

    content.replace(content.find("{{todos}}"), 14, todosHtml);

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


    todos.push_back(body["todo"]);

    std::ifstream file("templates/index.html");

    if (!file.is_open()) {
        return { CppHttp::Net::ResponseType::NOT_FOUND, "404 Not Found", {} };
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::string todosHtml;

    for (auto& todo : todos) {
        todosHtml += "<div class='bg-blue-500 h-[10vh] w-[35vw] rounded-[10pt]'>" + todo + "</div>";
    }

    std::cout << "Todos: " << todosHtml << std::endl;

    content.replace(content.find("{{todos}}"), 14, todosHtml);

    return { CppHttp::Net::ResponseType::HTML, content, {} };
}