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
        
        MenuItem {
            text: fm.index + 1 ? fm.vlist[fm.index + 1] : fm.curDir
            enabled:false
            onTriggered: {}
        }
        
        MenuSeparator { }
        
        MenuItem {
            text: "Create New Text File"
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"dialog":0,"mode":false})
            }
        }
        
        MenuItem {
            text: "Create New Folder"
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"dialog":0,"mode":true})
            }
        }
            
        MenuItem {
            text: "Rename"
            enabled:fm.index + 1
            onTriggered:
            {
                loader.setSource("dialog.qml",{"dialog":2})
            }
        }
        
        MenuItem {
            text: "Open With"
            enabled:fm.index + 1 && fm.vlist[fm.index + 2] 
            onTriggered: 
            {
                loader.setSource("dialog.qml",{"dialog":1,"txt":"Enter a command to assign this mime or left empty to cancel"})
            }
        }
        
        MenuItem {
            text: "Delete"
            enabled:fm.index + 1 || fm.selected.length
            onTriggered: {
                fm.selected.length ? fm.pendings = fm.selected : fm.pendings = [fm.vlist[fm.index]]
                fm.index = -1
                fm.del()
                fm.pendings = []
            }
        }
        
        MenuItem {
            text: "Paste"
            enabled:fm.pendings.length
            onTriggered: {
                fm.paste()
                fm.pendings = []
            }
        }
        
        MenuItem {
            text: "Copy"
            enabled:fm.index + 1 || fm.selected.length
            onTriggered: 
            {
                fm.paste_mode = false;
                
                if(fm.selected.length)
                {
                    fm.pendings = fm.selected
                }
                
                else 
                {
                    fm.pendings = [fm.vlist[fm.index]]
                    fm.index =-1;
                }
            }
        }
        
        MenuItem {
            text: "Cut"
            enabled:fm.index + 1 || fm.selected.length
            onTriggered: 
            {
                fm.paste_mode = true;

                if(fm.selected.length)
                {
                    fm.pendings = fm.selected
                }
                
                else 
                {
                    fm.pendings = [fm.vlist[fm.index]]
                    fm.index =-1;
                }
            }
        }
    }
    
    Loader {id:loader; source:"body.qml"; anchors.fill:parent}
    
    Component.onCompleted:{
        visible=true
    }
}
