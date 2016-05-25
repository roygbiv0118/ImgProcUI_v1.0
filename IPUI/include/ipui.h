#ifndef IPUI_H
#define IPUI_H

#include <QtWidgets/QtWidgets>
#include "ui_ipui.h"
#include <memory>
using namespace std;

class DigitalRecognize;

class IPUI : public QDialog
{
	Q_OBJECT

public:
	IPUI(QWidget *parent = 0);
	virtual ~IPUI();


private:
	void showImage(const QString& strFile);
	void saveCSV(const QString& strFile, unique_ptr<float[]>& upUnitsPixels);

	void killAndDel();

private slots:
	void on_pbTest_clicked();

private:
	Ui::IPUIClass ui;
	DigitalRecognize* m_pDitRec;

	QImage* m_pImage;
	QString m_strImage;
	QString m_strCurDir;

	QPoint m_StartPos;

	qint64 m_nPID;
};

#endif // IPUI_H
