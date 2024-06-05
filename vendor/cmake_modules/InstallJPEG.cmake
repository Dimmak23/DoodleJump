# ? Link to source code
set(GIT_REPOSITORY "https://github.com/libsdl-org/jpeg.git")

# ? Define JPEG git branch
set(GIT_TAG "v9e-SDL")

message(STATUS "\tDownloading JPEG branch: ${GIT_TAG} from ${GIT_REPOSITORY}...")

ExternalProject_Add(
	JPEG
	PREFIX ${CMAKE_BINARY_DIR}/JPEG
	GIT_REPOSITORY "${GIT_REPOSITORY}"
	GIT_TAG "${GIT_TAG}"
	CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${JPEG_INSTALL_DIR}
	UPDATE_DISCONNECTED 1 # Optional: Disconnect from updates
)
