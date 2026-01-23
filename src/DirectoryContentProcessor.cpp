#include "DirectoryContentProcessor.hpp"

#include "VolumeProcessor.hpp"

#include <memory>
#include <sstream>
#include <stdexcept>
#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <vector>

std::vector<std::filesystem::path> ListFiles(const std::filesystem::path &dir)
{
    std::vector<std::filesystem::path> files;

    for (const auto &entry : std::filesystem::directory_iterator(dir))
    {
        files.emplace_back(entry.path()); 
    }
    
    return files;
}

DirectoryContentProcessor::DirectoryContentProcessor(std::filesystem::path inputDir) : m_inputDir(std::move(inputDir))
{
}

void DirectoryContentProcessor::execute(const std::filesystem::path &outputDir)
{
    if (!std::filesystem::exists(m_inputDir))
    {
        std::ostringstream oss;
        oss << "Provided input directory does not exist:" << m_inputDir.string();

        throw std::runtime_error(oss.str());
    }

    if (!std::filesystem::exists(outputDir))
    {
        std::filesystem::create_directories(outputDir);
    }

    auto files = ListFiles(m_inputDir);
    if (files.empty())
    {
        std::cout << "Provided input directory is empty: " << m_inputDir.string() << std::endl;
        return;
    }

    for (const auto &file : files){
        std::cout << "Processing: " << file.string() << std::endl;
    }
    std::cout << "Dispatching parallel threads" << std::endl;

    tbb::parallel_for(tbb::blocked_range<size_t>(0, files.size()), [&, outputDir](const tbb::blocked_range<size_t>
    &r) {
        for (size_t i = r.begin(); i < r.end(); i++)
        {
            auto processor = VolumeProcessor(files[i]);
            processor.toNanoVDB(outputDir);
        }
    });
}