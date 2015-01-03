import QtQuick 2.4
import QtQuick.Controls 1.2

ScrollView 
{
    MouseArea 
    {
        anchors.fill:parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton       
        onClicked:
        {
            var obj = flow.childAt(mouseX, mouseY)
            if(obj)
            {
                if(fm.selected.indexOf(fm.vlist[obj.index]) ==-1)
                {
                    fm.selected.push(fm.vlist[obj.index])
                }
                                
                else
                {
                    fm.selected.splice(fm.selected.indexOf(fm.vlist[obj.index]),1)
                }
            }
                    
            else
            {
                fm.selected = []   
            }
                        
        }
                
        onDoubleClicked: 
        {
            var obj = flow.childAt(mouseX, mouseY)
            if(obj)
            {
                if ( fm.vlist[obj.index+2]=="true" ) 
                {
                    fm.curDir = fm.vlist[obj.index]
                    fm.search(false)
                } 
                                
                else if(fm.open_file(fm.vlist[obj.index]))
                {
   loader.setSource("dialog.qml",{"dialog":0,"txt":"Handler haven't set.Set a handler via 'Open With' action in right click menu."})
                }
            }
        }
                
        onPressed:
        {
            var obj = flow.childAt(mouseX, mouseY)
            if(obj && mouse.button == Qt.RightButton)
            {
                right_click.file = fm.vlist[obj.index]
                right_click.isDir = fm.vlist[obj.index + 2] == "true"
                right_click.onItem = true
                right_click.isSelected = false
                fm.selected = []
                right_click.popup()
            }
                    
            else if(mouse.button == Qt.RightButton)
            {
                right_click.onItem = false
                right_click.isSelected = fm.selected.length != 0
                right_click.popup()
            }
        }
    }
                    
    Flow 
    {
        id:flow
        x:10
        width:window.width - 10
        spacing:25
    
        Repeater 
        {
            model:fm.vlist.length/3
        
            Image 
            {
                property int index : 3 * Positioner.index
                source:["folder.svgz","text.svgz"][Number(fm.vlist[index+2] == "false")]
                fillMode:Image.Pad
                height:80
                width:75
                verticalAlignment:Image.AlignTop
                    
                Text 
                {
                    text:String(fm.vlist[parent.index + 1])
                    width:parent.width + 5
                    maximumLineCount:1
                    elide:Text.ElideRight
                    //height:50
                    color:fm.selected.indexOf(fm.vlist[parent.index]) !=-1 ? "red" : "black"
                    anchors.bottom:parent.bottom
                    anchors.horizontalCenter:parent.horizontalCenter
                    horizontalAlignment:Text.AlignHCenter
                }
            }
        }
    }
}  
