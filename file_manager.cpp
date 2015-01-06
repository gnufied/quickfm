#include "file_manager.hpp"

#include <QFile>
#include <QDebug>

#include <unistd.h>

void file_manager::search(bool go_up)
{
    if(go_up)
    {
        directory.setPath(curDir);
        directory.cdUp();
        curDir = directory.absolutePath();
    }
    
    update();
    
    vlist.clear();

    QStringList filterr = name_filter.split(' ',QString::SkipEmptyParts);

    QDirIterator it(curDir,filterr,dir_filter,iter_flags);
        
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

    QDirIterator it(curDir,filterr,dir_filter,iter_flags);
        
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

QString file_manager::get_mimetype(QString path)
{
    return mimeDb.mimeTypeForFile(path).name();
}

void file_manager::perform_delete(QString path)
{
    if(!path.isEmpty())
    {
        system(QString("rm -rf ").append(path).toLatin1().constData());
        search(false);
        return;
    }
    
    for(short lenn=0;lenn<selected.size();lenn++)
    {   
        system(QString("rm -rf ").append(selected[lenn]).toLatin1().constData());
    }
    search(false);
}

void file_manager::perform_move()
{
    QDir::setCurrent(curDir);

    if(pendings[0].isEmpty())
    {
        for(short zz=1; zz < pendings.size(); zz++)
        {
            system(QString("mv -f ").append(pendings[zz]).append(" .").toLatin1().constData());
        }
    }
    else
    {
        for(short zz=0; zz < pendings.size(); zz++)
        {
            system(QString("cp -pPR ").append(pendings[zz]).append(" .").toLatin1().constData());
        }
    }
    pendings.clear();
    emit pendingsch(pendings);
    search(false);
}

void file_manager::perform_rename(QString path,QString new_name)
{
    QFileInfo finfo(path);
    QDir::setCurrent(curDir);
    QString sttr = QString("rename ").append(finfo.fileName()).append(' ').append(new_name).append(' ').append(finfo.fileName());
    
    system(sttr.toLatin1().constData());
    search(false);
}

void file_manager::make_new(QString name,bool mode)
{
    QString str;
    str.append(curDir);
    str.append('/');
    str.append(name);
    
    if(mode)
    {
        QDir dir(curDir);
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

bool file_manager::open_file(QString path,QString handler)
{
    QMimeType mime_t = mimeDb.mimeTypeForFile(path);
    bool ret = false;
    
    if(mime_t.inherits("inode/directory")) return false;
    
    if(!handler.isEmpty())
    {
        configurations[mime_t.name()] = handler;

		QFile config_file(QString(getenv("HOME")).append("/.config/efemrc"));

		QString str = "%1=%2;\n";

        if(config_file.open(QIODevice::ReadWrite))
        {
			config_file.readAll();
            config_file.write(str.arg(mime_t.name(),handler).toLatin1().constData());
            config_file.close();
        }
	}
    
    if(mime_t.inherits("text/plain") and configurations.contains("text/plain") and !configurations.contains(mime_t.name()))
    {
		int pid = fork();
		QStringList prog = configurations["text/plain"].split(' ', QString::SkipEmptyParts);
		char* my_argv[prog.size() + 2];

		switch (pid) 
		{
		case 0:
			for(char loop=0; loop < prog.size(); loop++)
			{
				my_argv[loop] = prog[loop].toLatin1().data();
			}

			my_argv[prog.size()] = (char*)path.toLatin1().constData();
			my_argv[prog.size() + 1] = (char*)0;
			execv(prog[0].toLatin1().constData(), my_argv);
			perror("execv");
			break;
		
		case -1:
			perror("fork");
			break;
		default:
			break;	
		}	
    }
    
    else if(configurations.contains(mime_t.name()))
    {
		int pid = fork();
		QStringList prog = configurations[mime_t.name()].split(' ', QString::SkipEmptyParts);
		char* my_argv[prog.size() + 2];

		switch (pid) 
		{
		case 0:
			for(char loop=0; loop < prog.size(); loop++)
			{
				my_argv[loop] = prog[loop].toLatin1().data();
			}

			my_argv[prog.size()] = (char*)path.toLatin1().constData();
			my_argv[prog.size() + 1] = (char*)0;
			execv(prog[0].toLatin1().constData(), my_argv);
			perror("execv");
			break;
		
		case -1:
			perror("fork");
			break;
		default:
			break;	
		}	
    }
    
    else { ret = true;}
    
    return ret;
}

void file_manager::update()
{  
    if(name_filter.isEmpty()) name_filter = "*";
 
    recurs ? iter_flags = iter_flags | QDirIterator::Subdirectories : iter_flags;
    nofile ? dir_filter = QDir::NoDotAndDotDot | QDir::Dirs : dir_filter;
    nofold ? dir_filter = QDir::NoDotAndDotDot | QDir::Files : dir_filter;
    hidden ? dir_filter = dir_filter | QDir::Hidden : dir_filter;
    casesen ? dir_filter = dir_filter | QDir::CaseSensitive : dir_filter;
}

void file_manager::reset()
{
    name_filter    = QString("*");
    
    search_pattern = "";
    
    dir_filter = QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs;
    
    iter_flags = QDirIterator::NoIteratorFlags;
    
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
    
    reset();

    if(!config_file.exists())
    {
        if(config_file.open(QIODevice::WriteOnly))
        {
            config_file.write("quickfm_version=0.1;\n");
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
            
        if ( line.size() == 2 ) configurations[line[0]] = line[1];
    }
    config_file.close();
    
    QDirIterator it(curDir,QStringList("*"),dir_filter,iter_flags);
    
    while (it.hasNext()) 
    {
        vlist << it.next();
        vlist << it.fileName();
        vlist << QVariant(it.fileInfo().isDir()).toString();
    }
}

