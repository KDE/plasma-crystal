project(plasma-crystal)

find_package(Nepomuk REQUIRED)

set(crystal_SRCS
    stylesheet.cpp
    applet.cpp
    dialog.cpp
    dashboard.cpp
    resultview.cpp
    resultwidget.cpp
    resultwebview.cpp
    resourcewidget.cpp
    imageresourcewidget.cpp
    imagewidget.cpp
    ratingwidget.cpp
    utils.cpp
    )

kde4_add_ui_files(crystal_SRCS crystalConfig.ui )
kde4_add_plugin(plasma_applet_crystal ${crystal_SRCS})

target_link_libraries(
    plasma_applet_crystal
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KIO_LIBS}
    ${NEPOMUK_LIBRARIES}
    ${QT_QTWEBKIT_LIBRARY}
)

install(TARGETS plasma_applet_crystal DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-crystal.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES user.css DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES nepomuk.png DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES dashboard.css DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES dashboard.html DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
