// #if 0
// 
// #include "config.h"
// #include "info.h"
// #include "wizardpage.h"
// #include <QtCore/QDebug>
// #include <QtGui/QPushButton>
// #include <QtCore/QSettings>
// #include <QtGui/QGroupBox>
// #include <QtGui/QVBoxLayout>
// #include <QtGui/QHBoxLayout>
// #include <QtCore/QProcess>
// #include <QtGui/QLineEdit>
// #include <QtGui/QLabel>
// #include <QtGui/QFileDialog>
// #include <QtGui/QMessageBox>
// 
// namespace Core {
// 
// namespace MPlayer {
// 
// Config::Data Config::d;
// 
// Config::Data::Data()
// : volAmp(1.0), vo("auto"), ao("auto"), mplayer("mplayer")
// , filePath(Info().privatePath() + "/config_mplayer.ini")
// , options(), loaded(false) {}
// 
// ConfigWidget::ConfigWidget(QWidget *parent)
// : Core::ConfigWidget(Info(), parent) {
// 	QVBoxLayout *layout = static_cast<QVBoxLayout*>(this->layout());
// 	QGroupBox *box = new QGroupBox(trUtf8("MPlayer 경로"));
// 	mplayer = new QLineEdit;
// 	find = new QPushButton(trUtf8("찾기"));
// 	open = new QPushButton(trUtf8("열기"));
// 	QHBoxLayout *hbox = new QHBoxLayout;
// 	hbox->addWidget(mplayer);
// 	hbox->addWidget(find);
// 	hbox->addWidget(open);
// 	box->setLayout(hbox);
// 	layout->insertWidget(2, box);
// 
// 	box = new QGroupBox(trUtf8("실행 옵션"));
// 	options = new QLineEdit;
// 	QVBoxLayout *vbox = new QVBoxLayout;
// 	QLabel *label = new QLabel(trUtf8("MPlayer를 다음 옵션과 함께 실행합니다."));
// 	label->setWordWrap(true);
// 	vbox->addWidget(label);
// 	vbox->addWidget(options);
// 	label = new QLabel(trUtf8("주) 실행 옵션을 변경하면 "
// 			"정상적으로 작동하지 않을 수 있습니다."));
// 	label->setWordWrap(true);
// 	vbox->addWidget(label);
// 	box->setLayout(vbox);
// 	layout->insertWidget(3, box);
// 
// 	connect(find, SIGNAL(clicked()), this, SLOT(findMPlayer()));
// 	connect(open, SIGNAL(clicked()), this, SLOT(openMPlayer()));
// }
// 
// void ConfigWidget::findMPlayer() {
// 	QProcess proc;
// 	QStringList args("mplayer");
// 	proc.start("whereis", args, QProcess::ReadOnly);
// 	if (proc.waitForFinished(5000)) {
// 		const QString output = QString::fromLocal8Bit(proc.readAllStandardOutput());
// 		const QStringList list = output.split(' ');
// 		if (list.size() > 1) {
// 			mplayer->setText(list[1]);
// 			return;
// 		}
// 	}
// 	QMessageBox::warning(this, trUtf8("MPlayer 찾기"), trUtf8("MPlayer "
// 			"실행파일을 찾지 못하였습니다. '열기'버튼을 눌러 직접 경로를 지정해주세요."));
// }
// 
// void ConfigWidget::openMPlayer() {
// 	QString path = QFileDialog::getOpenFileName(this);
// 	if (!path.isEmpty())
// 		mplayer->setText(path);
// }
// 
// QWidget *Config::widget(QWidget *parent) {
// 	ConfigWidget *w = new ConfigWidget(parent);
// 	w->setVolumeAmplification(d.volAmp);
// 	w->setVideoDriver(d.vo);
// 	w->setAudioDriver(d.ao);
// 	w->mplayer->setText(d.mplayer);
// 	w->options->setText(d.options.join(" "));
// 	return w;
// }
// 
// void Config::update(QWidget *widget) {
// 	ConfigWidget *w = qobject_cast<ConfigWidget*>(widget);
// 	if (!w)
// 		return;
// 	d.volAmp = w->volumeAmplification();
// 	d.vo = w->videoDriver();
// 	d.ao = w->audioDriver();
// 	d.mplayer = w->mplayer->text();
// 	d.options = w->options->text().trimmed().split(' ');
// }
// 
// void Config::save() {
// 	QSettings set(d.filePath, QSettings::IniFormat);
// 	set.beginGroup("Audio");
// 	set.setValue("AudioDriver", d.ao);
// 	set.setValue("VolumeAmplification", d.volAmp);
// 	set.endGroup();
// 	set.beginGroup("Video");
// 	set.setValue("VideoDriver", d.vo);
// 	set.endGroup();
// 	set.beginGroup("Etc");
// 	set.setValue("MPlayer", d.mplayer);
// 	set.setValue("Options", d.options);
// 	set.endGroup();
// }
// 
// void Config::load() {
// 	QSettings set(d.filePath, QSettings::IniFormat);
// 	set.beginGroup("Audio");
// 	d.ao = set.value("AudioDriver", "auto").toString();
// 	d.volAmp = set.value("VolumeAmplification", 1.0).toDouble();
// 	set.endGroup();
// 	set.beginGroup("Video");
// 	d.vo = set.value("VideoDriver", "auto").toString();
// 	set.endGroup();
// 	set.beginGroup("Etc");
// 	d.mplayer = set.value("MPlayer", "mplayer").toString();
// 	d.options = set.value("Options", QStringList()).toStringList();
// 	set.endGroup();
// }
// 
// QList<Core::WizardPage*> Config::wizard(QWidget *parent) {
// 	QList<Core::WizardPage*> pages;
// 	pages.append(new WizardPage(parent));
// 	return pages;
// }
// 
// void Config::update(Core::WizardPage *wizard) {
// 	WizardPage *page = qobject_cast<WizardPage*>(wizard);
// 	if (page) {
// 	}
// }
// 
// }
// 
// }
// 
// #endif
// 
