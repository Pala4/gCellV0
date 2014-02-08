#ifndef CELEMENTOPTIONSWGT_H
#define CELEMENTOPTIONSWGT_H

#include <QDialog>

class QTabWidget;
class QGridLayout;
class QFormLayout;
class QLineEdit;
class QCheckBox;

class CElement;

class CElementOptionsWgt : public QDialog
{
	Q_OBJECT
private:
	QTabWidget *m_tabWidget;
	QGridLayout *m_generalLayout;
	QFormLayout *m_generalFormLayout;
	QLineEdit *m_captionEdit;
	QCheckBox *m_captionVisibleChBx;

	CElement *m_element;
public:
	explicit CElementOptionsWgt(CElement *element, QWidget *parent = 0);

	QTabWidget* tabWidget(void){return m_tabWidget;}
	QGridLayout* generalLayout(void){return m_generalLayout;}
	QFormLayout* generalFormLayout(void){return m_generalFormLayout;}
	QLineEdit* captionEdit(void){return m_captionEdit;}

	QString elementCaption(void);
	bool isCaptionVisible(void) const;
	void setCaptionVisible(const bool &captionVisible);
};

#endif // CELEMENTOPTIONSWGT_H