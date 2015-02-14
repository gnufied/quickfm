import QtQuick 2.4
import QtQuick.Controls 1.3

Column
{
    property bool mode:false
    property string txt:"Please enter a name"
    property int dialog:0
    
    Label{
        id:lbl
        text:parent.txt
    }
    
    TextField
    {
        id:fld
		width:loader.width
		height:30
        text: ""
        onAccepted:
        {
            if(dialog==0)
            {
                fm.nev(text,mode)
            }
            
            else if(dialog==1 && fld.length)
            {
                fm.open(text)
            }
            
            else if(dialog==2) fm.rename(text);
 
            loader.setSource("body.qml")
        }
    }
    spacing:20
} 
