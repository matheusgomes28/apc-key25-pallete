pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Window
import QtQuick.Layouts

Window {
    minimumWidth: 320
    minimumHeight: 480
    visible: true
    title: "APC25 Editor"
    id: apc25

    // Enums (cant access through Window for some readon, so variables)
    readonly property int buttonOff:         0 // "white"
    readonly property int buttonGreen:       1 // Qt.hsla(11/40, 0.7, 0.5, 1)
    readonly property int buttonGreenBlink:  2 // Qt.hsla(11/40, 0.7, 0.5, 1)
    readonly property int buttonRed:         3 // Qt.hsla(0, 0.7, 0.5, 1)
    readonly property int buttonRedBlink:    4 // Qt.hsla(0, 0.7, 0.5, 1)
    readonly property int buttonYellow:      5 // Qt.hsla(7/40, 0.7, 0.5, 1)
    readonly property int buttonYellowBlink: 6 // Qt.hsla(7/40, 0.7, 0.5, 1)

    readonly property color colorGreen:  Qt.hsla(0.275, 0.7, 0.5, 1)
    readonly property color colorRed:    Qt.hsla(0, 0.7, 0.5, 1)
    readonly property color colorYellow: Qt.hsla(0.175, 0.7, 0.5, 1)

    readonly property list<QtObject> colors: [
        QtObject{
            property color color: "white"
            property int buttonColor: apc25.buttonOff
            property bool buttonBlink: false
        },
        QtObject{
            property color color: apc25.colorGreen
            property int buttonColor: apc25.buttonGreen
            property bool buttonBlink: false
        },
        QtObject{
            property color color: apc25.colorGreen
            property int buttonColor: apc25.buttonGreenBlink
            property bool buttonBlink: true
        },
        QtObject{
            property color color: apc25.colorRed
            property int buttonColor: apc25.buttonRed
            property bool buttonBlink: false
        },
        QtObject{
            property color color: apc25.colorRed
            property int buttonColor: apc25.buttonRedBlink
            property bool buttonBlink: true
        },
        QtObject{
            property color color: apc25.colorYellow
            property int buttonColor: apc25.buttonYellow
            property bool buttonBlink: false
        },
        QtObject{
            property color color: apc25.colorYellow
            property int buttonColor: apc25.buttonYellowBlink
            property bool buttonBlink: true
        }
    ]

    readonly property list<int> initialColors: [
        // Row 1
        1, 3, 5, 1, 3, 5, 1, 3,
        // Row 2
        1, 3, 5, 1, 3, 5, 1, 3,
        // Row 3
        1, 3, 5, 1, 3, 5, 1, 3,
        // Row 4
        1, 3, 5, 1, 3, 5, 1, 3,
        // Row 5
        1, 3, 5, 1, 3, 5, 1, 3
    ]


    ColumnLayout {
        id: columns
        spacing: 2
        anchors.fill: parent
        // width: parent.width
        // height: parent.height

        // The button grid
        Grid {
          id: grid
          columns: 8
          rows: 5
          spacing: 2

          // Layout options
          Layout.fillWidth: true
          Layout.fillHeight: true

          Repeater {
            model: cellModel
            Rectangle {
              required property int index
              property int colorIndex: apc25.initialColors[index]
              property alias blink: blinkLabel.visible

              width: (grid.width - grid.spacing * (grid.columns - 1)) / grid.columns
              height: (grid.height - grid.spacing * (grid.rows - 1)) / grid.rows
              color: apc25.colors[colorIndex].color

              // Should only appear if blinking
              Text {
                  id: blinkLabel
                  text: "B"
                  horizontalAlignment: Text.AlignHCenter
                  verticalAlignment: Text.AlignVCenter
                  visible: false
              }

              TapHandler {
                onTapped: {
                  parent.colorIndex = (parent.colorIndex + 1) % apc25.colors.length
                  let entry = apc25.colors[parent.colorIndex]
                  parent.blink = entry.buttonBlink
                  parent.color = entry.color

                  let row = Math.floor(parent.index / grid.columns)
                  let col = parent.index % grid.columns
                  let cellPosition  = {col: col, row: row}


                  cellModel.setColor(parent.index, entry.buttonColor)
                }
              }
            }
          }
        }

        // The control grid
        Rectangle {
            id: bottom_panel
            color: "black"

            Layout.fillWidth: true
            Layout.preferredHeight: columns.height * 0.5
        }
    }
}
