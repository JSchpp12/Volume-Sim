#include "CmdLineArgHandler.hpp"

std::filesystem::path ProcessArg(const char *arg)
{
    std::filesystem::path path = arg;

    if (path.is_relative())
    {
        path = std::filesystem::absolute(path);
    }

    std::error_code err;
    if (!std::filesystem::exists(path, err))
    {
    }
    return path;
}

CmdLineArgHandler::CmdLineArgHandler(const char *argv) : m_processedPath(argv)
{
}

std::filesystem::path CmdLineArgHandler::getFullPath() const{
    if (m_processedPath.is_relative()){
        return std::filesystem::absolute(m_processedPath); 
    }

    return m_processedPath; 
}