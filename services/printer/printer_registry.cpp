// printer_registry.cpp
#include "./include/printer_registry.hpp"

std::map<std::string, std::unique_ptr<IPrinter>>& PrinterRegistry::printers() {
    static std::map<std::string, std::unique_ptr<IPrinter>> map;
    return map;
}

std::string& PrinterRegistry::current_name() {
    static std::string name = "png"; // Default
    return name;
}

void PrinterRegistry::register_printer(const std::string& name, std::unique_ptr<IPrinter> printer) {
    printers()[name] = std::move(printer);
}

IPrinter* PrinterRegistry::current() {
    auto it = printers().find(current_name());
    return (it != printers().end()) ? it->second.get() : nullptr;
}

void PrinterRegistry::set_current(const std::string& name) {
    if (printers().count(name))
        current_name() = name;
}

std::vector<std::string> PrinterRegistry::available_printers() {
    std::vector<std::string> names;
    for (auto it = printers().begin(); it != printers().end(); ++it) {
        names.push_back(it->first);
    }
    return names;
}
