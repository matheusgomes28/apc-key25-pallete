pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Window

Window {
    minimumWidth: 320
    minimumHeight: 480
    visible: true
    title: "APC25 Editor"

    ListModel {
        id: cellModel
        Component.onCompleted: {
            for (let i = 0; i < grid.rows * grid.columns; i++) {
                let row = Math.floor(i / grid.columns)
                let col = i % grid.columns
                append({ cellColor: Qt.hsla(i / 9, 0.7, 0.5, 1).toString(), cellPosition: {row: row, col: col} })
            }
        }
    }

    Grid {
        id: grid
        columns: 8
        rows: 5
        spacing: 2
        width: parent.width
        height: parent.height

        Repeater {
            model: grid.columns * grid.rows
            Rectangle {
                required property int index

                width: (grid.width - grid.spacing * (grid.columns - 1)) / grid.columns
                height: (grid.height - grid.spacing * (grid.rows - 1)) / grid.rows
                color: Qt.hsla(index / (grid.columns * grid.rows), 0.7, 0.5, 1)

                TapHandler {
                    onTapped: {
                        let row = Math.floor(parent.index / grid.columns)
                        let col = parent.index % grid.columns
                        let cellPosition  = {col: col, row: row}
                        cellModel.set(parent.index, { cellColor: "white", cellPosition: cellPosition })
                    }
                }
            }
        }
    }
}
