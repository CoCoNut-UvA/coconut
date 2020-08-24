#!/usr/bin/env bash

COCONUT_DIR=$(dirname $(realpath "$0"))
COCOGEN_EXECUTABLE="$COCONUT_DIR/build/cocogen/cocogen"
BUILD_DIR="${COCONUT_DIR}"/build



function build_cocogen {
  mkdir -p "${BUILD_DIR}"
  cmake -S "${COCONUT_DIR}" -B "${BUILD_DIR}"
  make -C "${BUILD_DIR}"
}

echo "Select an action to execute by typing its number"
select action in build gen clean help quit
do
  case $action in
    build)
      build_cocogen
      ;;
    gen)
      if [[ -f "${COCOGEN_EXECUTABLE}" ]]; then
        read -p "target directory: " TARGET_DIRECTORY
        mkdir -p "${TARGET_DIRECTORY}"/ccngen/ccngen
        cp "${COCONUT_DIR}"/copra/src/*c "${TARGET_DIRECTORY}"/ccngen
        cp -r ${COCONUT_DIR}/copra/ccn/ "${TARGET_DIRECTORY}"/ccngen/
        $COCOGEN_EXECUTABLE
      else
        echo "Could not find the cocogen executable."
        echo "Did you run the build command?"
      fi
      ;;
    clean)
      make clean -C "${BUILD_DIR}"
      ;;
    help)
      echo "Options:"
      echo "- build: build the cocogen executable in the right directory."
      echo "- gen: executes cocogen."
      echo "- help: this help message"
      echo "- clean: does a clean of the cocogen executable. Requires a build command to use cocogen again"
      echo "- quit: quit the repl"
      ;;
    quit)
      exit 0
  esac
done