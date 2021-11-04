import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.1
import QtQuick.Layouts 1.12
import App 1.0
import "."

Window {
    id: idMainWindow
    width: 1024
    height: 768
    visible: true
    title: qsTr("Sensors List")

    Rectangle {
        id: idDeviceContainer
        color: "grey"
        anchors.fill: parent

        Component.onCompleted:  {
            console.log(sensor_model.devices.length)
        }

        Rectangle {
            color: "transparent"
            border.width: 2
            border.color: "black"
            anchors.horizontalCenter: idDeviceContainer.horizontalCenter
            width: 500
            anchors.margins: 10
            height: 600

            ListView {
                id: idDevicesListView
                anchors.fill: parent
                anchors.margins: 10
                clip: true
                ScrollBar.vertical: ScrollBar {}
                model: sensor_model.devices
                spacing: 4
                delegate: DeviceDelegate {
                    width: idDevicesListView.width
                }
            }
        }

    }


}
