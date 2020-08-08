add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${KDE4_INCLUDES} ${QT_INCLUDES})

set(kded_imchooserstarter_PART_SRCS  imchooserstarter.cpp )

add_library(kded_imchooserstarter MODULE ${kded_imchooserstarter_PART_SRCS})

target_link_libraries(kded_imchooserstarter KF5::KDELibs4Support KF5::DBusAddons)

install(TARGETS kded_imchooserstarter  DESTINATION ${PLUGIN_INSTALL_DIR} )
install( FILES imchooserstarter.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}/kded )