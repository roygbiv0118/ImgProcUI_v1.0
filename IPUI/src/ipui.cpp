#include "ipui.h"
#include <string>
#include <thread>
using namespace std;

#include "DigitalRecognize.h"

IPUI::IPUI(QWidget *parent)
	: QDialog(parent)
	, m_pImage(NULL)
	, m_nPID(0)
{
	ui.setupUi(this);

	m_strCurDir.clear();

	m_pDitRec = new DigitalRecognize();

	showImage("../Res/aa.tif");

	QString strCur = QDir::currentPath();

}

IPUI::~IPUI()
{
#if !B_SWITCH_SAVE
	if (!m_strCurDir.isEmpty())
	{
		killAndDel();
	}
#endif

	if (m_pDitRec)
	{
		delete m_pDitRec;
		m_pDitRec = NULL;
	}

	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

}

void IPUI::showImage(const QString& strFile)
{
	m_strImage = QString(strFile);

	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	m_pImage = new QImage(m_strImage);

	QImage img = m_pImage->scaled(size());

	ui.lbShow->setPixmap(QPixmap::fromImage(img));
}

void IPUI::on_pbTest_clicked()
{
	QString strFile = QFileDialog::getOpenFileName(this, "Open", "../Res/", "Images(*.tif *.jpg *.bmp *.png)");
	if (strFile.isEmpty()) return;

	showImage(strFile);

	QString strFileName;
	int nBeg = strFile.lastIndexOf('/');
	if (-1 != nBeg)
	{
		strFileName = strFile.mid(nBeg + 1);
	}

	QString strSave = QString("../Res/Save_%1_%2_%3/").arg(QDate::currentDate().toString("yyyyMMdd")).arg(QTime::currentTime().toString("hhmmss")).arg(strFileName);

	bool bRes = QDir::current().mkdir(strSave);

	unique_ptr<float[]> upUnitsPixels(new float[CHIP_ROWS*CHIP_COLS*UNIT_ROWS*UNIT_COLS]{0});

	m_pDitRec->ProcessingImage(m_strImage.toStdString(), strSave.toStdString(), upUnitsPixels);

	QString strFileCSV = strSave + "save.csv";

#if B_SWITCH_SAVE
	saveCSV(strFileCSV, upUnitsPixels);

#else	
	if (!m_strCurDir.isEmpty())
	{
		killAndDel();
	}
	saveCSV(strFileCSV, upUnitsPixels);

	QProcess* pProcess = new QProcess(this);
	QStringList args;
	args << strFileCSV;
	//pProcess->start("C:/Program Files (x86)/Microsoft Office 2007/Office12/EXCEL.EXE", args);
	pProcess->start("D:\\Program Files\\Microsoft Office\\Office15\\excel.exe", args);
	m_nPID = pProcess->processId();

	m_strCurDir = strSave;
#endif

}

void IPUI::saveCSV(const QString& strFile, unique_ptr<float[]>& upUnitsPixels)
{
	QFile file(strFile);
	bool bRes = file.open(QIODevice::Truncate | QIODevice::WriteOnly);
	QTextStream stream(&file);

	char chCols = 'A';
	for (int chip_row = 0; chip_row < CHIP_ROWS; chip_row++)
	{
		for (int chip_col = 0; chip_col < CHIP_COLS; chip_col++)
		{

			stream << char(chCols + chip_col) << (chip_row + 1) << endl;
			for (int unit_row = 0; unit_row < UNIT_ROWS; unit_row++)
			{
				stream << ' ' << ',';
				for (int unit_col = 0; unit_col < UNIT_COLS; unit_col++)
				{
					stream << qRound(upUnitsPixels[(chip_row*CHIP_COLS + chip_col)*UNIT_ROWS*UNIT_COLS + (unit_row*UNIT_COLS + unit_col)]) << ',';
				}
				stream << endl;
			}
			stream << endl;

		}
	}
	file.close();

}

void IPUI::killAndDel()
{
	QString strCmd;
	int nRes = 0;
	strCmd = QString("@taskkill  /f /t /pid %1 1>nul 2>nul").arg(m_nPID);
	nRes = system(strCmd.toLocal8Bit());

	strCmd = QString("@rd /s/q ") + m_strCurDir.replace('/',"\\")+" 1>nul 2>nul";
	nRes = system(strCmd.toLocal8Bit());

}