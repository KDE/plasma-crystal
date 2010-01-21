project(plasma-crystal)

find_package(Nepomuk REQUIRED)

set(crystal_SRCS
    crystal.cpp
    crystaldialog.cpp
    )

kde4_add_ui_files(crystal_SRCS crystalConfig.ui )
kde4_add_plugin(plasma_applet_crystal ${crystal_SRCS})

target_link_libraries(
    plasma_applet_crystal
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KIO_LIBS}
    ${NEPOMUK_LIBRARIES}
)

install(TARGETS plasma_applet_crystal DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-crystal.desktop DESTINATION ${SERVICES_INSTALL_DIR})
