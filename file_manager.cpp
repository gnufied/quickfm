#include "file_manager.hpp"

#include <QFile>
#include <QProcess>
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

void file_manager::search_into()
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

QString file_manager::get_mimetype()
{
    return mimeDb.mimeTypeForFile(vlist[index]).name();
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
    QStringList args;

    for(short zz=0; zz < pendings.size(); zz++)
    {
        if(paste_mode)
        {
            
        }
            
        else
        {
            
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

void file_manager::nev(QString name,bool mode)
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
        config[mime_t.name()] = handler;

        QFile config_file(QString(getenv("HOME")).append("/.config/efemrc"));

        QString str = "%1=%2;\n";

        if(config_file.open(QIODevice::ReadWrite))
        {
            config_file.readAll();
            config_file.write(str.arg(mime_t.name(),handler).toLatin1().constData());
            config_file.close();
        }
    }
    
    if(mime_t.inherits("text/plain") and config.contains("text/plain") and !config.contains(mime_t.name()))
    {
        QString prog = config["text/plain"];
        prog.append(' ');
        prog.append(vlist[index]);
        QProcess::startDetached(prog);
        index = -1;
        emit indexch(-1);
    }
    
    else if(config.contains(mime_t.name()))
    {
		QString prog = config[mime_t.name()];
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
    :curDir(getenv("HOME"))
{  
    QFile config_file(QString(getenv("HOME")).append("/.config/efemrc"));
    index = -1;
    
    reset();

    if(!config_file.exists())
    {
        if(config_file.open(QIODevice::WriteOnly))
        {
            config_file.write("quickfm_version=0.2;\n");
            config_file.close();
        }
        
        else
        {
            qDebug()<<"ERROR:Config file couldn't created: "<<QString(getenv("HOME")).append("/.config/efemrc");
        }
    }
    
    config_file.open(QFile::ReadOnly);
    
    while (!config_file.atEnd())
    {
        QStringList line = QString(config_file.readLine()).split(";", QString::SkipEmptyParts)[0].split("=", QString::SkipEmptyParts);
            
        if ( line.size() == 2 ) config[line[0]] = line[1];
    }
    config_file.close();
    
    QDirIterator it(curDir, QStringList("*"), filters, flags);
    
    while (it.hasNext()) 
    {
        vlist << it.next();
        vlist << it.fileName();
        vlist << QVariant(it.fileInfo().isDir()).toString();
    }
}

