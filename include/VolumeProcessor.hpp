#pragma once

#include <openvdb/io/File.h>
#include <openvdb/openvdb.h>

#define NANOVDB_USE_OPENVDB
#include <nanovdb/NanoVDB.h>
#include <nanovdb/io/IO.h>
#include <nanovdb/tools/CreateNanoGrid.h>

#include <filesystem>

class VolumeProcessor
{
  public:
    explicit VolumeProcessor(std::filesystem::path filePath);

    void toNanoVDB(const std::filesystem::path &outputDir);

  private:
    std::filesystem::path m_filePath;

    void toCompressed(const std::filesystem::path &nanoVDBPath) const;

    template <typename TGrid> bool tryConvert(openvdb::GridBase::Ptr base, std::string outputPath) const
    {
        if (openvdb::SharedPtr<TGrid> g = openvdb::gridPtrCast<TGrid>(base))
        {
            nanovdb::GridHandle<nanovdb::HostBuffer> handle = nanovdb::tools::createNanoGrid(*g);
            nanovdb::io::writeGrid(outputPath, handle, nanovdb::io::Codec::NONE);
            return true;
        }
        return false;
    }
};