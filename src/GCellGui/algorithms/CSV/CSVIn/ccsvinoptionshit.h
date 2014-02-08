#ifndef CCSVINOPTIONSHIT_H
#define CCSVINOPTIONSHIT_H

#include <QWidget>

class QListWidget;

class CCSVIn;

class CFileNameEdit : public QWidget
{
	Q_OBJECT
private:
	QString m_fileName;
public:
	explicit CFileNameEdit(const QString &fileName, QWidget *parent = 0);

	const QString& fileName(void) const{return m_fileName;}
	void setFileName(const QString &fileName);
private slots:
	void selectFile(void);
signals:
	void fileNameChanged(QString fileName);
};

class CCSVInOptionShit : public QWidget
{
	Q_OBJECT
private:
    CCSVIn *m_csvIn;
    QString m_fileName;
	CFileNameEdit *m_fileNameEdit;
    QListWidget *m_dataLinesSelector;
public:
    explicit CCSVInOptionShit(CCSVIn *csvIn, QWidget *parent = 0);

    const QString& fileName(void) const{return m_fileName;}
    QMap<int, QString> dataIndexes(void);
private slots:
    void onFileNameChanged(const QString &fileName);
    void update(void);
};

#endif // CCSVINOPTIONSHIT_H