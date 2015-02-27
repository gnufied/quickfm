#include "fm.hpp"

#include <QFile>
#include <QProcess>
#include <QJsonDocument>
#include <QDebug>

void file_manager::search(bool go_up)
{
    if(go_up)
    {
        dir.setPath(curDir);
        dir.cdUp();
        curDir = dir.absolutePath();
        emit curDirch(curDir);
    }
    
    update();
    
    vlist.clear();

    QStringList filterr = name_filter.split(' ',QString::SkipEmptyParts);

    QDirIterator it(curDir,filterr,filters,flags);
        
        while (it.hasNext()) 
        {
            vlist << it.next();
            vlist << it.fileName();
            vlist << QVariant(it.fileInfo().isDir()).toString();
        }
    
    emit vlistch(vlist);
    reset();
}

void file_manager::find()
{
    update();
    
    vlist.clear();

    QStringList filterr = name_filter.split(' ',QString::SkipEmptyParts);

    QDirIterator it(curDir, filterr, filters, flags);
        
    while (it.hasNext()) 
    {
        it.next();
        it.fileInfo();

	    if(it.fileInfo().isReadable() and mimeDb.mimeTypeForFile(it.fileInfo()).inherits("text/plain"))
	    {
            QFile filee(it.filePath());
            filee.open(QFile::ReadOnly);

            if(filee.readAll().contains(search_pattern.toUtf8()))
            {
                vlist << it.filePath();
                vlist << it.fileName();
                vlist << "false";   
            }

            filee.close();
        }
    }
    
    emit vlistch(vlist);
    reset();
}

void file_manager::del()
{
    for(short lenn=0;lenn<pendings.size();lenn++)
    { 
        QFileInfo info(pendings[lenn]);
        
        if(info.isDir())
        {
            dir.setPath(info.filePath());
            dir.removeRecursively();
        }
            
        else 
        {
            QDir::setCurrent(info.path());
            QFile::remove(info.fileName());
        }
    }
    search(false);
}

void file_manager::paste()
{
    QDir::setCurrent(curDir);
    
    for(short zz=0; zz < pendings.size(); zz++)
    {
        QString prog = "cp -a ";
        prog.append(pendings[zz]);
        prog.append(" .");
        QProcess::startDetached(prog);
        
        if (paste_mode)
        {
            prog = "rm -r ";
            prog.append(pendings[zz]);
            QProcess::startDetached(prog);
        }
    }

    search(false);
}

void file_manager::rename(QString new_name)
{
    QFileInfo info(vlist[index]);
        
    dir.setPath(info.path());
    
    if(dir.rename(info.fileName(),new_name)) search(false);
}

void file_manager::create(QString name,bool mode)
{
    QString str;
    str.append(curDir);
    str.append('/');
    str.append(name);
    
    if(mode)
    {
        dir.setPath(curDir);
        if(dir.mkdir(name))
        {
            vlist<< str;
            vlist<< name;
            vlist<< QVariant(mode).toString();
        }
    }
    
    else
    {
        QFile fll(str);
        if(fll.open(QIODevice::WriteOnly))
        {
            vlist<< str;
            vlist<< name;
            vlist<< QVariant(mode).toString();
            fll.close();
        }
    } 
    emit vlistch(vlist);
}

bool file_manager::open(QString handler)
{
    QMimeType mime_t = mimeDb.mimeTypeForFile(vlist[index]);
    bool ret = false;
    
    if(!handler.isEmpty())
    {
        cfg.insert(mime_t.name(),QJsonValue(handler));

        QFile config_file(QDir::homePath().append("/.config/quickfm"));

        if(config_file.open(QIODevice::WriteOnly))
        {
            QJsonDocument doc(cfg);
            config_file.write(doc.toJson());
            config_file.close();
        }
    }
    
    if(mime_t.inherits("text/plain") and cfg.contains("text/plain") and !cfg.contains(mime_t.name()))
    {
        QString prog = cfg["text/plain"].toString();
        prog.append(' ');
        prog.append(vlist[index]);
        QProcess::startDetached(prog);
        index = -1;
        emit indexch(-1);
    }
    
    else if(cfg.contains(mime_t.name()))
    {
		QString prog = cfg[mime_t.name()].toString();
        prog.append(' ');
        prog.append(vlist[index]);
        QProcess::startDetached(prog);
        index = -1;
        emit indexch(-1);
    }
    
    else { ret = true;}
    
    return ret;
}

void file_manager::update()
{  
    if(name_filter.isEmpty()) name_filter = "*";
 
    recurs ? flags = flags | QDirIterator::Subdirectories : flags;
    nofile ? filters = QDir::NoDotAndDotDot | QDir::Dirs : filters;
    nofold ? filters = QDir::NoDotAndDotDot | QDir::Files : filters;
    hidden ? filters = filters | QDir::Hidden : filters;
    casesen ? filters = filters | QDir::CaseSensitive : filters;
}

void file_manager::reset()
{
    name_filter    = QString("*");
    
    search_pattern = "";
    
    filters = QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs;
    
    flags = QDirIterator::NoIteratorFlags;
    
    hidden =false;
    recurs =false;
    casesen=false;
    nofold = false;
    nofile = false;
    selected.clear();
    emit selectedch(selected);
}

file_manager::~file_manager()
{}

file_manager::file_manager()
{  
    curDir = QDir::homePath();
    QFile config_file(QDir::homePath().append("/.config/quickfm"));
    index = -1;
    
    reset();
    
    if(config_file.exists())
    {
        config_file.open(QFile::ReadOnly);
        QJsonDocument doc = QJsonDocument::fromJson(config_file.readAll());
        cfg = doc.object();
        config_file.close();
    }
    
    QDirIterator it(curDir, QStringList("*"), filters, flags);
    
    while (it.hasNext()) 
    {
        vlist << it.next();
        vlist << it.fileName();
        vlist << QVariant(it.fileInfo().isDir()).toString();
    }
}

