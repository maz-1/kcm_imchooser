set(kcm_SRCS
    main.cpp
    module.cpp
    profilegenerator.cpp
    improfile.cpp
    improfilemodel.cpp
)

find_package(Gettext REQUIRED)

ki18n_wrap_ui(kcm_SRCS module.ui)

add_library(kcm_imchooser MODULE ${kcm_SRCS})

target_link_libraries(kcm_imchooser
    KF5::KDELibs4Support
)

install(TARGETS kcm_imchooser DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES kcm_imchooser.desktop DESTINATION ${SERVICES_INSTALL_DIR})
