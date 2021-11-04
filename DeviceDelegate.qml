
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import App 1.0

Rectangle {
    height: 40
    color: 'darkgrey'

    RowLayout {
        id: idActionTitleRow
        height: 28
        anchors.fill: parent
        anchors.leftMargin: 4

        Rectangle {
            width: 20
            height: 20
            radius: 10
            color: {
                if (modelData.state === Device.Nominal) {
                    return "green";
                } else if (modelData.state === Device.Warning) {
                    return "orange";
                } else if (modelData.state === Device.Error) {
                    return "red";
                } else {
                    console.log(modelData.state)
                    return "black";
                }
            }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Text {
            text: {
                let type = 'UNKNOWN';
                if (modelData.type === Device.GPS)
                {
                    type = "[GPS] - "
                }
                else if (modelData.type === Device.Gyro)
                {
                    type = "[GYRO] - "
                }
                else if (modelData.type === Device.Compass)
                {
                    type = "[COMPASS] - "
                }
                else if (modelData.type === Device.Anemometer)
                {
                    type = "[ANEMOMETER] - "
                }


                return type + modelData.name
            }
            font.pixelSize: 18
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Text {
            Layout.rightMargin: 4
            text:  (modelData.strength < 0 ? "--" : modelData.strength) + ' %'
            font.pixelSize: 18
        }
    }
}
