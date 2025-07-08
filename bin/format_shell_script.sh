#!/usr/bin/env bash
#
# SPDX-FileCopyrightText: © 2022-2025 franklin <franklin@bitsmasher.net>
#
# SPDX-License-Identifier: GPL-3.0-or-later

# This script formats shell scripts using shfmt, ensuring consistent style.
# It checks for shfmt installation, installs it if missing, and then
# applies formatting to specified files and directories.

# Enable strict mode for robust scripting:
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error.
# -o pipefail: The return value of a pipeline is the status of the last command
#              to exit with a non-zero status, or zero if all commands exit
#              successfully.
set -euo pipefail

IFS=$'\n\t'

# --- Configuration ---

# Define ANSI color codes for better output readability
RED='\033[0;31m'    # Red text
YELLOW='\033[1;33m' # Yellow text
NC='\033[0m'        # No Color - resets text color

# --- Functions ---
install_shfmt() {
  echo "Checking for shfmt..."
  if ! command -v shfmt &>/dev/null; then
    echo "shfmt not found. Attempting to install via webi.sh..."
    curl -sS https://webi.sh/shfmt | sh

    # webi.sh typically installs to ~/.local/bin. Ensure it's in PATH for immediate use.
    if [ -f "${HOME}/.local/bin/shfmt" ]; then
      export PATH="${HOME}/.local/bin:${PATH}"
    fi
  fi

  # Verify shfmt is now available and set its path
  if command -v shfmt &>/dev/null; then
    MY_SHFMT=$(which shfmt)
    echo "shfmt found at: ${MY_SHFMT}"
  else
    echo -e "${RED}ERROR: shfmt could not be found or installed automatically.${NC}"
    echo -e "${RED}Please install shfmt manually (e.g., 'curl -sS https://webi.sh/shfmt | sh') and ensure it's in your PATH.${NC}"
    exit 1
  fi
}

check_git_repo() {
  if [ ! -d "./.git" ]; then
    echo -e "${RED}ERROR: ${YELLOW}This script must be run from the top-level directory of your Git repository.${NC}"
    exit 1
  fi
}

format_script() {
  local filename="$1"
  if [ -f "${filename}" ]; then
    echo "Formatting file: ${filename}"
    # shfmt options:
    # -i 2: Indent by 2 spaces.
    # -l: List files whose formatting would change (useful for CI/CD checks).
    # -w: Write the result back to the source file (in-place formatting).
    "${MY_SHFMT}" -i 2 -l -w "${filename}"
  else
    echo -e "${YELLOW}Warning: File not found, skipping: ${filename}${NC}"
  fi
}

check_git_repo
install_shfmt
#echo "--- Formatting specific files ---"
#format_script "bin/bootstrap.sh"

echo "" # Add a newline for better spacing
echo "--- Formatting scripts in 'bin/' directory ---"
for filename in bin/*.sh; do
  echo "$filename"
  format_script "${filename}"
done

echo "" # Add a newline for better spacing
echo "--- Formatting scripts in 'src/fyp_dist' directory ---"
for filename in src/fyp_dist/*.sh; do
  format_script "${filename}"
done

echo "---"
echo -e "${YELLOW}All specified shell scripts have been checked and formatted.${NC}"
echo "If any changes were made, they should now be staged or committed."
