# Get the directory where the script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Write-Host "script_dir: $scriptDir"

# Download and extract data.zip if the data directory doesn't exist
if (-not (Test-Path "$scriptDir\data")) {
    Write-Host "Downloading data.zip..."
    Invoke-WebRequest -Uri "https://github.com/humbertodias/sdl-openmugen/releases/download/v0.01/data.zip" -OutFile "$scriptDir\data.zip"
    
    # Unzip the file into the script's directory
    Write-Host "Extracting data.zip..."
    Expand-Archive -Path "$scriptDir\data.zip" -DestinationPath "$scriptDir"
    Remove-Item "$scriptDir\data.zip"
}

cd "$scriptDir"
# Launch the application with the data directory argument
Write-Host "Launching application..."
Start-Process -FilePath $mainPath.FullName -ArgumentList "-d `"$scriptDir\data`""
