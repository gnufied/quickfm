import QtQuick 2.4
import QtQuick.Controls 1.3

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
'Named' Field:Type here space seperated arbitrary number of search patterns (eg '*.cpp *.h') for file or folder name.\n
If left empty default is '*'.Correspond to 'nameFilters' above.\n
'Look in' Field:Type here for absolute path which that you seach into.Default is current dir.Correspond to 'path' above.\n
'Contain' Field:Type here for pattern which search into text files.(files for inherits 'text/plain' mimetype)\n
Utf8 patterns supported.Implemented via QByteArray::contains(QByteArray& pattern)\n\n
\tAbout right click menu actions\n
'Open With' action:Specify the command which assigned to this file's mimetype for opening files has such mimetype.Arguments for commands supported.\n\n
\tAbout config file\n
quichfm keeps settings in ~/.config/quickfm  file.Manually editing to this file may cause undesired behaviour.\n\n
\tFurther infos and examples\n
Say an user right clicked a file named foo.tar.gz then triggered 'Open With' action. User can be specify a command like 'tar -zxf'\n
Thus 'tar -zxf' command assigned to 'application/x-compressed-tar' mimetype, this setting writed to config file and remembered later.\n
So later user double clicked a .tar.gz file 'tar -zxf' command executed automatically."
}
