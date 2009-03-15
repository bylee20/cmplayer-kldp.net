#include "geturldialog.h"
#include "encodingcombobox.h"
#include "ui_geturldialog.h"
#include <core/playlist.h>
#include <core/info.h>
#include <QtCore/QFileInfo>
#include <QtCore/QTemporaryFile>
#include <QtCore/QDir>
#include <QtCore/QEventLoop>
#include <QtNetwork/QHttp>
#include <QtCore/QTimer>

struct GetUrlDialog::Data {
	Ui::GetUrlDialog ui;
	QHttp *http;
	int id;
	QEventLoop loop;
	QTimer timer;
};

GetUrlDialog::GetUrlDialog(QWidget *parent)
: QDialog(parent), d(new Data) {
	d->ui.setupUi(this);
	d->http = new QHttp(this);
	d->id = -1;
	d->timer.setSingleShot(true);
	connect(d->http, SIGNAL(requestFinished(int, bool)), this, SLOT(slotFinished(int)));
	connect(&d->timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

GetUrlDialog::~GetUrlDialog() {
	delete d;
}

void GetUrlDialog::accept() {
	QDialog::accept();
}

QUrl GetUrlDialog::url() const {
	return QUrl(d->ui.url->text().trimmed());
}

bool GetUrlDialog::isPlaylist() const {
	const QString suffix = QFileInfo(url().path()).suffix();
	return suffix.compare("pls", Qt::CaseInsensitive) == 0;
}

Core::Playlist GetUrlDialog::playlist() const {
	if (!isPlaylist())
		return Core::Playlist();
	const QUrl url = this->url();
	QTemporaryFile file(Core::Info::privatePath() + "/temp_XXXXXX.pls");
	if (!file.open())
		return Core::Playlist();
	const QHttp::ConnectionMode mode = url.scheme().toLower() == "https"
			? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	d->http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());
	if (!url.userName().isEmpty())
		d->http->setUser(url.userName(), url.password());
	const QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
	d->id = d->http->get(path, &file);
	d->timer.start(60000);
	d->loop.exec();
	if (d->http->error() != QHttp::NoError)
		return Core::Playlist();
	Core::Playlist list;
	list.load(&file, d->ui.enc->encoding());
	return list;
}

void GetUrlDialog::slotTimeout() {
	d->http->abort();
	d->loop.quit();
}

void GetUrlDialog::slotFinished(int id) {
	if (id == d->id) {
		d->timer.stop();
		d->loop.quit();
	}
}

void GetUrlDialog::setUrl(const QUrl &url) {
	d->ui.url->setText(url.toString());
}

void GetUrlDialog::showEvent(QShowEvent *event) {
	d->ui.retranslateUi(this);
	QDialog::showEvent(event);
}
