project( klondikebattle )

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)
set (QT_MIN_VERSION "5.2.0")
set (KF5_MIN_VERSION "5.15.0")

find_package(ECM 1.7.0 REQUIRED CONFIG)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

find_package(Qt5 ${QT_MIN_VERSION} REQUIRED NO_MODULE COMPONENTS Widgets Qml Quick QuickWidgets Svg Test)
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS 
    Completion 
    Config
    ConfigWidgets 
    CoreAddons 
    Crash
    DBusAddons 
    Declarative
    DocTools
    I18n 
    GuiAddons 
    ItemViews 
    IconThemes 
    TextWidgets
    KDELibs4Support
    KIO
    NotifyConfig
    NewStuff
    WidgetsAddons
    XmlGui)

find_package(KF5KDEGames 4.9.0 REQUIRED)

find_package(Phonon4Qt5)

include(FeatureSummary)
include(ECMAddAppIcon)
include(ECMInstallIcons)
include(KDEInstallDirs)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings)

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
add_definitions(-DTRANSLATION_DOMAIN="klondikebattle")

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/libkcardgame/include )

add_subdirectory( icons )
add_subdirectory( libkcardgame )
add_subdirectory( mimetypes )
add_subdirectory( previews )
add_subdirectory( sounds )
add_subdirectory( themes )
add_subdirectory( doc )

set( klondikebattle_SRCS
    main.cpp
    dealer.cpp
    dealerinfo.cpp
    gameselectionscene.cpp
    mainwindow.cpp
    messagebox.cpp
    numbereddealdialog.cpp
    patpile.cpp
    pileutils.cpp
    renderer.cpp
    soundengine.cpp
    statisticsdialog.cpp
    view.cpp
    patsolve/memory.cpp
    patsolve/patsolve.cpp
    player.cpp
    russianbank.cpp
)

ki18n_wrap_ui( klondikebattle_SRCS statisticsdialog.ui players.ui)
kconfig_add_kcfg_files( klondikebattle_SRCS settings.kcfgc )

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/icons/*-apps-kpat.png")
ecm_add_app_icon(klondikebattle_SRCS ICONS ${ICONS_SRCS})
add_executable( klondikebattle ${klondikebattle_SRCS} )
target_link_libraries( klondikebattle kcardgame KF5KDEGames KF5::KIOCore Phonon::phonon4qt5 KF5::I18n KF5::DBusAddons KF5::KDELibs4Support)
install( TARGETS klondikebattle ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} )


########### install files ###############

install( PROGRAMS org.kde.klondikebattle.desktop  DESTINATION  ${KDE_INSTALL_APPDIR} )
install( FILES klondikebattleui.rc  DESTINATION  ${KDE_INSTALL_KXMLGUI5DIR}/klondikebattle )
install( FILES klondikebattle.kcfg DESTINATION ${KDE_INSTALL_KCFGDIR} )
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
