import QtQuick 2.4
import QtQuick.Controls 1.3

Column 
{
    x:20
    y:20
    width:window.width
    spacing:20
      
    Grid 
    {
        columnSpacing:22
        rowSpacing:20
        columns:2
    
        Label {font.pixelSize:18; text:"Named:" }
      
        TextField {width:560; id:nameFilters}
      
        Label {font.pixelSize:18; text:"Look in:" }
      
        TextField {width:560; id:directory}
      
        Label {font.pixelSize:18; text:"Contain:" }
      
        TextField {width:560; id:pattern}
    }
      
    Row {
        spacing:20
    
        CheckBox{
            id:recursive
            text:"Recursive"
            onClicked:fm.recurs=checked
        }
    
        CheckBox{
            id:hidden
            text:"Show hidden"
            onClicked:fm.hidden=checked
        }
    
        CheckBox{
            id:case_sensitive
            text:"Case sensiteve"
            onClicked:fm.casesen=checked
        }
    
        CheckBox{
            id:only_folders
            text:"Only Folders"
            onClicked:fm.nofile=checked
        }
    
        CheckBox{
            id:only_files
            text:"Only Files"
            onClicked:fm.nofold=checked
        }
                
        Button{
            text:"Search"
            onClicked:
            {
                directory.text.length ? fm.curDir = directory.text : false
                fm.name_filter = nameFilters.text
                
                if(pattern.text)
                { 
                    fm.nofold = true
                    fm.search_pattern = pattern.text
                    fm.find()
                }
            
                else {fm.search(false)}
                loader.source = "body.qml"
            }
        }
    }
} 
