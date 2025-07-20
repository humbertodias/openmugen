# Get the directory where the script is located
$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Write-Host "script_dir: $scriptDir"

$latestReleaseUrl = "https://api.github.com/repos/humbertodias/openmugen/releases/latest"
$latestTag = (Invoke-RestMethod -Uri $latestReleaseUrl).tag_name
Write-Host "Latest tag: $latestTag"

# Download and extract data.zip if the data directory doesn't exist
if (-not (Test-Path "$scriptDir\data")) {
    Write-Host "Downloading data.zip from release $latestTag..."
    $downloadUrl = "https://github.com/humbertodias/openmugen/releases/download/$latestTag/data.zip"
    Invoke-WebRequest -Uri $downloadUrl -OutFile "$scriptDir\data.zip"
    
    # Unzip the file into the script's directory
    Write-Host "Extracting data.zip..."
    Expand-Archive -Path "$scriptDir\data.zip" -DestinationPath "$scriptDir"
    Remove-Item "$scriptDir\data.zip"
}

cd "$scriptDir"
# Launch the application with the data directory argument
Write-Host "Launching application..."
Start-Process -FilePath $mainPath.FullName -ArgumentList "-d `"$scriptDir\data`""
