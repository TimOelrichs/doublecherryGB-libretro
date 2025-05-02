// printer_registry.hpp
#pragma once
#include "printer_interface.hpp"
#include <memory>
#include <map>
#include <string>



class PrinterRegistry {
public:
    // Registrierung zur Initialisierung
    static void register_printer(const std::string& name, std::unique_ptr<IPrinter> printer);

    // Gibt aktiven Drucker zurück
    static IPrinter* current();
    static void set_current(const std::string& name);

    // Optional: für UI-Menü
    static std::vector<std::string> available_printers();

private:
    static std::map<std::string, std::unique_ptr<IPrinter>>& printers();
    static std::string& current_name();
};
