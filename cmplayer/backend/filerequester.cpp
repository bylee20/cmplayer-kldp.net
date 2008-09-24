#include "filerequester.h"
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtCore/QProcess>
#include <QtCore/QFileInfo>

namespace Backend {

struct FileRequester::Data {
	QLineEdit *fileEdit;
	QPushButton *findButton, *openButton;
	QLabel *valid_label;
	QString fileName;
};

FileRequester::FileRequester(const QString &fileName, QWidget *parent)
: QWidget(parent), d(new Data) {
	d->fileName = fileName;
	d->fileEdit = new QLineEdit;
	d->findButton = new QPushButton(trUtf8("찾기"));
	d->openButton = new QPushButton(trUtf8("열기"));
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(d->fileEdit);
	hbox->addWidget(d->findButton);
	hbox->addWidget(d->openButton);
	d->valid_label = new QLabel;
	QVBoxLayout *vbox = new QVBoxLayout;
	vbox->addLayout(hbox);
	vbox->addWidget(d->valid_label);
	setLayout(vbox);
	
	connect(d->findButton, SIGNAL(clicked()), this, SLOT(find()));
	connect(d->openButton, SIGNAL(clicked()), this, SLOT(open()));
	connect(d->fileEdit, SIGNAL(textChanged(const QString&))
			, this, SLOT(checkFileExists(const QString&)));
}

FileRequester::~FileRequester() {
	delete d;
}

void FileRequester::find() {
	QProcess proc;
	QStringList args(d->fileName);
	proc.start("whereis", args, QProcess::ReadOnly);
	if (proc.waitForFinished(5000)) {
		const QString output = QString::fromLocal8Bit(proc.readAllStandardOutput());
		const QStringList list = output.split(' ');
		if (list.size() > 1) {
			d->fileEdit->setText(list[1]);
			return;
		}
	}
	QMessageBox::warning(this, trUtf8("찾기"), trUtf8("%1 파일을 "
			"찾지 못하였습니다. '열기'버튼을 눌러 직접 경로를 지정해주세요.").arg(d->fileName));
}

void FileRequester::open() {
	QString filePath = QFileDialog::getOpenFileName(this);
	if (!filePath.isEmpty())
		d->fileEdit->setText(filePath);
}

void FileRequester::checkFileExists(const QString &filePath) {
	const bool exists = QFileInfo(filePath).exists();
	if (exists)
		d->valid_label->setText(trUtf8("%1 은(는) 유효한 파일입니다."));
	else
		d->valid_label->setText(trUtf8("%1 은(는) 존재하지 않습니다."));
	emit validityChanged(exists);
}

QString FileRequester::filePath() const {
	return d->fileEdit->text();
}

bool FileRequester::isValidFile() const {
	return QFileInfo(filePath()).exists();
}

}
