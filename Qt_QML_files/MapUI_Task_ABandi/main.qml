import QtQuick 2.6
import QtQuick.Window 2.2
import QtLocation 5.6
import QtPositioning 5.6
import QtQuick.Controls 1.3

Window {
    visible: true
    width: 1200
    height: 480
    title: qsTr("NAVI UI")

    // Map zoom in and zoom out function
    function mapZooming(zoom){
        var ret = 0;
        if(zoom ==="ZoomIn")
        {
            map.zoomLevel++;
            ret = 1;
        }
        else if(zoom ==="ZoomOut")
        {
            map.zoomLevel--;
            ret = 1;
        }
        else
        {
            ret = 0;
        }
        return(ret)
    }

    // Map navigation function
    function mapNavigate(event){
        var dx = 0;
        var dy = 0;
        var ret = 0;

            if(event === "NavLeft"){
                dx = map.width / 4;
                ret = 1;
            }
            else if (event === "NavRight"){
                dx = -map.width / 4;
                ret = 1;
            }
            else if (event === "NavUp"){
                dy = map.height / 4;
                ret = 1;
            }
            else if (event === "NavDown"){
                dy = -map.height / 4;
                ret = 1;
            }
            else
                ret = 0;

        var mapCenterPoint = Qt.point(map.width / 2.0 - dx, map.height / 2.0 - dy);
        map.center = map.toCoordinate(mapCenterPoint);
            return (ret)
    }

    //Address and POI overlay open and clear function
    function overlayStatus(status) {
        var ret = 0;
            if(status)
            {
                overlay.visible = true;
                ret = 1;
            }
            else
            {
                overlay.visible = false;
                ret = 0;
            }
            return(ret)
    }


    Item {
        Timer {
            interval: 50;
            running: true;
            repeat: true
            onTriggered: {
                    var serialData = serialHMI.readSerial(1) // Reads serial data for every 50ms.
                                                             // serialHMI.readSerial(1) sets the AUTO simulation mode
                                                             // serialHMI.readSerial(0) sets the serial data input mode, where input comes from keypad

                    if(serialData === "NavOverlay")          // Checking for Nav options overlay
                    {
                        var overlayActive = overlayStatus(1);
                    }
                    else if(serialData === "ClearOverlay")   // Clears overlay if its already active
                    {
                        overlayActive = overlayStatus(0);
                    }
                    else if( overlay.visible==false)         // Does the map zoom and navigation function
                    {
                        if(mapZooming(serialData));
                        else if(mapNavigate(serialData));
                    }
            }
        }
    }

   MainForm {
        anchors.fill: parent

    Plugin{
        id: osmPlugin
        name: "osm"
    }
    // Initializes map and sets the default location to Sunnyvale CA and zoomlevel to 15
    Map {
        id: map
        anchors.fill: parent
        plugin: osmPlugin
        center: QtPositioning.coordinate(37.386099, -122.036423)  // Sunnyvale, CA
        zoomLevel: 15
    }

    //Inserts a black dial plate image on top of maps
    Image {
        id: backer
        source: "files/images/MapBezel.svg"
        width: Window.width
        height: Window.height
    }

    //Insets the overlay backer with static address and POI fields
    Image {
        id: overlay
        source: "files/images/Overlay.png"
        anchors.centerIn: parent
        visible: false

        Image {
            id: destinationLogo
            source: "files/images/compass.png"
            x: 20
            y: 40

            TextField {
                id: destinationText
                text:"Enter your destination here"
                width: 300
                x: 55
                y: 15
            }

        }

        Image {
            id:imagePOI
            source: "files/images/POI.png"
            x: 20
            y: 100

            Button{
                 id: poiButton
                 text: "Points of Interest"
                 x: 55
                 y: 15
            }
        }
    }

}

}
