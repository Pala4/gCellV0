#ifndef CLINEEDITBUTTON_H
#define CLINEEDITBUTTON_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class CLineEditButton : public QWidget
{
	Q_OBJECT
private:
	QLineEdit *m_lineEdit;
	QPushButton *m_pushButton;
protected:
	QLineEdit* lineEdit(void){return m_lineEdit;}
	QPushButton* pushButton(void){return m_pushButton;}
public:
	explicit CLineEditButton(QWidget *parent = 0);

	QString text(void) const;
	void setText(const QString &text);
signals:
	void buttonClicked(void);
};

#endif // CLINEEDITBUTTON_H