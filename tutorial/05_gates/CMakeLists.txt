include_directories(../../include)
set(tutorial gates)
add_executable(${tutorial} ${tutorial}.cc)
target_link_libraries(${tutorial} utility itensor matrix)
