set(LIBRARY_NAME ariadnedl-dnn)

add_library(${LIBRARY_NAME} OBJECT
    layer.cpp
    model.cpp
    dense.cpp
    cce_loss.cpp
    mse_loss.cpp
    gd_optimizer.cpp
)

if(COVERAGE)
    target_link_libraries(${LIBRARY_NAME} PUBLIC coverage_config)
endif()