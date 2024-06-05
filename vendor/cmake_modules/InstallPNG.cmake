# ? Link to source code
set(GIT_REPOSITORY "https://github.com/libsdl-org/libpng.git")

# ? Define JPEG git branch
set(GIT_TAG "v1.6.41-SDL")

message(STATUS "\tDownloading PNG branch: ${GIT_TAG} from ${GIT_REPOSITORY}...")

ExternalProject_Add(
	PNG
	PREFIX ${CMAKE_BINARY_DIR}/PNG
	GIT_REPOSITORY "${GIT_REPOSITORY}"
	GIT_TAG "${GIT_TAG}"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${PNG_INSTALL_DIR}
	UPDATE_DISCONNECTED 1 # Optional: Disconnect from updates
)
