#!/bin/bash

set -e

# Directory where the script is located
script_dir="$(cd "$(dirname "$0")" && pwd)"
echo "script_dir: $script_dir"

latest_tag=$(curl -s https://api.github.com/repos/humbertodias/openmugen/releases/latest | grep '"tag_name":' | sed -E 's/.*"tag_name": "(.*)".*/\1/')
echo "Latest tag: $latest_tag"

# Download and extract data.zip if the resource directory doesn't exist
if [ ! -d "$script_dir/data" ]; then
    echo "Downloading data.zip..."
    curl -L https://github.com/humbertodias/openmugen/releases/download/$latest_tag/data.zip -o "$script_dir/data.zip"
    unzip "$script_dir/data.zip" -d "$script_dir" && rm "$script_dir/data.zip"
fi

os=`uname -s`
arch=`uname -m`
executable="$script_dir/OpenMugen"
if [ ! -f "$executable" ]; then
    echo "OpenMugen executable not found, downloading latest release..."

    release_url="https://github.com/humbertodias/openmugen/releases/download/$latest_tag/OpenMugen-$os-$arch.tar.gz"
    echo "Downloading $release_url"
    curl -L "$release_url" -o "$script_dir/OpenMugen.tar.gz"

    echo "Extracting OpenMugen.tar.gz..."
    tar -xzf "$script_dir/OpenMugen.tar.gz" -C "$script_dir" OpenMugen
    rm "$script_dir/OpenMugen.tar.gz"

    chmod +x "$executable"
fi

cd $script_dir
./OpenMugen
