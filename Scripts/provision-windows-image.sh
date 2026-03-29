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

echo "Updating Visual Studio Installer catalog..."
"$VS_INSTALLER" update --quiet --wait

echo "Adding VS component: $COMPONENT_ID to $VS_INSTALL_PATH"
"$VS_INSTALLER" modify --installPath "$VS_INSTALL_PATH" --add "$COMPONENT_ID" --quiet --norestart --wait

EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ] || [ $EXIT_CODE -eq 3010 ]; then
    echo "Visual Studio component installation completed successfully (Exit Code: $EXIT_CODE)."
else
    echo "Visual Studio component installation failed with Exit Code: $EXIT_CODE."
    
    # Dump the log if it fails
    LOG_FILE=$(ls -t $TEMP/dd_setup_*.log | head -n 1)
    if [ -f "$LOG_FILE" ]; then
        echo "--- BEGIN VS INSTALLER LOG ---"
        cat "$LOG_FILE"
        echo "--- END VS INSTALLER LOG ---"
    fi
    exit 1
fi

# Check the contents using Git Bash's ls
# Convert Windows path backslashes to forward slashes for bash 'ls'
BASH_MSVC_PATH=$(echo "$VS_INSTALL_PATH/VC/Tools/MSVC" | sed 's/\\/\//g' | sed 's/C:/\/c/')
echo "Contents of $BASH_MSVC_PATH:"
ls -l "$BASH_MSVC_PATH"