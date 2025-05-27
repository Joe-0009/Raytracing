#!/bin/bash

# Cleanup debug prints from miniRT codebase
# This script removes all DEBUG printf statements for production version

echo "Cleaning up debug prints from miniRT codebase..."

# Files to clean up
FILES=(
    "src/main.c"
    "src/parser/parse_file.c"
    "src/parser/parse_elements.c"
    "src/render/draw.c"
)

# Create backup directory
mkdir -p backups
backup_date=$(date +"%Y%m%d_%H%M%S")

echo "Creating backups in backups/backup_${backup_date}/"
mkdir -p "backups/backup_${backup_date}"

# Backup files before modification
for file in "${FILES[@]}"; do
    if [ -f "$file" ]; then
        cp "$file" "backups/backup_${backup_date}/$(basename $file)"
        echo "  Backed up: $file"
    fi
done

# Function to remove debug prints from a file
cleanup_file() {
    local file="$1"
    if [ ! -f "$file" ]; then
        echo "Warning: File $file not found"
        return
    fi
    
    echo "Cleaning $file..."
    
    # Remove lines that contain DEBUG printf statements
    # These patterns match the debug prints found in the code
    sed -i '' '/printf("DEBUG:/d' "$file"
    sed -i '' '/printf("DEBUG /d' "$file"
    sed -i '' '/printf.*DEBUG.*$/d' "$file"
    
    # Also remove any remaining standalone debug printf lines
    sed -i '' '/^\s*printf.*DEBUG.*);$/d' "$file"
    
    echo "  ✓ Cleaned debug prints from $file"
}

# Clean up each file
for file in "${FILES[@]}"; do
    cleanup_file "$file"
done

echo ""
echo "Debug cleanup completed!"
echo "Backups saved in: backups/backup_${backup_date}/"
echo ""
echo "To rebuild the project without debug output:"
echo "  make clean && make"
echo ""
echo "To restore from backup if needed:"
echo "  cp backups/backup_${backup_date}/* src/"
echo "  cp backups/backup_${backup_date}/* src/parser/"
echo "  cp backups/backup_${backup_date}/* src/render/"
