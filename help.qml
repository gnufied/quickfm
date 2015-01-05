import QtQuick 2.4
import QtQuick.Controls 1.2

TextArea
{
	readOnly:true
	text: "\n
\t\t\tWelcome Quickfm Help\n\n
\tToolbar icons from left to right\n
red rectangle   :quit this app\n
magnifying glass:open/close search tool\n
left arrow      :reset search filters\n
up arrow        :go to upper directory\n
blue cicle      :show/hide this help\n\n
Searching in directory content implemented via:\n
QDirIterator(const QString & path, const QStringList & nameFilters, QDir::Filters filters = QDir::NoFilter, IteratorFlags flags = NoIteratorFlags)\n
\tAbout searh tool from top to bottom\n
'Named' Field:Type here a search pattern for file or folder name.if left empty default is '*'.Correspond to 'nameFilters' above.\n
'Look in' Field:Type here for path which that you seach into.Correspond to 'path' above.\n
'Contain' Field:Type here for pattern which search into text files(files for inherits 'text/plain' mimetype)<WIP>\n\n
\tAbout right click menu actions\n
'Open With' action:Specify the command which assigned to this file's mimetype for opening files has such mimetype.Arguments for commands supported.\n\n
\tAbout config file\n
quichfm keeps settings (eg file handlers) ~/.config/quickfm  file.Manually edit to this file may cause undesired/harmfull behaviour.\n\n
\tFurther infos and examples\n
eg. Say right click a foo.tar.gz then trigger 'Open With' action. User can be specify a command like 'tar -zxf'  
"
}
