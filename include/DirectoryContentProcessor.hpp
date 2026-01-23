#pragma once

#include <filesystem>

class DirectoryContentProcessor
{
    public:
    DirectoryContentProcessor(std::filesystem::path inputDir);

    void execute(const std::filesystem::path &outputDir); 

    private:
    std::filesystem::path m_inputDir; 
};