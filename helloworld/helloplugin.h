#ifndef HELLOPLUGIN_H
#define HELLOPLUGIN_H

#include <libkipi/plugin.h>
#include <libkipi/interface.h>

class HelloPlugin :public KIPI::Plugin {
    Q_OBJECT

public:
    HelloPlugin( QObject* parent, const char* name = 0, const QStringList& = QStringList());
    virtual KIPI::Category category() const;

protected slots:
    void go();

private:
    KIPI::Interface* m_interface;
};


#endif /* HELLOPLUGIN_H */

