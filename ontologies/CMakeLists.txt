# include(NepomukAddOntologyClasses)
# include_directories(${SOPRANO_INCLUDE_DIR})
# 
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
# 
# nepomuk_add_ontology_classes(
#   nie_apps_SRCS
#   ONTOLOGIES
#   ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
#   CLASSES 
#   "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image"
#   "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Website"
#   )
# 
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nie.trig "NIE" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nco.trig "NCO" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nmo.trig "NMO" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nfo/nfo.trig "PersonContact" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nfo/nfo.trig "Email" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nfo/nfo.trig "Image" "Vocabulary" "trig")
# soprano_add_ontology(nie_apps_SRCS ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nfo/nfo.trig "Video" "Vocabulary" "trig")
# 
# kde4_add_library(niefast_apps STATIC ${nie_apps_SRCS})
# target_link_libraries(niefast_apps ${QT_QTCORE_LIBRARY} ${SOPRANO_LIBRARIES} ${NEPOMUK_LIBRARIES})

nepomuk_add_ontology_classes(
  crystal_SRCS
  ONTOLOGIES
  ${SHAREDDESKTOPONTOLOGIES_ROOT_DIR}/nie/nfo.trig
  CLASSES 
  #"http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Email"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nmo#Email"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Image"
  #"http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#PersonContact"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nco#PersonContact"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Website"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Video"
  "http://www.semanticdesktop.org/ontologies/2007/03/22/nfo#Visual"
  )

