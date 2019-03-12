set(LAUNCHER_NAME "Launch")
add_executable(
    ${LAUNCHER_NAME}
    "main.cpp"
)

execute_process(COMMAND bash -c "touch ${LEDLIB_LAUNCHER_ARGS_PATH}")
execute_process(COMMAND bash -c "chmod 666 \"${MATLIB_LAUNCHER_ARGS_PATH}\"")
