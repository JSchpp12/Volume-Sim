
import bpy, sys, os, time, shutil

# --- simple arg parsing after `--`
def parse_args(argv):
    out = {"out": None, "start": None, "end": None}
    i = 0
    while i < len(argv):
        if argv[i] == "--out":
            out["out"] = argv[i+1]; i += 2
        elif argv[i] == "--start":
            out["start"] = int(argv[i+1]); i += 2
        elif argv[i] == "--end":
            out["end"] = int(argv[i+1]); i += 2
        else:
            i += 1
    return out

if "--" in sys.argv:
    args = parse_args(sys.argv[sys.argv.index("--")+1:])
else:
    args = {"out": None, "start": None, "end": None}

# 1) Ensure the .blend file is saved
if not bpy.data.filepath:
    raise RuntimeError("Blend file must be saved before baking.")

# Defaults / paths
cache_dir = args["out"] or os.path.join(os.path.dirname(bpy.data.filepath), "cache_fluid")

# If exists, remove & recreate
if os.path.exists(cache_dir):
    try:
        parent_dir = os.path.abspath(os.path.join(cache_dir, os.path.pardir))
        archive_dir = os.path.join(parent_dir, "archive_cache")
        if (os.path.exists(archive_dir)):
            shutil.rmtree(archive_dir)
            
        shutil.move(cache_dir, archive_dir)
    except FileNotFoundError:
        print("File not found")
        exit
    except Exception as e:
        print(f"An unknown error occurred: {e}")
        exit
        
os.makedirs(cache_dir, exist_ok=True)

scene = bpy.context.scene
if args["start"] is not None: scene.frame_start = args["start"]
if args["end"]   is not None: scene.frame_end   = args["end"]

# 2) Find fluid GAS domains and set cache settings
domains = []
for obj in bpy.data.objects:
    for mod in obj.modifiers:
        if mod.type == 'FLUID' and getattr(mod, "fluid_type", None) == 'DOMAIN':
            ds = mod.domain_settings
            if ds.domain_type == 'GAS':
                ds.cache_directory = cache_dir
                ds.resolution_max = 512
                ds.cache_frame_start = scene.frame_start
                ds.cache_frame_end = scene.frame_end
                ds.cache_type = 'ALL'
                ds.cache_data_format = 'OPENVDB'
                domains.append((obj, mod))

if not domains:
    print("No GAS fluid domains found; nothing to bake.")
    sys.exit(0)

# 4) Bake per domain
for obj, mod in domains:
    bpy.context.view_layer.objects.active = obj
    print(f"Baking domain on: {obj.name} → {mod.domain_settings.cache_directory}")
    start_time = None  # reset timers for each domain
    last_frame = None
    result = bpy.ops.fluid.bake_all()
    print("Bake result:", result)

print("Done. VDBs are in:", cache_dir, "/data and possibly /noise.")
