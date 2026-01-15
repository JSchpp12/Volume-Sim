
import bpy, sys, os, time

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

# ----- PROGRESS HANDLER SETUP -----
start_time = None
last_frame = None

def progress_handler(scene):
    global start_time, last_frame

    f = scene.frame_current
    fs = scene.frame_start
    fe = scene.frame_end

    if start_time is None:
        start_time = time.time()

    # Avoid repeated prints on same frame
    if last_frame == f:
        return
    last_frame = f

    total = fe - fs + 1
    done = f - fs + 1
    pct = (done / total) * 100

    elapsed = time.time() - start_time
    if done > 0:
        est_total = elapsed * (total / done)
        eta = est_total - elapsed
    else:
        eta = 0

    print(f"[Bake Progress] Frame {f}/{fe}  ({pct:5.1f}%)  ETA: {eta:6.1f}s")

# Register handler
if progress_handler not in bpy.app.handlers.frame_change_post:
    bpy.app.handlers.frame_change_post.append(progress_handler)

# Defaults / paths
cache_dir = args["out"] or os.path.join(os.path.dirname(bpy.data.filepath), "cache_fluid")

# If exists, remove & recreate
if os.path.exists(cache_dir):
    try:
        os.rmdir(cache_dir)
    except:
        # rmdir fails if not empty; clear manually
        import shutil
        shutil.rmtree(cache_dir)

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
