#!/bin/bash
set -e

echo "Locating Visual Studio installation..."
INSTALLER_DIR="/c/Program Files (x86)/Microsoft Visual Studio/Installer"
VSWHERE="$INSTALLER_DIR/vswhere.exe"
VS_INSTALLER="$INSTALLER_DIR/vs_installer.exe"

if [ ! -f "$VSWHERE" ] || [ ! -f "$VS_INSTALLER" ]; then
    echo "Could not find vswhere.exe or vs_installer.exe in standard location."
    exit 1
fi

# Capture the path and remove trailing carriage returns/newlines
VS_INSTALL_PATH=$("$VSWHERE" -latest -products \* -property installationPath | tr -d '\r\n')

if [ -z "$VS_INSTALL_PATH" ]; then
    echo "Could not locate Visual Studio installation path."
    exit 1
fi

# Trim trailing backslashes if any
VS_INSTALL_PATH="${VS_INSTALL_PATH%\\}"

COMPONENT_ID="Microsoft.VisualStudio.Component.VC.Preview.Tools.x86.x64"

# Use powershell wrapper to ensure -Wait works correctly for these Windows GUI apps
echo "Step 1: Updating Visual Studio Installer catalog for $VS_INSTALL_PATH..."
powershell.exe -NoProfile -Command "Start-Process -FilePath '$VS_INSTALLER' -ArgumentList \"update --installPath '$VS_INSTALL_PATH' --quiet\" -Wait"

echo "Step 2: Adding VS component: $COMPONENT_ID to $VS_INSTALL_PATH"
powershell.exe -NoProfile -Command "Start-Process -FilePath '$VS_INSTALLER' -ArgumentList \"modify --installPath '$VS_INSTALL_PATH' --add $COMPONENT_ID --quiet --norestart\" -Wait"

# Checking the results
# Convert Windows path backslashes to forward slashes for bash 'ls'
BASH_MSVC_PATH=$(echo "$VS_INSTALL_PATH/VC/Tools/MSVC" | sed 's/\\/\//g' | sed 's/C:/\/c/')
echo "Contents of $BASH_MSVC_PATH:"
if [ -d "$BASH_MSVC_PATH" ]; then
    ls -l "$BASH_MSVC_PATH"
else
    echo "Path not found: $BASH_MSVC_PATH"
fi