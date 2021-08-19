cmake_minimum_required(VERSION 3.0.0)

if (WIN32)

    MESSAGE(STATUS "Now is windows")
    # CUDA
    find_package(CUDA REQUIRED)
    MESSAGE(${CUDA_INCLUDE_DIRS})
    # OpenCV
    find_package(OpenCV 3.4.4 REQUIRED PATHS D:/3rd_party/opencv344/build_gpu/install)

    # TensorRT
    set(TENSORRT_ROOT D:/TensorRT-7.0.0.11)
    set(TENSORRT_INCLUDE_DIR ${TENSORRT_ROOT}/include)
    set(TENSORRT_LIB_DIR ${TENSORRT_ROOT}/lib)

    #
    # aux_source_directory(. LIB_SRC)
    file(GLOB_RECURSE LIB_SRC ./*.cpp ./*.cu)

    # message(${LIB_SRC})
    add_library(mocap SHARED ${LIB_SRC} )

    target_include_directories(mocap 
                PUBLIC ${CUDA_INCLUDE_DIRS}
                PUBLIC ${TENSORRT_INCLUDE_DIR}
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extra/detector
                PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/extra/hmr
                PUBLIC ${OpenCV_INCLUDE_DIRS})

    target_link_libraries(mocap ${OpenCV_LIBS} 
                          ${CMAKE_CURRENT_SOURCE_DIR}/extra/detector/detector.lib
                          ${CMAKE_CURRENT_SOURCE_DIR}/extra/hmr/hmr.lib
                          ${TENSORRT_LIB_DIR}/nvinfer.lib 
                          ${CUDA_LIBRARIES})

endif()

# cd build 
# make install
# install(FILES class_human3d.h DESTINATION include)
# install(TARGETS mocap LIBRARY DESTINATION lib)
# install(FILES extra/yolov5s_fp16_b1.engine DESTINATION lib)
# install(FILES extra/hmr_fp16_b1.engine DESTINATION lib)
# install(FILES extra/smpl_male.json DESTINATION lib)
# install(FILES extra/libdetector.so DESTINATION lib)
# install(FILES extra/libhmr.so DESTINATION lib)
# install(FILES ../data/im1010.jpg DESTINATION .)
