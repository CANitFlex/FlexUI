#!/bin/bash

# Build script with automatic Qt variable resolution

set -e

# Farben
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Konfiguration
BUILD_TYPE="${1:-Debug}"
QTDIR_INPUT="${2:-}"
BUILD_DIR="build-${BUILD_TYPE}"

echo -e "${YELLOW}Building AndroidCanController (${BUILD_TYPE})${NC}"
echo ""

# Qt-Verzeichnis: User-Input oder default
if [ -z "$QTDIR_INPUT" ]; then
  DEFAULT_QTDIR="/opt/qt/6.9.0/gcc_64"
  echo -e "${BLUE}Qt Verzeichnis eingeben (oder Enter für Default):${NC}"
  echo "  Default: $DEFAULT_QTDIR"
  read -p "  QTDIR: " QTDIR_INPUT
  
  if [ -z "$QTDIR_INPUT" ]; then
    QTDIR_INPUT="$DEFAULT_QTDIR"
  fi
fi

# Qt-Verzeichnis validieren
if [ ! -d "$QTDIR_INPUT" ]; then
  echo -e "${RED}✗ Fehler: Qt-Verzeichnis nicht gefunden: $QTDIR_INPUT${NC}"
  exit 1
fi

# Qt und Compiler automatisch erkennen
CXX_COMPILER=$(which g++ 2>/dev/null || which clang++ 2>/dev/null || echo "g++")
C_COMPILER=$(which gcc 2>/dev/null || which clang 2>/dev/null || echo "gcc")
QT_INSTALL_PREFIX="${QTDIR_INPUT}"

echo -e "${YELLOW}Configuration:${NC}"
echo "  Build Type: $BUILD_TYPE"
echo "  Qt Prefix: $QT_INSTALL_PREFIX"
echo "  C++ Compiler: $CXX_COMPILER"
echo "  C Compiler: $C_COMPILER"
echo "  Build Dir: $BUILD_DIR"
echo ""

# Build-Verzeichnis erstellen
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# CMake Configure
echo -e "${YELLOW}Configuring CMake...${NC}"
cmake -S .. -B . \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  -DCMAKE_COLOR_DIAGNOSTICS=ON \
  -DCMAKE_CXX_COMPILER="$CXX_COMPILER" \
  -DCMAKE_C_COMPILER="$C_COMPILER" \
  -DCMAKE_GENERATOR=Ninja \
  -DCMAKE_PREFIX_PATH="$QT_INSTALL_PREFIX/lib/cmake" \
  -DQT_QMAKE_EXECUTABLE="$QT_INSTALL_PREFIX/libexec/qmake"

# CMake Build
echo -e "${YELLOW}Building...${NC}"
cmake --build . --config "$BUILD_TYPE"

echo -e "${GREEN}✓ Build erfolgreich!${NC}"
echo ""
echo -e "${YELLOW}Starte die App:${NC}"
echo "  ./$BUILD_DIR/AndroidCanController"
