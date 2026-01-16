#!/usr/bin/env bash
set -euo pipefail

# --- Resolve directory of this script ---
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="samples/flat_plain"

# --- User-editable config ---
BLEND_FILE="${PROJECT_DIR}/Smoke_Simulation.blend"  # path to your .blend
PY_SCRIPT="scripts/cache_fluid.py"

# --- Custom arguments ---
OUT_DIR="${PROJECT_DIR}/cache/vdb"
START_FRAME=1
END_FRAME=120

# --- Run Blender in Docker ---
docker run --rm \
  -v "${SCRIPT_DIR}:/work" \
  -w /work \
  "linuxserver/blender:5.0.1" \
  blender --background "${BLEND_FILE}" \
    --python "${PY_SCRIPT}" -- \
      --out "${OUT_DIR}" \
      --start "${START_FRAME}" \
      --end "${END_FRAME}"
