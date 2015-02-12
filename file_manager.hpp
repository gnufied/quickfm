#include <QMimeDatabase>
#include <QDirIterator>

class file_manager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString     curDir MEMBER curDir NOTIFY curDirch)
    Q_PROPERTY(short  index MEMBER index NOTIFY indexch)
    Q_PROPERTY(QStringList vlist  MEMBER vlist   NOTIFY vlistch)
    Q_PROPERTY(QStringList selected  MEMBER selected NOTIFY selectedch)
    Q_PROPERTY(QStringList pendings  MEMBER pendings NOTIFY pendingsch)
    Q_PROPERTY(QString name_filter  MEMBER name_filter)
    Q_PROPERTY(QString search_pattern  MEMBER search_pattern)
    Q_PROPERTY(bool hidden        MEMBER hidden )
    Q_PROPERTY(bool recurs        MEMBER recurs )
    Q_PROPERTY(bool casesen       MEMBER casesen)
    Q_PROPERTY(bool nofile        MEMBER nofile )
    Q_PROPERTY(bool nofold        MEMBER nofold )
    
public:
    file_manager();
    ~file_manager();
    QString curDir,search_pattern,name_filter;
    QStringList vlist,selected,pendings;
    bool hidden;
    bool recurs;
    bool casesen;
    bool nofile;
    bool nofold;
    short index;
    
public slots:
    void search(bool);
    void search_into();
    void perform_rename(QString);
    bool open_file(QString handler = "");
    void perform_delete();
    void perform_move();
    QString get_mimetype();
    void make_new(QString,bool);
    
signals:
    void vlistch(QStringList);
    void pendingsch(QStringList);
    void selectedch(QStringList);  
    void indexch(short);
    void curDirch(QString);
    
private:
    void update();
    void reset();
    QDir directory;
    QDir::Filters dir_filter;
    QDirIterator::IteratorFlags iter_flags;
    
    QMimeDatabase mimeDb;
    QHash<QString, QString> configurations;
}; 
