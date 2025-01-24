# ? Link to source code
set(GIT_REPOSITORY "https://github.com/madler/zlib.git")

# ? Define JPEG git branch
set(GIT_TAG "v1.3.1")

message(STATUS "\tDownloading ZLib branch: ${GIT_TAG} from ${GIT_REPOSITORY}...")

ExternalProject_Add(
	ZLIB
	PREFIX ${CMAKE_BINARY_DIR}/ZLIB
	GIT_REPOSITORY "${GIT_REPOSITORY}"
	GIT_TAG "${GIT_TAG}"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${ZLIB_INSTALL_DIR}
	UPDATE_DISCONNECTED 1 # Optional: Disconnect from updates
)
