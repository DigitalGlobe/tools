set(PROGRAMS
    projinfo
)

set(INSTALL_DIR "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/share/bash-completion/completions")

file(MAKE_DIRECTORY "${INSTALL_DIR}")

foreach (program IN LISTS PROGRAMS)
  message(STATUS "Installing ${INSTALL_DIR}/${program}")
  configure_file("D:/Users/tim.tisler/tools/build/proj/scripts/${program}-bash-completion.sh" "${INSTALL_DIR}/${program}" COPYONLY)
  file(APPEND "D:/users/tim.tisler/tools/build/proj/build/install_manifest_extra.txt" "${INSTALL_DIR}/${program}\n")
endforeach ()
