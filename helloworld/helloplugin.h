#ifndef HELLOPLUGIN_H
#define HELLOPLUGIN_H

#include <libkipi/plugin.h>
#include <libkipi/interface.h>

class HelloPlugin :public KIPI::Plugin {
    Q_OBJECT

public:
    HelloPlugin( QObject* parent, const char* name = 0, const QStringList& = QStringList());
    virtual KIPI::Category category() const;
    QString id() const { return QString::fromLatin1("hello"); }
protected slots:
    void go();

private:
    KIPI::Interface* m_interface;
};


#endif /* HELLOPLUGIN_H */

