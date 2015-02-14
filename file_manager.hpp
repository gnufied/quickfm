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
    Q_PROPERTY(bool paste_mode    MEMBER paste_mode )
    
public:
    file_manager();
    ~file_manager();
    QString curDir,search_pattern,name_filter;
    QStringList selected,pendings,vlist;
    bool hidden;
    bool recurs;
    bool casesen;
    bool nofile;
    bool nofold;
    short index;
    bool paste_mode;
    
public slots:
    void search(bool);
    void search_into();
    void rename(QString);
    bool open(QString handler = "");
    void del();
    void paste();
    QString get_mimetype();
    void nev(QString,bool);
    
signals:
    void vlistch(QStringList);
    void selectedch(QStringList);
    void curDirch(QString);
    void indexch(short);
    void pendingsch(QStringList);
    
private:
    void update();
    void reset();
    QDir dir;
    QDir::Filters filters;
    QDirIterator::IteratorFlags flags;
    
    QMimeDatabase mimeDb;
    QHash<QString, QString> config;
}; 
