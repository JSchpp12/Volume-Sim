#include "VolumeProcessor.hpp"

#include <memory>
#include <zstd.h>

// Read whole file
static std::vector<char> readFile(const char *path)
{
    std::ifstream f(path, std::ios::binary);
    return std::vector<char>(std::istreambuf_iterator<char>(f), {});
}

// Write whole file
static void writeFile(const char *path, const std::vector<char> &data)
{
    std::ofstream f(path, std::ios::binary);
    f.write(data.data(), data.size());
}

std::unique_ptr<nanovdb::GridHandle<nanovdb::HostBuffer>> LoadNanoVDB(openvdb::SharedPtr<openvdb::FloatGrid> &grid)
{
    return std::make_unique<nanovdb::GridHandle<nanovdb::HostBuffer>>(nanovdb::tools::createNanoGrid(*grid));
}

VolumeProcessor::VolumeProcessor(std::filesystem::path filePath) : m_filePath(std::move(filePath))
{
}

std::string GetNanoFileNameFromOpen(const std::string openName)
{
    std::string result;

    return result;
}

void VolumeProcessor::toNanoVDB(const std::filesystem::path &outputDir)
{
    const std::string baseName = "density";

    if (!std::filesystem::exists(m_filePath))
    {
        std::ostringstream oss;
        oss << "Provided file toNanoVDB does not exist: " << m_filePath.string();
        throw std::runtime_error(oss.str());
    }
    openvdb::io::File file(m_filePath.string());
    openvdb::GridBase::Ptr baseGrid;
    file.open();
    for (openvdb::io::File::NameIterator nameIter = file.beginName(); nameIter != file.endName(); ++nameIter)
    {
        if (!baseGrid)
        {
            baseGrid = file.readGrid(nameIter.gridName());
        }
    }
    file.close();

    if (!baseGrid)
    {
        std::ostringstream oss;
        oss << "Grid not found while processing file: " << m_filePath;
        throw std::runtime_error(oss.str());
    }

    auto nanoPath = outputDir / std::filesystem::path(m_filePath).filename();
    nanoPath.replace_extension(".nvdb");

    bool error = false;
    if (!this->tryConvert<openvdb::FloatGrid>(baseGrid, nanoPath.string()))
    {
        error = true;
    }

    if (error)
    {
        std::ostringstream oss;
        oss << "Unable to find working type of grid for file";
        throw std::runtime_error(oss.str());
    }

    // compress file
    toCompressed(nanoPath);
}

void VolumeProcessor::toCompressed(const std::filesystem::path &nanoVDBPath) const
{
    std::filesystem::path resultPath = nanoVDBPath.parent_path() / nanoVDBPath.filename().replace_extension(".zst");

    auto src = readFile(nanoVDBPath.string().c_str());

    size_t maxSize = ZSTD_compressBound(src.size());
    std::vector<char> dst(maxSize);

    size_t csize = ZSTD_compress(dst.data(), dst.size(), src.data(), src.size(), 15);

    dst.resize(csize);
    writeFile(resultPath.string().c_str(), dst);
}
