#pragma once

#include <filesystem>
#include <system_error>
#include <optional>

class CmdLineArgHandler{
    public:
    CmdLineArgHandler(const char *argv);
    
    std::filesystem::path getFullPath() const; 

    private:
    std::filesystem::path m_processedPath; 
};