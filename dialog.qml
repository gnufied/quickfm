import QtQuick 2.4
import QtQuick.Controls 1.3

Column
{
    property bool mode:false
    property string txt:""
    property int dialog:0
    
    function funn(dial)
    {
        var rett = ""
        switch (dial)
        {
        case 0:
            rett = txt
            break
                
        case 1:
            rett = txt
            break
                
        case 2:
            rett = String("Enter full path for program.Arguments are supported.(eg /usr/bin/foo --bar) Mimetype: ").concat(fm.get_mimetype(txt))
            break
                
        case 3:
            rett = "Please enter new name"
            break
        }
                
        return rett        
    }
    
    Label{
        id:lbl
        text:funn(dialog)
    }
    
    Button{
        id:bttn
        text:"Ok"
        visible:parent.dialog == 0
        onClicked:
        {
            loader.setSource("body.qml")
        }
    }
    
    TextField
    {
        id:fld
		width:loader.width
		height:30
        text: ""
        visible:parent.dialog > 0
        onAccepted:
        {
            if(dialog==1)
            {
                fm.make_new(text,mode)
            }
            
            else if(dialog==2)
            {
                fm.open_file(txt,text)
            }
            
            else if(dialog==3) fm.perform_rename(txt,text);
 
            loader.setSource("body.qml")
        }
    }
    spacing:20
} 
