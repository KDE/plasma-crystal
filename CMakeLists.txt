project(plasma-crystal)

find_package(Nepomuk REQUIRED)

find_package(Soprano)
  macro_log_feature(Soprano_FOUND "Soprano" "Semantic Desktop Storing" "http://soprano.sourceforge.net" TRUE "${SOPRANO_MIN_VERSION}" "Soprano is needed for Nepomuk")



include(SopranoAddOntology)


include_directories(
  ${CMAKE_BINARY_DIR}/ontologies)

#  ${CMAKE_SOURCE_DIR}/ontologies)

#include_directories(/home/sebas/kdesvn/build/kdepim/runtime/agents/ontologies/)


set(crystal_SRCS
    stylesheet.cpp
    applet.cpp
    dialog.cpp
    dashboard.cpp
    resultview.cpp
    resultwidget.cpp
    resultwebview.cpp
    resourcewidget.cpp
    resourcewidgets/imageresourcewidget.cpp
    resourcewidgets/contactwidget.cpp
    resourcewidgets/emailwidget.cpp
    resourcewidgets/videoresourcewidget.cpp
    imagewidget.cpp
    ratingwidget.cpp
    utils.cpp

#     ontologies/nco.cpp
#     ontologies/contact.cpp
#     ontologies/role.cpp
#     ontologies/resource.cpp
#     ontologies/personcontact.cpp
    )

nepomuk_add_ontology_classes(
  crystal_SRCS
  ONTOLOGIES
  ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
  ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmo.trig
  ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
  CLASSES 
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#Email"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#RasterImage"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image"
  #nmo:Email
  #nfo:Image
  #nfo:RasterImage
  #nco:PersonContact
  #nfo:Website
  #nfo:Video
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PersonContact"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Website"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Visual"
  )


kde4_add_ui_files(crystal_SRCS crystalConfig.ui )
kde4_add_plugin(plasma_applet_crystal ${crystal_SRCS})


#add_subdirectory(ontologies)
# nepomuk_add_ontology_classes(
#   crystal_SRCS
#   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig
#   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig
#   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmo.trig
#   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/ncal.trig
#   )
# 
# 
# soprano_add_ontology(crystal_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig "NIE" "Vocabulary" "trig")
#soprano_add_ontology(crystal_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig "NCO" "Vocabulary" "trig")
# 

target_link_libraries(
    plasma_applet_crystal
    ${QT_QTWEBKIT_LIBRARY}
    ${KDE4_PLASMA_LIBS}
    ${KDE4_KIO_LIBS}
    ${NEPOMUK_LIBRARIES}
    ${NEPOMUK_QUERY_LIBRARIES}
    ${SOPRANO_LIBRARIES}
    ${SOPRANO_CLIENT_LIBRARIES}
    #niefast_apps
  ${KDEPIMLIBS_KMIME_LIBS}

)

install(TARGETS plasma_applet_crystal DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-applet-crystal.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES user.css DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES nepomuk.png DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES dashboard.css DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
install(FILES dashboard.html DESTINATION ${DATA_INSTALL_DIR}/plasma-applet-crystal/)
