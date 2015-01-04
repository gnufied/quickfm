import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1

ApplicationWindow 
{
    id: window
    width:800
    height:600
    minimumWidth:300
    minimumHeight:300
    
    toolBar:ToolBar
    {
        width:parent.width
        height:33
        RowLayout {
            anchors.fill: parent
            
            ToolButton {
                iconSource: "exit.svgz"
                onClicked:Qt.quit()
            }
            
            ToolButton {
                iconSource: "edit-find.svgz"
                onClicked:
                {
                    String(loader.source).indexOf("body") + 1 ? loader.setSource("search.qml") : loader.setSource("body.qml")
                }
            }
            
            ToolButton {
                iconSource: "edit-clear.svgz"  //reset filters
                onClicked:
                {
                    if (String(loader.source).indexOf("body") + 1)
                    {
                        fm.search(false)
                    }
                }
            }
            
            ToolButton {
                iconSource: "go-up.svgz"
                onClicked:fm.search(true)
            }

			ToolButton {
                iconSource: "help-about.svgz"
                onClicked:
				{
					String(loader.source).indexOf("help") + 1 ? loader.setSource("body.qml") : loader.setSource("help.qml")
				}
            }
            
            Item { Layout.fillWidth: true }
        }
    }
    
    Menu
    {
        id:right_click
        property string file:""
        property bool onItem:false
        property bool isSelected:false
        property bool isDir:false
        
        MenuItem {
            text: "Create New Text File"
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"mode":false,"dialog":1,"txt":"Please enter a name for file."})
            }
        }
        
        MenuItem {
            text: "Create New Folder"
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"mode":true,"dialog":1,"txt":"Please enter a name for folder."})
            }
        }
            
        MenuItem {
            text: "Rename"
            enabled:right_click.onItem
            onTriggered:
            {
                loader.setSource("dialog.qml",{"dialog":3,"txt":right_click.file})
            }
        }
        
        MenuItem {
            text: "Open With"
            enabled:!right_click.isDir && right_click.onItem
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"dialog":2,"txt":right_click.file})
            }
        }
        
        MenuItem {
            text: "Delete"
            enabled:right_click.onItem || right_click.isSelected
            onTriggered: {
                right_click.onItem ? fm.perform_delete(right_click.file) : fm.perform_delete("")
            }
        }
        
        MenuItem {
            text: "Paste"
            enabled:fm.pendings.length > 0
            onTriggered: {
                fm.perform_move()
            }
        }
        
        MenuItem {
            text: "Copy"
            enabled:right_click.onItem || right_click.isSelected
            onTriggered: {
                if(right_click.onItem)
                {
                    fm.pendings = [right_click.file]
                }
                else
                {
                    fm.pendings = fm.selected
                }
            }
        }
        
        MenuItem {
            text: "Cut"
            enabled:right_click.onItem || right_click.isSelected
            onTriggered: {
                if(right_click.onItem)
                {
                    fm.pendings = ["",right_click.file]
                }
                else
                {
                    fm.pendings = fm.selected
                    fm.pendings.unshift("")
                }
            }
        }
    }
    
    Loader {id:loader; source:"body.qml"; anchors.fill:parent}
    
    Component.onCompleted:{
        visible=true
    }
}
