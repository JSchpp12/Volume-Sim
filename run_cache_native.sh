#!/bin/bash

GIT_ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PY_SCRIPT="${GIT_ROOT_DIR}/scripts/cache_fluid.py"
PROJ_DIR="${GIT_ROOT_DIR}/samples/flat_plain"
OUT_DIR="${PROJ_DIR}/cache/vdb"
BLEND_FILE="${PROJ_DIR}/Smoke_Simulation.blend"

mkdir -p tmp
cd tmp
if [[ -d blender-5.0.1-linux-x64 ]]; then
        echo "Skipping download"
else

        wget https://download.blender.org/release/Blender5.0/blender-5.0.1-linux-x64.tar.xz
        tar -xf blender-5.0.1-linux-x64.tar.xz
        rm blender-5.0.1-linux-x64.tar.xz
fi

cd blender-5.0.1-linux-x64

# --- Custom arguments ---
START_FRAME=0
END_FRAME=250

./blender -b "${BLEND_FILE}" --python "${PY_SCRIPT}" -- \
        --out "${OUT_DIR}" \
        --start "${START_FRAME}"
        --stop "${END_FRAME}"