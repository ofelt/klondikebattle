set( klondikebattle_themes
   ancientegypt.desktop
   ancientegypt.svgz
   ancientegypt.png

   cleangreen.desktop
   cleangreen.svgz
   cleangreen.png

   greenblaze.desktop
   greenblaze.svgz
   greenblaze.png

   oldertheme.desktop
   oldertheme.svgz
   oldertheme.png
)

install( FILES ${klondikebattle_themes} DESTINATION  ${KDE_INSTALL_DATADIR}/klondikebattle/themes )

install( FILES klondikebattle.knsrc  DESTINATION  ${KDE_INSTALL_CONFDIR} )

