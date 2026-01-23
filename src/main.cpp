
#include "CmdLineArgHandler.hpp"
#include "DirectoryContentProcessor.hpp"

#include <openvdb/openvdb.h>

#include <filesystem>
#include <iostream>
#include <string>

std::filesystem::path ProcessPath(const int &index, char **argv)
{
    CmdLineArgHandler handler = CmdLineArgHandler(argv[index]);
    return handler.getFullPath();
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_directory>\n";
        return 1;
    }

    openvdb::initialize();
    std::filesystem::path inputDir = ProcessPath(1, argv);
    std::filesystem::path outputDir = ProcessPath(2, argv);

    DirectoryContentProcessor processor = DirectoryContentProcessor(inputDir);
    processor.execute(outputDir);
}