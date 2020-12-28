# Verify that the library is usable in an external project.

set -e
SOURCE_DIR="${PWD}"
BUILD_DIR=${PYPP_BUILD_DIR}/test/integration
mkdir -p "${BUILD_DIR}" || true
cd "${BUILD_DIR}"
cmake -DCMAKE_BUILD_TYPE=Debug "${SOURCE_DIR}"
cmake --build .
./test_pypp
