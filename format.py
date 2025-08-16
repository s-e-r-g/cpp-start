#!/usr/bin/env python3
import subprocess
import pathlib
import sys

try:
    import pathspec
except ImportError:
    print("Missing dependency: pathspec (install with `pip install pathspec`)")
    sys.exit(1)

# Load .gitignore patterns
gitignore = pathlib.Path(".gitignore")
spec = None
if gitignore.exists():
    with gitignore.open() as f:
        spec = pathspec.PathSpec.from_lines("gitwildmatch", f)

# File extensions we want to format
extensions = {".c", ".cc", ".cpp", ".h", ".hpp"}

# Collect source files
files = []
for p in pathlib.Path(".").rglob("*"):
    if not p.is_file():
        continue
    if spec and spec.match_file(str(p)):
        continue
    if p.suffix in extensions:
        files.append(str(p))

if not files:
    print("No source files found.")
else:
    print(f"Formatting {len(files)} files...")
    subprocess.run(["clang-format", "-i", *files])
