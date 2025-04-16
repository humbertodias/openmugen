#!/bin/bash

# Directory where the script is located
script_dir="$(cd "$(dirname "$0")" && pwd)"
echo "script_dir: $script_dir"

# Download and extract data.zip if the resource directory doesn't exist
if [ ! -d "$script_dir/data" ]; then
    echo "Downloading resource.zip..."
    curl -L https://github.com/humbertodias/sdl-openmuegen/releases/download/v0.01/data.zip -o "$script_dir/data.zip"
    unzip "$script_dir/data.zip" -d "$script_dir" && rm "$script_dir/data.zip"
fi

cd $script_dir
./OpenMugen