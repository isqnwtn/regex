add_executable(app
    main.cpp
)

# Link the app to the library
target_link_libraries(app PRIVATE lib)
