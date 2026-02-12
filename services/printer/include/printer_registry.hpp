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

    // Gibt aktiven Drucker zur�ck
    static IPrinter* current();
    static void set_current(const std::string& name);

    // Optional: f�r UI-Men�
    static std::vector<std::string> available_printers();

    static void shutdown()
    {
        printers().clear();
    }

private:
    static std::map<std::string, std::unique_ptr<IPrinter>>& printers();
    static std::string& current_name();
};
