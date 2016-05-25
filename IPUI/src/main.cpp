#include "ipui.h"
#include <QtWidgets/QApplication>
#include <QResource>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QDir::setCurrent(a.applicationDirPath());
	IPUI w;
	w.show();
	return a.exec();
}
