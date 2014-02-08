#include "ccsvinoptionshit.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QScrollArea>
#include <QFile>
#include <QListWidget>

#include "ccsvin.h"

/*!
 * \class CFileNameEdit
 */
CFileNameEdit::CFileNameEdit(const QString &fileName, QWidget *parent) : QWidget(parent)
{
	setObjectName(QStringLiteral("CFileNameEdit"));

	QHBoxLayout *layout = new QHBoxLayout();
	layout->setSpacing(0);
	layout->setMargin(0);
	layout->setContentsMargins(0, 0, 0, 0);
	setLayout(layout);

	QLineEdit *leFileName = new QLineEdit();
	connect(this, SIGNAL(fileNameChanged(QString)), leFileName, SLOT(setText(QString)));
	layout->addWidget(leFileName, 1);

	QPushButton *pbSelectFile = new QPushButton("...");
	pbSelectFile->setMaximumWidth(20);
	connect(pbSelectFile, SIGNAL(clicked()), this, SLOT(selectFile()));
	layout->addWidget(pbSelectFile, 0);

	setFileName(fileName);
}

void CFileNameEdit::setFileName(const QString &fileName)
{
	if(m_fileName == fileName) return;
	m_fileName = fileName;
	emit fileNameChanged(m_fileName);
}

void CFileNameEdit::selectFile(void)
{
	QString opfileName = QFileDialog::getOpenFileName(this, "Select CSV File", fileName());
	if(!opfileName.isEmpty())
	{
		setFileName(opfileName);
	}
}

/*!
 * \class CCSVInOptionShit
 */
CCSVInOptionShit::CCSVInOptionShit(CCSVIn *csvIn, QWidget *parent) : QWidget(parent)
{
	setObjectName(QStringLiteral("CCSVInOptionShit"));

    m_csvIn = 0;
	m_fileNameEdit = 0;
    m_dataLinesSelector = 0;

    m_csvIn = csvIn;
    if(m_csvIn) m_fileName = m_csvIn->fileName();

	QVBoxLayout *vbl = new QVBoxLayout();
	vbl->setSpacing(0);
	vbl->setContentsMargins(0, 0, 0, 0);
	vbl->setMargin(0);
	setLayout(vbl);

    m_fileNameEdit = new CFileNameEdit(m_fileName);
	m_fileNameEdit->setObjectName(QStringLiteral("fileNameEdit"));
    connect(m_fileNameEdit, SIGNAL(fileNameChanged(QString)), this, SLOT(onFileNameChanged(QString)));
	vbl->addWidget(m_fileNameEdit, 0);

    m_dataLinesSelector = new QListWidget();
    vbl->addWidget(m_dataLinesSelector, 0);

    update();
}

QMap<int, QString> CCSVInOptionShit::dataIndexes(void)
{
    QMap<int, QString> resultIndexes;
    if(!m_dataLinesSelector) return resultIndexes;

    for(int ci = 0; ci < m_dataLinesSelector->count(); ++ci)
    {
        if(m_dataLinesSelector->item(ci)->checkState() == Qt::Checked) resultIndexes[ci] = m_dataLinesSelector->item(ci)->text();
    }

    return resultIndexes;
}

void CCSVInOptionShit::onFileNameChanged(const QString &fileName)
{
    if(m_fileName == fileName) return;
    m_fileName = fileName;
    update();
}

void CCSVInOptionShit::update(void)
{
    QFile csvFile(m_fileName);
    if(!csvFile.exists()) return;

    csvFile.open(QIODevice::ReadOnly);
    QStringList header;
    int maxColumn = 0;
    QChar separator = QChar(';');
    QString l = csvFile.readLine();
    l.remove('\n');
    l.remove('\r');
    int size = l.length();
    bool isQuoted = false;
    for(int i = 0; i < size; i++)
    {
        if(i > 0)
        {
            if(l[i] == '"' && l[i-1] != '\\') isQuoted = !isQuoted;
            else if(!isQuoted && l[i] == separator) l[i] = QChar(1);
        }
        else
        {
            if(l[i] == '"') isQuoted = !isQuoted;
            else if(!isQuoted && l[i] == separator) l[i] = QChar(1);
        }
    }
    if(l.count(QChar(1)) + 1 > maxColumn) maxColumn = l.count(QChar(1)) + 1;
    header = l.split(QChar(1));
    csvFile.close();

    if(m_dataLinesSelector)
    {
        m_dataLinesSelector->clear();
        foreach(QString column, header)
        {
            QListWidgetItem *item = new QListWidgetItem(column);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            item->setFlags(item->flags() ^ Qt::ItemIsSelectable);
            item->setCheckState(Qt::Unchecked);
            m_dataLinesSelector->addItem(item);
        }
        if(m_csvIn)
        {
            foreach(int dataIndex, m_csvIn->dataIndexes())
            {
                if(m_dataLinesSelector->count() <= dataIndex) continue;
                m_dataLinesSelector->item(dataIndex)->setCheckState(Qt::Checked);
            }
        }
    }
}